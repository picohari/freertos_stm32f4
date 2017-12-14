/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#include "gfx.h"

#if GFX_USE_GINPUT && GINPUT_NEED_DIAL

#include "../../../../src/ginput/ginput_driver_dial.h"

#if GINPUT_DIAL_NUM_PORTS >= 5
	#error "GINPUT: Dial - GADC driver currently only supports 4 devices"
#endif


/* ADC handler declaration */
ADC_HandleTypeDef AdcHandle;

/* ADC sample buffer */
static uint16_t samplebuf[GINPUT_DIAL_NUM_PORTS];

#if GINPUT_DIAL_CYCLE_POLL
	static uint16_t		cycle;
#endif



static void GADCCallback(uint16_t *buffer, void *param) {
	((DialCallbackFn)param)(buffer-samplebuf, buffer[0]);
}


void ginput_lld_dial_init(void) {

	#if GINPUT_DIAL_CYCLE_POLL
		cycle = 0;
	#endif


	GPIO_InitTypeDef          GPIO_InitStruct;

	/*##-1- Enable peripherals and GPIO Clocks #################################*/
	/* ADC3 Periph clock enable */
	ADCx_CLK_ENABLE();
	/* Enable GPIO clock ****************************************/
	ADCx_CHANNEL_GPIO_CLK_ENABLE();

	/*##-2- Configure peripheral GPIO ##########################################*/ 
	/* ADC3 Channel8 GPIO pin configuration */
	GPIO_InitStruct.Pin 	= ADCx_CHANNEL_PIN;
	GPIO_InitStruct.Mode 	= GPIO_MODE_ANALOG;
	GPIO_InitStruct.Pull 	= GPIO_NOPULL;
	HAL_GPIO_Init(ADCx_CHANNEL_GPIO_PORT, &GPIO_InitStruct);


	ADC_ChannelConfTypeDef sConfig;

	/*##-1- Configure the ADC peripheral #######################################*/
	AdcHandle.Instance          		 = ADCx;

	AdcHandle.Init.ClockPrescaler        = ADC_CLOCKPRESCALER_PCLK_DIV2;
	AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
	AdcHandle.Init.ScanConvMode          = DISABLE;
	AdcHandle.Init.ContinuousConvMode    = DISABLE;
	AdcHandle.Init.DiscontinuousConvMode = DISABLE;
	AdcHandle.Init.NbrOfDiscConversion   = 0;
	AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	AdcHandle.Init.NbrOfConversion       = 1;
	AdcHandle.Init.DMAContinuousRequests = DISABLE;
	AdcHandle.Init.EOCSelection          = DISABLE;
	  
	if(HAL_ADC_Init(&AdcHandle) != HAL_OK) {
		/* Initialization Error */
		  while(1) {}
	}

	/*##-2- Configure ADC regular channel ######################################*/  
	sConfig.Channel      = ADCx_CHANNEL;
	sConfig.Rank         = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	sConfig.Offset       = 0;

	if(HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK) {
		/* Channel Configuration Error */
		  while(1) {}
	}

}

void ginput_lld_dial_poll(DialCallbackFn fn) {

	/*##-3- Start the conversion process #######################################*/  
	if(HAL_ADC_Start(&AdcHandle) != HAL_OK) {
		/* Start Conversation Error */
		  while(1) {}
	}

	/*##-4- Wait for the end of conversion #####################################*/  
	/*  Before starting a new conversion, you need to check the current state of 
	    the peripheral; if its busy you need to wait for the end of current
	    conversion before starting a new one.
	    For simplicity reasons, this example is just waiting till the end of the 
	    conversion, but application may perform other tasks while conversion 
	    operation is ongoing. */
	HAL_ADC_PollForConversion(&AdcHandle, 10);

	/* Check if the continuous conversion of regular channel is finished */
	if((HAL_ADC_GetState(&AdcHandle) & HAL_ADC_STATE_EOC_REG) == HAL_ADC_STATE_EOC_REG) {
		/*##-5- Get the converted value of regular channel #######################*/
		//uhADCxConvertedValue = HAL_ADC_GetValue(&AdcHandle);
		samplebuf[0] = HAL_ADC_GetValue(&AdcHandle);

		fn(0, samplebuf[0]);
	}

}

#endif /* GFX_USE_GINPUT && GINPUT_NEED_DIAL */
