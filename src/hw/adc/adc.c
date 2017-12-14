
/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include "XCore407I.h"

#include "gfx.h"
#include "pages/zen_main_home.h"
#include "adc.h"



/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
ADC_HandleTypeDef AdcTempHandle;


/* Variable used to get converted value */
__IO uint16_t uhADCxConvertedValue = 0;


/**
  * @brief ADC MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

  (void) hadc;

  //GPIO_InitTypeDef          GPIO_InitStruct;
  
  /* Internal Temperature Sensor */
  //__HAL_RCC_ADC1_CLK_ENABLE();

  /* Wake up temperature sensor and Vref */
  //ADC->CCR |= ADC_CCR_TSVREFE;
  //ADC->CCR &= ~ADC_CCR_VBATE ;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* ADC3 Periph clock enable */
  //ADCx_CLK_ENABLE();
  /* Enable GPIO clock ****************************************/
  //ADCx_CHANNEL_GPIO_CLK_ENABLE()
  
  /*##-2- Configure peripheral GPIO ##########################################*/ 
  /* ADC3 Channel8 GPIO pin configuration */
  //GPIO_InitStruct.Pin = ADCx_CHANNEL_PIN;
  //GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  //GPIO_InitStruct.Pull = GPIO_NOPULL;
  //HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the NVIC #################################################*/
  /* NVIC configuration for DMA transfer complete interrupt (USART1_TX) */
  //HAL_NVIC_SetPriority(ADCx_IRQn, configLIBRARY_LOWEST_INTERRUPT_PRIORITY, 1);
  //HAL_NVIC_EnableIRQ(ADCx_IRQn);
}


/**
  * @brief ADC MSP De-Initialization 
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO to their default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{

  (void) hadc;
  
  /*##-1- Reset peripherals ##################################################*/
  //ADCx_FORCE_RESET();
  //ADCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize the ADC3 Channel8 GPIO pin */
  //HAL_GPIO_DeInit(GPIOF, GPIO_PIN_9);

  /*##-3- Disable the NVIC for UART ##########################################*/
  //HAL_NVIC_DisableIRQ(ADC_IRQn);

}




void adc_temp_init(void) {

  ADC_ChannelConfTypeDef sConfig;
  
#if 1
  /*##-1- Configure the ADC peripheral #######################################*/
  AdcTempHandle.Instance          		     = ADC1;
  
  AdcTempHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV4;
  AdcTempHandle.Init.Resolution            = ADC_RESOLUTION_12B;
  AdcTempHandle.Init.ScanConvMode          = DISABLE;
  AdcTempHandle.Init.ContinuousConvMode    = DISABLE;
  AdcTempHandle.Init.DiscontinuousConvMode = DISABLE;
  AdcTempHandle.Init.NbrOfDiscConversion   = 0;
  AdcTempHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcTempHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;
  AdcTempHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
  AdcTempHandle.Init.NbrOfConversion       = 16;
  AdcTempHandle.Init.DMAContinuousRequests = DISABLE;
  AdcTempHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
      
  if(HAL_ADC_Init(&AdcTempHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Configure ADC regular channel ######################################*/  
  sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank         = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  sConfig.Offset       = 0;
  
  if(HAL_ADC_ConfigChannel(&AdcTempHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }
  
#if 0
  /*##-3- Start the conversion process #######################################*/  
  if(HAL_ADC_Start(&AdcTempHandle) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
  
  /*##-4- Wait for the end of conversion #####################################*/  
   /*  Before starting a new conversion, you need to check the current state of 
        the peripheral; if its busy you need to wait for the end of current
        conversion before starting a new one.
        For simplicity reasons, this example is just waiting till the end of the 
        conversion, but application may perform other tasks while conversion 
        operation is ongoing. */
  HAL_ADC_PollForConversion(&AdcTempHandle, 10);
  
  /* Check if the continuous conversion of regular channel is finished */
  if((HAL_ADC_GetState(&AdcTempHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG)
  {
    /*##-5- Get the converted value of regular channel #######################*/
    uhADCxConvertedValue = HAL_ADC_GetValue(&AdcTempHandle);
  }
#endif
#endif



#if 0
/*##-1- Configure the ADC peripheral #######################################*/
  AdcTempHandle.Instance          			= ADCx;
  
  AdcTempHandle.Init.ClockPrescaler 		= ADC_CLOCKPRESCALER_PCLK_DIV2;
  AdcTempHandle.Init.Resolution 			= ADC_RESOLUTION_12B;
  AdcTempHandle.Init.ScanConvMode 			= DISABLE;
  AdcTempHandle.Init.ContinuousConvMode 	= DISABLE;
  AdcTempHandle.Init.DiscontinuousConvMode 	= DISABLE;
  AdcTempHandle.Init.NbrOfDiscConversion 	= 0;
  AdcTempHandle.Init.ExternalTrigConvEdge 	= ADC_EXTERNALTRIGCONVEDGE_NONE;
  AdcTempHandle.Init.ExternalTrigConv 		= ADC_EXTERNALTRIGCONV_T1_CC1;
  AdcTempHandle.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
  AdcTempHandle.Init.NbrOfConversion 		= 1;
  AdcTempHandle.Init.DMAContinuousRequests 	= DISABLE;
  AdcTempHandle.Init.EOCSelection 			= DISABLE;
      
  if(HAL_ADC_Init(&AdcTempHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
  
  /*##-2- Configure ADC regular channel ######################################*/  
  sConfig.Channel = ADCx_CHANNEL;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  sConfig.Offset = 0;
  
  if(HAL_ADC_ConfigChannel(&AdcTempHandle, &sConfig) != HAL_OK)
  {
    /* Channel Configuration Error */
    Error_Handler();
  }

#if 0
  /*##-3- Start the conversion process and enable interrupt ##################*/  
  if(HAL_ADC_Start_IT(&AdcTempHandle) != HAL_OK)
  {
    /* Start Conversation Error */
    Error_Handler();
  }
#endif

#endif




}



#if 0
/**
  * @brief  Conversion complete callback in non blocking mode 
  * @param  AdcTempHandle : AdcTempHandle handle
  * @note   This example shows a simple way to report end of conversion, and 
  *         you can add your own implementation.    
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcTempHandle)
{
  /* Get the converted value of regular channel */
  //uhADCxConvertedValue = HAL_ADC_GetValue(AdcTempHandle);

  char adc_string[16];
  sprintf(adc_string, "%d", uhADCxConvertedValue);

  gwinSetText(ghLabelADCval, adc_string, 1);
}
#endif




uint16_t adc_temp_value;

float temperature;



float get_temperature(void) {

  return 0;
}


