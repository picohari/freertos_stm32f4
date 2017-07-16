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

#include "uart3.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TX_QUEUE_SIZE               (1)   //!< Queue size is 1, messages are sent byte by byte across the queue.
#define RX_QUEUE_SIZE               (1)   //!< Queue size is 1, messages are sent byte by byte across the queue.

#define TX_QUEUE_MAX_WAIT_TIME_MS         (1000)  //!< Max time (in ms) to wait for putting character onto tx queue before error occurs

//! Max time (in ms) to wait for another task to finish putting a string onto the tx queue
//! Since only the DEBUG task is using this UART, the semaphore should never have to be
//! waited on.
#define TX_SEMAPHORE_MAX_WAIT_TIME_MS       (1000)

//! Time to wait for another char to arrive on tx queue before the UART module is slept.
#define TIME_TO_WAIT_FOR_ANOTHER_CHAR_BEFORE_SLEEPING_MS (5)

#define TICKS_IN_DAY   (pdMS_TO_TICKS(24 * 60 * 60 * 1000))

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef      hUART;

static TaskHandle_t tx_task_handle;
static TaskHandle_t rx_task_handle;

static QueueHandle_t tx_queue;
static QueueHandle_t rx_queue;          
static QueueHandle_t rx_char_queue;     /* queues the received character from ISR */




#if 1
char cmd_string[MAXCLISTRING]; // where we build our string from characters coming in
int rx_index = 0; // index for going though cmd_string
//char cmd_buf[2];
#endif

uint8_t rx_buffer[2];


#include "syscalls.c"


/* Arrow keys */
static char const seq_up[] = {0x1B, 0x5B, 0x41};
static char const seq_down[] = {0x1B, 0x5B, 0x42};
static char const seq_left[] = {0x1B, 0x5B, 0x44};
static char const seq_right[] = {0x1B, 0x5B, 0x43};

/* Private function prototypes -----------------------------------------------*/

static void os_uart_tx_task (void const * arg);
static void os_uart_rx_task (void const * arg);






/* Private functions ---------------------------------------------------------*/

//FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

#if 0
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int __io_putchar(int ch)
{

  HAL_UART_Transmit(&hUART, (uint8_t *)&ch, 1, 0xFFFF);

  return (ch);
}
#endif



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
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC for UART ########################################*/   
  /* NVIC for USARTx */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
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


  /* Create queues */
  rx_queue = xQueueCreate( UART_RX_QUEUE_SIZE , sizeof(char*));
  tx_queue = xQueueCreate( UART_TX_QUEUE_SIZE , sizeof(char*));

