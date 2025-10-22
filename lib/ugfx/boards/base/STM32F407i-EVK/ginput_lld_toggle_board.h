/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

/**
 * @file    boards/base/STM32F407i-EVK/ginput_lld_toggle_board.h
 * @brief   GINPUT Toggle low level driver source for the ChibiOS PAL hardware on the Olimex SAM7EX256 board.
 */

#include "stm32f4xx_hal.h"

#ifndef _GDISP_LLD_TOGGLE_BOARD_H
#define _GDISP_LLD_TOGGLE_BOARD_H

#define GINPUT_TOGGLE_NUM_PORTS			5			// The total number of toggle inputs
#define GINPUT_TOGGLE_CONFIG_ENTRIES	1			// The total number of GToggleConfig entries

#define GINPUT_TOGGLE_LEFT			0				// Joystick Left
#define GINPUT_TOGGLE_UP			1				// Joystick Up
#define GINPUT_TOGGLE_DOWN			2				// Joystick Down
#define GINPUT_TOGGLE_RIGHT			3				// Joystick Right
#define GINPUT_TOGGLE_CENTER		4				// Joystick Center

#define GINPUT_TOGGLE_DECLARE_STRUCTURE()											\
 																					\
	const GToggleConfig GInputToggleConfigTable[GINPUT_TOGGLE_CONFIG_ENTRIES] = {	\
																					\
		{GPIOE,								/* B1..4 Joystick */		\
			GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,		\
			GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6,		\
			GPIO_MODE_INPUT},														\
	}

#endif /* _GDISP_LLD_TOGGLE_BOARD_H */

