/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_INPUT_H
#define __ADC_INPUT_H


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "cmsis_os.h"



/* Private variables ---------------------------------------------------------*/
/* ADC handler declaration */
extern ADC_HandleTypeDef AdcTempHandle;

/* Variable used to get converted value */
extern __IO uint16_t uhADCxConvertedValue;

/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance used and associated 
   resources */
/* Definition for ADCx clock resources */

#if 0
#define ADCx                            ADC3
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE()
#define DMAx_CLK_ENABLE()               __HAL_RCC_DMA2_CLK_ENABLE()     
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
     
#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_9
#define ADCx_CHANNEL_GPIO_PORT          GPIOF 

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_7

/* Definition for ADCx's NVIC */
#define ADCx_IRQn                      ADC_IRQn

/* Definition for ADCx's DMA */
#define ADCx_DMA_CHANNEL                DMA_CHANNEL_2
#define ADCx_DMA_STREAM                 DMA2_Stream0         

/* Definition for ADCx's NVIC */
#define ADCx_DMA_IRQn                   DMA2_Stream0_IRQn
#define ADCx_DMA_IRQHandler             DMA2_Stream0_IRQHandler
#endif


extern uint16_t adc_temp_value;

extern float temperature;


void adc_temp_init(void);

float get_temperature(void);



#endif /* __ADC_INPUT_H */
