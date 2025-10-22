/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.io/license.html
 */

#include "gfx.h"

#if GFX_USE_GDISP

#define GDISP_DRIVER_VMT			GDISPVMT_CH1115
#include "gdisp_lld_config.h"
#include "../../../src/gdisp/gdisp_driver.h"

#include "board_CH1115.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#ifndef GDISP_SCREEN_HEIGHT
	#define GDISP_SCREEN_HEIGHT		32	// This controller should also support 64 (untested)
#endif
#ifndef GDISP_SCREEN_WIDTH
	#define GDISP_SCREEN_WIDTH		128
#endif
#ifndef GDISP_INITIAL_CONTRAST
	#define GDISP_INITIAL_CONTRAST	100
#endif
#ifndef GDISP_INITIAL_BACKLIGHT
	#define GDISP_INITIAL_BACKLIGHT	100
#endif
#ifdef CH1115_PAGE_PREFIX
	#define CH1115_PAGE_WIDTH      (GDISP_SCREEN_WIDTH+1)
	#define CH1115_PAGE_OFFSET     1
#else
	#define CH1115_PAGE_WIDTH      GDISP_SCREEN_WIDTH
	#define CH1115_PAGE_OFFSET     0
#endif

#define GDISP_FLG_NEEDFLUSH			(GDISP_FLG_DRIVER<<0)

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

// Some common routines and macros
#define RAM(g)						((gU8 *)g->priv)
#define xyaddr(x, y)        		(CH1115_PAGE_OFFSET + (x) + ((y)>>3)*CH1115_PAGE_WIDTH)
#define xybit(y)            		(1<<((y)&7))

#define write_cmd_1(g, a)			{ gU8 cmd[1]; cmd[0] = a; write_cmd(g, cmd, 1); }
#define write_cmd_2(g, a, b)		{ gU8 cmd[2]; cmd[0] = a; cmd[1] = b; write_cmd(g, cmd, 2); }

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * As this controller can't update on a pixel boundary we need to maintain the
 * the entire display surface in memory so that we can do the necessary bit
 * operations. Fortunately it is a small monochrome display.
 * 64 * 128 / 8 = 1024 bytes.
 */

LLDSPEC gBool gdisp_lld_init(GDisplay *g)
{
	// The private area is the display surface.
	g->priv = gfxAlloc(GDISP_SCREEN_HEIGHT/8 * CH1115_PAGE_WIDTH);
	if (!g->priv)
		return gFalse;

	// Fill in the prefix command byte on each page line of the display buffer
	// We can do this during initialisation as we're being careful that this byte is never overwritten.
	#ifdef CH1115_PAGE_PREFIX
		for (unsigned i = 0; i < GDISP_SCREEN_HEIGHT/8 * CH1115_PAGE_WIDTH; i += CH1115_PAGE_WIDTH)
			RAM(g)[i] = CH1115_PAGE_PREFIX;
	#endif

	// Initialise the board interface
	init_board(g);

	// Hardware reset
	setpin_reset(g, gTrue);
	gfxSleepMilliseconds(20);
	setpin_reset(g, gFalse);
	gfxSleepMilliseconds(200);

	acquire_bus(g);

	// Configuration
    // This might require display module vendor specific changes
	{
		// Display off
		write_cmd_1(g, 0xAE);

		// Clock divider
		write_cmd_2(g, 0xD5, 0x80);

		// Multiplex ratio
		write_cmd_2(g, 0xA8, 0x1F);

		// Display offset
		write_cmd_2(g, 0xD3, 0x30);

		// Display start line
		write_cmd_1(g, 0x40);

		// Charge pump
		write_cmd_2(g, 0x8D, 0x72);		// 0x10 if Vcc externally supplied

		// Segment re-map
		write_cmd_1(g, 0xA1);

		// COM output scan direction
		write_cmd_1(g, 0xC0);

		// COM pin hardware configuration
		write_cmd_2(g, 0xDA, 0x10);

		// Set internal/external current reference
		write_cmd_2(g, 0xAD, 0x50);

		// Set contract
		write_cmd_2(g, 0x81, 0x17);

		// Set pre-charge period
		write_cmd_2(g, 0xD9, 0xF1);

		// Set VCOMH select level
		write_cmd_2(g, 0xDB, 0x30);

		// Set entire display on/off
		write_cmd_1(g, 0xA4);

		// Set normal/inverse display
		write_cmd_1(g, 0xA6);

		// Page addressing mode 
		write_cmd_2(g, 0x20, 0x02);

		// Display on
		write_cmd_1(g, 0xAF);
	}

	release_bus(g);

    // Finish Init
    post_init_board(g);

	// Initialise the GDISP structure
	g->g.Width = GDISP_SCREEN_WIDTH;
	g->g.Height = GDISP_SCREEN_HEIGHT;
	g->g.Orientation = gOrientation0;
	g->g.Powermode = gPowerOn;
	g->g.Backlight = GDISP_INITIAL_BACKLIGHT;
	g->g.Contrast = GDISP_INITIAL_CONTRAST;

	return gTrue;
}

