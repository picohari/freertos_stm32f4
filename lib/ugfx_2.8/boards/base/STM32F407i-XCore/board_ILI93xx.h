/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.org/license.html
 */

#ifndef GDISP_LLD_BOARD_H
#define GDISP_LLD_BOARD_H

#include "XCore407I.h"

/* FSMC allows direct write acces to memory */
#define  LCD_REG     (*((volatile unsigned short *) 0x6C000000))
#define  LCD_RAM     (*((volatile unsigned short *) 0x6C010000))


static GFXINLINE void init_board(GDisplay *g) {

	// As we are not using multiple displays we set g->board to NULL as we don't use it.
	g->board = 0;

	switch(g->controllerdisplay) {
		case 0:											// Set up for Display 0
			/* Set up of LCD port in BSP driver */
			BSP_LCD_Init();			
			break;
	}

	//LOG_UART("uGFX: LCD init");
}

static GFXINLINE void post_init_board(GDisplay *g) {
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, bool_t state) {
	(void) g;
	(void) state;
}

static GFXINLINE void set_backlight(GDisplay *g, uint8_t percent) {
	(void) g;
	(void) percent;
}

static GFXINLINE void acquire_bus(GDisplay *g) {
	(void) g;
}

static GFXINLINE void release_bus(GDisplay *g) {
	(void) g;
}

static GFXINLINE void write_index(GDisplay *g, uint16_t index) {
	(void) g;
	LCD_REG = index;
}

static GFXINLINE void write_data(GDisplay *g, uint16_t data) {
	(void) g;
	LCD_RAM = data;
}

static GFXINLINE void setreadmode(GDisplay *g) {
	(void) g;
}

static GFXINLINE void setwritemode(GDisplay *g) {
	(void) g;
}

static GFXINLINE uint16_t read_data(GDisplay *g) {
	(void) g;
	return LCD_RAM;
}

#endif /* GDISP_LLD_BOARD_H */
