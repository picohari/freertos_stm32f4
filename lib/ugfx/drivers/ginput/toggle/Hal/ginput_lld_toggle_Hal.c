/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#include "gfx.h"

#if (GFX_USE_GINPUT && GINPUT_NEED_TOGGLE) /*|| defined(__DOXYGEN__)*/

#include "../../../../src/ginput/ginput_driver_toggle.h"

#include "stm32f4xx_hal.h"


GINPUT_TOGGLE_DECLARE_STRUCTURE();

void ginput_lld_toggle_init(const GToggleConfig *ptc) {

	(void) ptc;

	GPIO_InitTypeDef  GPIO_InitStruct;												
 																					
	/* Configure the input pin */
	__GPIOE_CLK_ENABLE();	

	GPIO_InitStruct.Pin   = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6;	
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;													
	GPIO_InitStruct.Pull  = GPIO_PULLUP;												
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);											
}

unsigned ginput_lld_toggle_getbits(const GToggleConfig *ptc) {

	(void) ptc;

	return GPIOE->IDR;
}

#endif /* GFX_USE_GINPUT && GINPUT_NEED_TOGGLE */
