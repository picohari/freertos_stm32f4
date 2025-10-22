/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.io/license.html
 */

#pragma once

/**
 * The command byte to put in front of each page line.
 *
 * If this is defined, each page line is prefixed with the specified value in the locally
 * maintained framebuffer.
 *
 * This can be omitted and handled inside of write_data() manually instead.
 */
#define CH1115_PAGE_PREFIX	0x40		// Co = 0, D/C = 1

static GFXINLINE void init_board(GDisplay *g)
{
	(void) g;
}

static GFXINLINE void post_init_board(GDisplay *g)
{
	(void) g;
}

static GFXINLINE void setpin_reset(GDisplay *g, gBool state)
{
	(void) g;
	(void) state;
}

static GFXINLINE void acquire_bus(GDisplay *g)
{
	(void) g;
}

static GFXINLINE void release_bus(GDisplay *g)
{
	(void) g;
}

static GFXINLINE void write_cmd(GDisplay *g, gU8 *data, gU16 length)
{
	(void) g;
	(void) data;
    (void) length;
}

static GFXINLINE void write_data(GDisplay *g, gU8 *data, gU16 length)
{
	(void) g;
	(void) data;
    (void) length;
}