#if GDISP_HARDWARE_FLUSH
	LLDSPEC void gdisp_lld_flush(GDisplay *g)
    {
		gU8 * ram;
		unsigned pages;

		// Only flush if necessary
		if (!(g->flags & GDISP_FLG_NEEDFLUSH))
			return;

		ram = RAM(g);
		pages = GDISP_SCREEN_HEIGHT/8;

		acquire_bus(g);
		write_cmd_1(g, 0x40 | 0);
		while (pages--) {
			write_cmd_1(g, 0xB0 + (((GDISP_SCREEN_HEIGHT/8)-1)-pages));
			write_cmd_1(g, 0x00);
			write_cmd_1(g, 0x10);
			write_data(g, ram, CH1115_PAGE_WIDTH);
			ram += CH1115_PAGE_WIDTH;
		}
		release_bus(g);

		g->flags &= ~GDISP_FLG_NEEDFLUSH;
	}
#endif

#if GDISP_HARDWARE_DRAWPIXEL
	LLDSPEC void gdisp_lld_draw_pixel(GDisplay *g)
    {
		gCoord		x, y;

		switch(g->g.Orientation) {
		default:
		case gOrientation0:
			x = g->p.x;
			y = g->p.y;
			break;
		case gOrientation90:
			x = g->p.y;
			y = GDISP_SCREEN_HEIGHT-1 - g->p.x;
			break;
		case gOrientation180:
			x = GDISP_SCREEN_WIDTH-1 - g->p.x;
			y = GDISP_SCREEN_HEIGHT-1 - g->p.y;
			break;
		case gOrientation270:
			x = GDISP_SCREEN_WIDTH-1 - g->p.y;
			y = g->p.x;
			break;
		}
		if (gdispColor2Native(g->p.color) != gdispColor2Native(GFX_BLACK))
			RAM(g)[xyaddr(x, y)] |= xybit(y);
		else
			RAM(g)[xyaddr(x, y)] &= ~xybit(y);
		g->flags |= GDISP_FLG_NEEDFLUSH;
	}
#endif

#if GDISP_HARDWARE_PIXELREAD
    LLDSPEC gColor gdisp_lld_get_pixel_color(GDisplay *g)
    {
		gCoord		x, y;

		switch(g->g.Orientation) {
		default:
		case gOrientation0:
			x = g->p.x;
			y = g->p.y;
			break;
		case gOrientation90:
			x = g->p.y;
			y = GDISP_SCREEN_HEIGHT-1 - g->p.x;
			break;
		case gOrientation180:
			x = GDISP_SCREEN_WIDTH-1 - g->p.x;
			y = GDISP_SCREEN_HEIGHT-1 - g->p.y;
			break;
		case gOrientation270:
			x = GDISP_SCREEN_WIDTH-1 - g->p.y;
			y = g->p.x;
			break;
		}

		return (RAM(g)[xyaddr(x, y)] & xybit(y)) ? GFX_WHITE : GFX_BLACK;
	}
#endif