#if 0
  /* Create tasks */
  osThreadDef(uart_rx, os_uart_rx_task,   osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  rx_task_handle = osThreadCreate( osThread(uart_rx),  NULL);

  osThreadDef(uart_tx, os_uart_tx_task,   osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  tx_task_handle = osThreadCreate( osThread(uart_tx),  NULL);
#endif

  /* Switch to queue-less mode (direct feed into rx_buffer */
  HAL_UART_Receive_IT(&hUART, rx_buffer, 1);    //activate uart rx interrupt every time receiving 1 byte

  setbuf(stdout, NULL);
}










static void *os_malloc(size_t const size) {
    void * const rc = pvPortMalloc(size);
    if (rc == NULL) {
        Error_Handler();
    }
    return rc;
}

static char * os_strdup(char const * const s) {
    char *dst = os_malloc(strlen(s) + 1);
    strcpy(dst, s);
    return dst;
}

static char *os_strndup(char const * const s, size_t const size) {
    char *dst = os_malloc(size  + 1);
    strncpy(dst, s, size);
    *(dst+size) = '\0';
    return dst;
}



void uart_send(char const * const str) {
    char const * const dupped = os_strdup(str);
    if (xQueueSendToBack(tx_queue, &dupped, 0) == errQUEUE_FULL) {
        vPortFree((void*)dupped);
    }
}

void queue_rx(char const * const str, size_t const len) {
    char const * const dupped = os_strndup(str, len);
    if (xQueueSendToBack(rx_queue, &dupped, 0) == errQUEUE_FULL) {
        vPortFree((void*)dupped);
    }
}

char *uart_receive(void) {
    char * str;
    for (;;) {
        int const queue_rc = xQueueReceive(rx_queue, &str, TICKS_IN_DAY);
        if (queue_rc == pdFALSE) {
            continue; // Try again
        }
        return str;
    }
}











__attribute__((noreturn))
static void os_uart_tx_task(void const * arg) {

    (void) arg;

    for(;;) {

        char *str;

        // Wait for a sting to be queued. Returns false if timeout... (nothing to send)
        int const queue_rc = xQueueReceive(tx_queue, &str, TICKS_IN_DAY);
        if (queue_rc == pdFALSE) {
            continue; // Try again
        }

        // Begin non-blocking interrupt-based transmission
        HAL_UART_Transmit_IT(&hUART, (uint8_t*)str, strlen(str));

        // Block this FreeRTOS thread until we get notification from HAL_UART_TxCpltCallback (called from ISR)
        // this can also timeout...
        ulTaskNotifyTake( /* xClearCountOnExit = */ pdTRUE, pdMS_TO_TICKS(1000));

        // Free memory
        vPortFree((void*)str);
    }
}



__attribute__((noreturn))
static void os_uart_rx_task(void const * arg)
{
    (void) arg;

    static char rx_buf[UART_RX_BUF_LEN];
    static int  rx_buf_count;

    // Enable interrupt RX Not Empty Interrupt
    __HAL_UART_ENABLE_IT(&hUART, UART_IT_RXNE);

    rx_buf_count = 0;

    for(;;) {
        // If there is something received, then we clear it in 60 seconds anyway, otherwise no need to wake up
        int const timeout = rx_buf_count ? pdMS_TO_TICKS(60000) : TICKS_IN_DAY;

        // Wait for a char to be queued. Returns false if timeout... (nothing received)
        char * const buf_empty_pos = rx_buf + rx_buf_count;

        int const queue_rc = xQueueReceive(rx_char_queue, buf_empty_pos, timeout);
        if (queue_rc == pdFALSE) {
            rx_buf_count = 0;
            continue; // Try again
        }

        // Process
        char const c = *buf_empty_pos;

        if (c == '\r') {
            if (rx_buf_count) {
                // There is some cmd in buffer
                // ECHO newline
                uart_send("!!!\r\n");

                // Queue newline into rx_queue
                //queue_rx(rx_buf, rx_buf_count);

                // Reset our buffer position
                rx_buf_count = 0;
            } else {
                // No data in buffer, just echo..
                // ECHO newline
                uart_send("\r\n");
            }
        } else if (rx_buf_count < UART_RX_BUF_LEN - 1) {
            // Not overflow
            rx_buf_count++;

            // ECHO
            if (*rx_buf == 0x1b) {
                // Seems like an escape sequence
                if (rx_buf_count == sizeof(seq_up) && !strncmp(rx_buf, seq_up, sizeof(seq_up))) {
                    //queue_rx(AK_UART_UP_KEY, sizeof(AK_UART_UP_KEY));
                    rx_buf_count = 0;
                } else if (rx_buf_count == sizeof(seq_down) && !strncmp(rx_buf, seq_down, sizeof(seq_down))) {
                    //queue_rx(AK_UART_DOWN_KEY, sizeof(AK_UART_DOWN_KEY));
                    rx_buf_count = 0;
                } else if (rx_buf_count == sizeof(seq_left) && !strncmp(rx_buf, seq_left, sizeof(seq_left))) {
                    //queue_rx(AK_UART_LEFT_KEY, sizeof(AK_UART_LEFT_KEY));
                    rx_buf_count = 0;
                } else if (rx_buf_count == sizeof(seq_right) && !strncmp(rx_buf, seq_right, sizeof(seq_right))) {
                    //queue_rx(AK_UART_RIGHT_KEY, sizeof(AK_UART_RIGHT_KEY));
                    rx_buf_count = 0;
                }
            } else {
                // It's not an escape sequence, just echo it...
                char echo_buf[2];
                echo_buf[0] = c;
                echo_buf[1] = '\0';

                uart_send(echo_buf);
            }
        }
    }
}

















#if 0
void USARTx_IRQHandler(void) {

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
}
#endif


/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle. 
  * @note   This example shows a simple way to report end of IT Tx transfer, and 
  *         you can add your own implementation. 
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

  (void) huart;
  
#if 0
  /* Turn LED1 on: Transfer in transmission process is correct */
  //BSP_LED_On(LED4);
  if (huart->Instance == USART3) {

    static portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

    // Wake task that's waiting for transmission to be completed
    vTaskNotifyGiveFromISR(tx_task_handle, &xHigherPriorityTaskWoken);

    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
  }
#endif
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of IT Rx transfer, and 
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Turn LED2 on: Transfer in reception process is correct */
  BSP_LED_On(LED4);

  if (huart->Instance == USART3) {
    
    static portBASE_TYPE xHigherPriorityTaskWoken;

    // Set to false on interrupt entry
    xHigherPriorityTaskWoken = pdFALSE;

    __HAL_UART_FLUSH_DRREGISTER(huart);   // Clear the buffer to prevent overrun

    printf((char*)&rx_buffer);   // Echo the character that caused this callback so the user can see what they are typing
    //fflush(stdout);
    //printf("\n");

#if 1
 
    uint16_t rx_byte = rx_buffer[0];

    if (rx_buffer[0] == 8 || rx_buffer[0] == 127)           // If Backspace or del
    {
        printf(" \b");        // "\b space \b" clears the terminal character. Remember we just echoced a \b so don't need another one here, just space and \b
        cmd_string[rx_index-1] = 0;
        rx_index--; 
        if (rx_index < 0) rx_index = 0;
    }

    else if (rx_buffer[0] == '\n' || rx_buffer[0] == '\r')  // If Enter
    {
        printf("\nCmd: %s Length: %d", cmd_string, rx_index);
        //executeSerialCommand(cmd_string);

        /* Cleanup */
        rx_index = 0;
        memset(cmd_string, 0, sizeof(cmd_string));
    }

    else
    {
        cmd_string[rx_index] = rx_buffer[0];  // Add that character to the string

        //xQueueSendToBackFromISR(rx_queue, &rx_byte, &xHigherPriorityTaskWoken);

        rx_index++;
        if (rx_index > MAXCLISTRING)     // User typing too much, we can't have commands that big
        {
            rx_index = 0;
            memset(cmd_string, 0, sizeof(cmd_string));
            printf("\r\nCMD too long!\n");
        }
    }
#endif

    // Force a context swicth if interrupt unblocked a task with a higher or equal priority
    // to the currently running task
    portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
    
    HAL_UART_Receive_IT(&hUART, rx_buffer, 1);

  }

  
  /* Turn LED2 off: Transfer in reception process is correct */
  BSP_LED_Off(LED4);

}













/*! Groesse des Puffers fuer die Logausgaben ueber UART und ueber TCP/IP. */
#define LOG_BUFFER_SIZE   128
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


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
