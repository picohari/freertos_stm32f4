/**
  ******************************************************************************
  * @file    UART/UART_Printf/Src/stm32f4xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    17-February-2017
  * @brief   HAL MSP module.    
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "XCore407I.h"

#include "stm32f4xx_it.h"

#include "uart.h"

#include <stdarg.h>
#include <string.h>


/* Recast system printf to uart output */


/* UART handler declaration */
UART_HandleTypeDef      hUART;

QueueHandle_t   rx_char_queue;     /* queues the received character from ISR */

//uint8_t rx_buffer[2];


/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{ 
  
  (void) huart;

  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable USARTx clock */
  USARTx_CLK_ENABLE(); 

  /* Enable GPIO clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();
  
  /*##-2- Configure peripheral GPIO ##########################################*/
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FAST;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;
  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);
    
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_RX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for UART ########################################*/   
  /* NVIC for USARTx */
  HAL_NVIC_SetPriority(USARTx_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY, 1); /* Must be same prio as tasks... */
  HAL_NVIC_EnableIRQ(USARTx_IRQn);

}

/**
  * @brief UART MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

  (void) huart;

  /*##-1- Reset peripherals ##################################################*/
  USARTx_FORCE_RESET();
  USARTx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure UART Tx as alternate function  */
  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
  /* Configure UART Rx as alternate function  */
  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);
  
  /*##-3- Disable the NVIC for UART ##########################################*/
  HAL_NVIC_DisableIRQ(USARTx_IRQn);

}




void uart_init(void)
{
  /*##-1- Configure the UART peripheral ######################################*/
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART1 configured as follow:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = ODD parity
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */
  hUART.Instance          = USARTx;
  
  hUART.Init.BaudRate     = 115200;
  hUART.Init.WordLength   = UART_WORDLENGTH_8B;
  hUART.Init.StopBits     = UART_STOPBITS_1;
  hUART.Init.Parity       = UART_PARITY_NONE;
  hUART.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
  hUART.Init.Mode         = UART_MODE_TX_RX;
  hUART.Init.OverSampling = UART_OVERSAMPLING_16;
    
  if(HAL_UART_Init(&hUART) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }

  /* Switch to queue-less mode (direct feed into rx_buffer */
  //HAL_UART_Receive_IT(&hUART, rx_buffer, 1);    //activate uart rx interrupt every time receiving 1 byte
  __HAL_UART_ENABLE_IT(&hUART, UART_IT_RXNE);

  /* Set stdout buffer to 0 for immediate output of printf via _write() */
  setbuf(stdout, NULL);

  /* Initalize char receive queue */
  rx_char_queue = xQueueCreate(UART_RX_CHAR_QUEUE_SIZE, 1);
}


/* IRQ handler trampoline for HAL UART Driver */
void USARTx_IRQHandler(void)
{

#if 1
  // We handle RX here, but TX is handled by HAL_UART implementation...
  int32_t const flag = __HAL_UART_GET_FLAG(&hUART, UART_FLAG_RXNE);
  int32_t const it_source = __HAL_UART_GET_IT_SOURCE(&hUART, UART_IT_RXNE);

  if ((flag != RESET) && (it_source != RESET)) {

      // RX Read data from data register
      char const c = (char)(hUART.Instance->DR & (uint8_t)0x00FF);

      static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

      /* Put the character in queue*/
      xQueueSendToBackFromISR(rx_char_queue, &c, &xHigherPriorityTaskWoken);

      portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
  } else {
      // TX as usual ...
      HAL_UART_IRQHandler(&hUART);

  }
#else
      HAL_UART_IRQHandler(&hUART);
#endif

}

















































/*! Groesse des Puffers fuer die Logausgaben ueber UART und ueber TCP/IP. */
#define LOG_BUFFER_SIZE   1024
#define LINE_FEED         "\r\n"    /**< Linefeed fuer nicht Windows */

/*! Puffer fuer das Zusammenstellen einer Logausgabe */
static char log_buffer[LOG_BUFFER_SIZE];

/*!
 * Schreibt die eigentliche Ausgabeinformation in den Puffer.
 * @param format Format
 */
void UART_log_printf(const char * format, ...) {
  va_list args;
  unsigned int len = strlen(log_buffer);

  va_start(args, format);
  vsnprintf(&log_buffer[len], LOG_BUFFER_SIZE - len, format, args);
  va_end(args);

  return;
}

void UART_log_send(void) {
  /* String ueber UART senden, ohne '\0'-Terminierung */
  HAL_UART_Transmit(&hUART, (uint8_t *)log_buffer, strlen(log_buffer), 0xFFFF);
  /* Line feed senden */
  HAL_UART_Transmit(&hUART, (uint8_t *)LINE_FEED, strlen(LINE_FEED), 0xFFFF);

  memset(log_buffer, 0, sizeof(log_buffer));

  return;
}

void UART_txt_send(void) {

  /* String ueber UART senden, ohne '\0'-Terminierung */
  HAL_UART_Transmit(&hUART, (uint8_t *)log_buffer, strlen(log_buffer), 0xFFFF);

  memset(log_buffer, 0, sizeof(log_buffer));
}
