/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef _GINPUT_LLD_MOUSE_BOARD_H
#define _GINPUT_LLD_MOUSE_BOARD_H

#include "stm32f4xx_hal.h"

// Resolution and Accuracy Settings
#define GMOUSE_ADS7843_PEN_CALIBRATE_ERROR		8
#define GMOUSE_ADS7843_PEN_CLICK_ERROR			6
#define GMOUSE_ADS7843_PEN_MOVE_ERROR			4
#define GMOUSE_ADS7843_FINGER_CALIBRATE_ERROR	14
#define GMOUSE_ADS7843_FINGER_CLICK_ERROR		18
#define GMOUSE_ADS7843_FINGER_MOVE_ERROR		14

// How much extra data to allocate at the end of the GMouse structure for the board's use
#define GMOUSE_ADS7843_BOARD_DATA_SIZE			0


SPI_HandleTypeDef SpiHandle;

static bool_t init_board(GMouse* m, unsigned driverinstance) {
	(void) m;

	if (driverinstance)
		return FALSE;

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* Configure the CS pin */
	__GPIOI_CLK_ENABLE();
	GPIO_InitStructure.Pin   	 = GPIO_PIN_0;
	GPIO_InitStructure.Mode  	 = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull  	 = GPIO_PULLUP;
	GPIO_InitStructure.Speed 	 = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

	/* Configure the IRQ input pin */
	__GPIOC_CLK_ENABLE();
	GPIO_InitStructure.Pin   	 = GPIO_PIN_6;
	GPIO_InitStructure.Mode  	 = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull  	 = GPIO_NOPULL;
	GPIO_InitStructure.Speed 	 = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure the MOSI, MISO and SCK pins */
	__HAL_RCC_GPIOI_CLK_ENABLE();
	GPIO_InitStructure.Pin       = GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStructure.Mode      = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull      = GPIO_PULLUP;
	GPIO_InitStructure.Speed     = GPIO_SPEED_FAST;
	GPIO_InitStructure.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOI, &GPIO_InitStructure);

	/* NVIC for SPI */
	HAL_NVIC_SetPriority(SPI2_IRQn, 0, 1);
	HAL_NVIC_EnableIRQ(SPI2_IRQn);

	/* Enable SPI clock */
	__HAL_RCC_SPI2_CLK_ENABLE();

	SpiHandle.Instance               = SPI2;
	SpiHandle.Init.Mode 			 = SPI_MODE_MASTER;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial     = 7;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;

	HAL_SPI_Init(&SpiHandle);

	return TRUE; 
}

static GFXINLINE bool_t getpin_pressed(GMouse* m) {
	(void) m;
	return !HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
}

static GFXINLINE void aquire_bus(GMouse* m) {
	(void) m;
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_RESET);
}

static GFXINLINE void release_bus(GMouse* m) {
	(void) m;
	HAL_GPIO_WritePin(GPIOI, GPIO_PIN_0, GPIO_PIN_SET);

}

static GFXINLINE uint16_t read_value(GMouse* m, uint16_t port) {
	static uint8_t txbuf[3] = {0};
	static uint8_t rxbuf[3] = {0};
	(void) m;

	txbuf[0] = port;
	HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)txbuf, (uint8_t *)rxbuf, 3, 100);

	return ((uint16_t)rxbuf[1] << 5) | (rxbuf[2] >> 3);
}

#endif /* _GINPUT_LLD_MOUSE_BOARD_H */
