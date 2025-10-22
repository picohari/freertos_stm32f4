/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    boards/base/STM32F407i-EVK/ginput_lld_dial_board.h
 * @brief   GINPUT Dial Driver config file.
 */

#ifndef _GINPUT_LLD_DIAL_BOARD_H
#define _GINPUT_LLD_DIAL_BOARD_H

#if GFX_USE_GINPUT && GINPUT_NEED_DIAL

#include "stm32f4xx_hal.h"
 
/*===========================================================================*/
/* Analogue devices on this board                                            */
/*===========================================================================*/

#define GINPUT_DIAL_NUM_PORTS			1

#define GINPUT_DIAL_DEVICE0				

#define GINPUT_DIAL_POLL_PERIOD			200

#define GINPUT_DIAL_CYCLE_POLL			FALSE


#define GADC_BITS_PER_SAMPLE			12


#define ADCx                            ADC3
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC3_CLK_ENABLE();
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOF_CLK_ENABLE()
     
#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN                GPIO_PIN_9
#define ADCx_CHANNEL_GPIO_PORT          GPIOF 

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_7




#endif	/* GFX_USE_GINPUT && GINPUT_NEED_DIAL */

#endif	/* _GINPUT_LLD_DIAL_BOARD_H */