#if GDISP_HARDWARE_FILLS
    LLDSPEC void gdisp_lld_fill_area(GDisplay *g)
    {
		gCoord		sy, ey;
		gCoord		sx, ex;
		gCoord		col;
		unsigned	spage, zpages;
		gU8 *	base;
		gU8		mask;

		switch(g->g.Orientation) {
		default:
		case gOrientation0:
			sx = g->p.x;
			ex = g->p.x + g->p.cx - 1;
			sy = g->p.y;
			ey = sy + g->p.cy - 1;
			break;
		case gOrientation90:
			sx = g->p.y;
			ex = g->p.y + g->p.cy - 1;
			sy = GDISP_SCREEN_HEIGHT - g->p.x - g->p.cx;
			ey = GDISP_SCREEN_HEIGHT-1 - g->p.x;
			break;
		case gOrientation180:
			sx = GDISP_SCREEN_WIDTH - g->p.x - g->p.cx;
			ex = GDISP_SCREEN_WIDTH-1 - g->p.x;
			sy = GDISP_SCREEN_HEIGHT - g->p.y - g->p.cy;
			ey = GDISP_SCREEN_HEIGHT-1 - g->p.y;
			break;
		case gOrientation270:
			sx = GDISP_SCREEN_WIDTH - g->p.y - g->p.cy;
			ex = GDISP_SCREEN_WIDTH-1 - g->p.y;
			sy = g->p.x;
			ey = g->p.x + g->p.cx - 1;
			break;
		}

		spage = sy / 8;
		base = RAM(g) + CH1115_PAGE_OFFSET + CH1115_PAGE_WIDTH * spage;
		mask = 0xff << (sy&7);
		zpages = (ey / 8) - spage;

		if (gdispColor2Native(g->p.color) == gdispColor2Native(GFX_BLACK)) {
			while (zpages--) {
				for (col = sx; col <= ex; col++)
					base[col] &= ~mask;
				mask = 0xff;
				base += CH1115_PAGE_WIDTH;
			}
			mask &= (0xff >> (7 - (ey&7)));
			for (col = sx; col <= ex; col++)
				base[col] &= ~mask;
		}
        else {
			while (zpages--) {
				for (col = sx; col <= ex; col++)
					base[col] |= mask;
				mask = 0xff;
				base += CH1115_PAGE_WIDTH;
			}
			mask &= (0xff >> (7 - (ey&7)));
			for (col = sx; col <= ex; col++)
				base[col] |= mask;
		}
		g->flags |= GDISP_FLG_NEEDFLUSH;
	}
#endif

#if GDISP_NEED_CONTROL && GDISP_HARDWARE_CONTROL
    LLDSPEC void gdisp_lld_control(GDisplay *g)
    {
		switch(g->p.x) {
		case GDISP_CONTROL_POWER:
			if (g->g.Powermode == (gPowermode)g->p.ptr)
				return;
			switch((gPowermode)g->p.ptr) {
			case gPowerOff:
			case gPowerSleep:
			case gPowerDeepSleep:
				acquire_bus(g);
				write_cmd_1(g, 0xAE);
				release_bus(g);
				break;
			case gPowerOn:
				acquire_bus(g);
				write_cmd_1(g, 0xAF);
				release_bus(g);
				break;
			default:
				return;
			}
			g->g.Powermode = (gPowermode)g->p.ptr;
			return;

		case GDISP_CONTROL_ORIENTATION:
			if (g->g.Orientation == (gOrientation)g->p.ptr)
				return;

			switch((gOrientation)g->p.ptr) {
			// Rotation is handled by the drawing routines
			case gOrientation0:
			case gOrientation180:
				g->g.Height = GDISP_SCREEN_HEIGHT;
				g->g.Width = GDISP_SCREEN_WIDTH;
				break;
			case gOrientation90:
			case gOrientation270:
				g->g.Height = GDISP_SCREEN_WIDTH;
				g->g.Width = GDISP_SCREEN_HEIGHT;
				break;
			default:
				return;
			}

			g->g.Orientation = (gOrientation)g->p.ptr;
			return;

		case GDISP_CONTROL_CONTRAST:
            if ((unsigned)g->p.ptr > 100)
            	g->p.ptr = (void *)100;
			acquire_bus(g);
			write_cmd_2(g, 0x81, (((unsigned)g->p.ptr)<<8)/101);
			release_bus(g);
            g->g.Contrast = (unsigned)g->p.ptr;
			return;

		// Our own special controller code to inverse the display
		// 0 = normal, 1 = inverse
		case GDISP_CONTROL_INVERSE:
			acquire_bus(g);
			write_cmd_1(g, g->p.ptr ? 0xA7: 0xA6);
			release_bus(g);
			return;
		}
	}
#endif // GDISP_NEED_CONTROL

#endif // GFX_USE_GDISP

