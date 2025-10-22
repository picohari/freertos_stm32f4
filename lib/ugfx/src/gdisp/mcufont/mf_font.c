/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.io/license.html
 */

#ifndef MF_NO_COMPILE

#define MF_BWFONT_INTERNALS
#define MF_RLEFONT_INTERNALS

#include "mf_font.h"

#include "mf_bwfont.h"
#include "mf_rlefont.h"
#include "mf_bwfont.h"
#include "mf_scaledfont.h"

#include <stdbool.h>

/* This will be made into a list of included fonts using macro magic. */
#define MF_INCLUDED_FONTS 0

/* Included fonts begin here */
#include MF_FONT_FILE_NAME
/* Include fonts end here */

gU8 mf_render_character(const struct mf_font_s *font,
                            gI16 x0, gI16 y0,
                            mf_char character,
                            mf_pixel_callback_t callback,
                            void *state)
{
    gU8 width;
    width = font->render_character(font, x0, y0, character, callback, state);

    if (!width)
    {
        width = font->render_character(font, x0, y0, font->fallback_character,
                                       callback, state);
    }

    return width;
}

gU8 mf_character_width(const struct mf_font_s *font,
                           mf_char character)
{
    gU8 width;
    width = font->character_width(font, character);

    if (!width)
    {
        width = font->character_width(font, font->fallback_character);
    }

    return width;
}

struct whitespace_state
{
    gU8 min_x, min_y;
    gU8 max_x, max_y;
};

static void whitespace_callback(int16_t x, int16_t y, gU8 count,
                                gU8 alpha, void *state)
{
    struct whitespace_state *s = state;
    if (alpha > 7)
    {
        if (s->min_x > x) s->min_x = x;
        if (s->min_y > y) s->min_y = y;
        x += count - 1;
        if (s->max_x < x) s->max_x = x;
        if (s->max_y < y) s->max_y = y;
    }
}

MF_EXTERN void mf_character_whitespace(const struct mf_font_s *font,
                                       mf_char character,
                                       gU8 *left, gU8 *top,
                                       gU8 *right, gU8 *bottom)
{
    struct whitespace_state state = {255, 255, 0, 0};
    mf_render_character(font, 0, 0, character, whitespace_callback, &state);

    if (state.min_x == 255 && state.min_y == 255)
    {
        /* Character is whitespace */
        if (left) *left = font->width;
        if (top) *top = font->height;
        if (right) *right = 0;
        if (bottom) *bottom = 0;
    }
    else
    {
        if (left) *left = state.min_x;
        if (top) *top = state.min_y;
        if (right) *right = font->width - state.max_x - 1;
        if (bottom) *bottom = font->height - state.max_y - 1;
    }
}

/* Avoids a dependency on libc */
static bool strequals(const char *a, const char *b)
{
    while (*a)
    {
        if (*a++ != *b++)
            return false;
    }
    return (!*b);
}

const struct mf_font_s *mf_find_font(const char *name)
{
    const struct mf_font_list_s *f;
    f = MF_INCLUDED_FONTS;

    while (f)
    {
        if (strequals(f->font->full_name, name) ||
            strequals(f->font->short_name, name))
        {
            return f->font;
        }

        f = f->next;
    }

    return 0;
}

const struct mf_font_list_s *mf_get_font_list(void)
{
    return MF_INCLUDED_FONTS;
}

#endif //MF_NO_COMPILE

