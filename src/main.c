/**
  ******************************************************************************
  * @file    src/main.c 
  * @author  Harald W. Leschner (DK6YF) - H9 Laboratory Ltd.
  * @version V1.0.0
  * @date    15-July-2017
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
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

#include "uart3.h"

#include "gfx.h"
#include "gui.h"
#include "vt100.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define LED1_UPDATE_DELAY   125
#define LED2_UPDATE_DELAY   250

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
osThreadId LEDThread1Handle, LEDThread2Handle;


/* Private function prototypes -----------------------------------------------*/
static void os_init(void);
static void os_tasks(void);
static void SystemClock_Config(void);



static void LED1_thread (void const * arg);
static void LED2_thread (void const * arg);

static void GUI_start (void const * arg);
static void GUI_thread (void const * arg);




/* Private functions ---------------------------------------------------------*/
static void os_init(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
  */
  HAL_Init();
  
  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  
  HAL_NVIC_SetPriority(SysTick_IRQn, 0x0, 0x0);

  /* Configure LEDs */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  uart_init();

  debug("Firmware %s", VERSION_STRING_LONG);
  debug("CMSIS - FreeRTOS - LwIP - BSP - uGFX");

}

static void os_tasks(void)
{

  osThreadDef(led1, LED1_thread,   osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  LEDThread1Handle = osThreadCreate( osThread(led1),  NULL);

  osThreadDef(led2, LED2_thread,   osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
  LEDThread2Handle = osThreadCreate( osThread(led2),  NULL);

  osThreadDef(gui, GUI_start,      osPriorityNormal, 0, 256);
  osThreadCreate( osThread(gui),   NULL);

}


int main(void)
{

  /* Initialize modules and hardware */
	os_init();

  /* Create all tasks */
  os_tasks();

  /* Start scheduler - should never leave this ;) */
  osKernelStart();

	for(;;)

  /* Should never ever get here ..*/
	return 0;
}



























static void GUI_start (void const * arg)
{

  (void) arg;
  
  /* Start µGFX */
  gfxInit();

  gdispSetBacklight(100);
  gdispSetContrast(100);

  guiCreate();

  osThreadDef(gui, GUI_thread, osPriorityNormal, 0, 256);
  osThreadCreate( osThread(gui), NULL);

  while (1) {
    osThreadTerminate( NULL );  /* important to stop task here !! */
  }

}

static void GUI_thread (void const * arg)
{

  (void) arg;

  while (1) {

#if 0
    if (cmd_buf[0] != 0) {
      vt100_putc(cmd_buf[0]);

      cmd_buf[0] = 0;
    }
#endif
    
    guiEventLoop();
    //osThreadTerminate( NULL );
  }
}






static void LED1_thread (void const * arg)
{

  (void) arg;

  while(1) {
    BSP_LED_On(LED1);
    osDelay(LED1_UPDATE_DELAY);
    BSP_LED_Off(LED1);
    osDelay(LED1_UPDATE_DELAY);
  }
}


static void LED2_thread (void const * arg)
{

  (void) arg;

  while(1) {
    BSP_LED_On(LED2);
    osDelay(LED2_UPDATE_DELAY);
    BSP_LED_Off(LED2);
    osDelay(LED2_UPDATE_DELAY);
  }
}


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

