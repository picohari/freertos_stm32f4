/**
  ******************************************************************************
  * @file    RTC/RTC_Calendar/Src/stm32f4xx_hal_msp.c
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

#include "rtc_clock.h"
/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @defgroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;

timestamp_t clock_timestamp = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief RTC MSP Initialization 
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  * 
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select 
  *        the RTC clock source; in this case the Backup domain will be reset in  
  *        order to modify the RTC Clock source, as consequence RTC registers (including 
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *             
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
     - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to 
       configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and 
       __HAL_RCC_BACKUPRESET_RELEASE().
     - Configure the needed RTc clock source */
  
  /*##-1- Configue LSE as RTC clock soucre ###################################*/ 
  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  { 
    Error_Handler();
  }
  
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  { 
    Error_Handler();
  }
  
  /*##-2- Enable RTC peripheral Clocks #######################################*/ 
  /* Enable RTC Clock */ 
  __HAL_RCC_RTC_ENABLE(); 
}

/**
  * @brief RTC MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
   __HAL_RCC_RTC_DISABLE();
}


void rtc_init(void)
{
/*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;

  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */ 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  

#if 0
  /*##-2- Check if Data stored in BackUp register0: No Need to reconfigure RTC#*/
  /* Read the Back Up Register 0 Data */
  if(HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != 0x32F2)
  {  
    /* Configure RTC Calendar */
    RTC_CalendarConfig();
  }
  else
  {
    /* Check if the Power On Reset flag is set */  
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
    {
      /* Turn on LED2: Power on reset occurred */
      BSP_LED_On(LED3);
    }
    /* Check if Pin Reset flag is set */
    if(__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
    {
      /* Turn on LED4: External reset occurred */
      BSP_LED_On(LED4);
    }
    /* Clear source Reset Flag */
    __HAL_RCC_CLEAR_RESET_FLAGS();
  }
#endif
}



#include <sys/time.h>

int _gettimeofday( struct timeval *tv, void *tzvp )
{
    /* clock_timestamp++ occurs every millisecond in SysTick_Handler() */
    tv->tv_sec = clock_timestamp / 1000;              // convert to seconds
    tv->tv_usec = (clock_timestamp % 1000) * 1000;
  
    return 0;  // return non-zero for error
} // end _gettimeofday()




/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_CalendarConfig(uint8_t year, uint8_t month, uint8_t day, uint8_t dow, uint8_t hour, uint8_t min)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
    
  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = year;
  sdatestructure.Month = month;
  sdatestructure.Date = day;
  sdatestructure.WeekDay = dow;
  
  if(HAL_RTC_SetDate(&RtcHandle, &sdatestructure, RTC_FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  } 
  
  /*##-2- Configure the Time #################################################*/
  /* Set Time: 02:00:00 */
  stimestructure.Hours = hour;
  stimestructure.Minutes = min;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT_24;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BIN) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
  
  /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR0, 0x32F2);  
}

/**
  * @brief  Display the current time and date.
  * @param  showtime : pointer to buffer
  * @param  showdate : pointer to buffer
  * @retval None
  */
void RTC_CalendarShow(uint8_t* showtime, uint8_t* showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char*)showtime,"%02d:%02d:%02d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char*)showdate,"%02d-%02d-%02d", sdatestructureget.Date, sdatestructureget.Month, 2000 + sdatestructureget.Year);
} 




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
