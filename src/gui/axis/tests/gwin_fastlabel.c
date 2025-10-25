/* ============================================================================
 * File: gwin_fastlabel.c
 * Description: Implementation of custom flicker-free label widget
 * ============================================================================ */

#include "gfx.h"
/* Include µGFX internal widget headers */
#include "lib/ugfx/src/gwin/gwin_class.h"
#include "gwin_fastlabel.h"
#include "mf_justify.h"
#include <string.h>
#include <stdio.h>
 

/* Widget class definition */
static const gwidgetVMT fastLabelVMT;

/* Memory optimization: Use CCM RAM if available */
#ifdef __CCM_RAM
    #define CCM_RAM __attribute__((section(".ccmram")))
#else
    #define CCM_RAM
#endif

/* Optimized buffer sizes */
#define MAX_CHAR_WIDTH  16   // Reduced from 24
#define MAX_CHAR_HEIGHT 30   // Reduced from 40

/* Character rendering buffer in CCM RAM */
CCM_RAM static color_t char_buffer[MAX_CHAR_HEIGHT][MAX_CHAR_WIDTH];

/* Rendering context */
typedef struct {
    coord_t char_x;         // Current character X position
    coord_t char_y;         // Character Y position  
    color_t text_color;     // Text color
    color_t bg_color;       // Background color
    const struct mf_font_s *font;
} render_context_t;

static render_context_t g_ctx;

/**
 * @brief mcufont pixel callback - marks pixels in character buffer
 */
static void pixel_callback(int16_t x, int16_t y, uint8_t count, 
                          uint8_t alpha, void *state) {
    (void)state;
    
    /* Bounds check */
    if (y < 0 || y >= MAX_CHAR_HEIGHT) {
        return;
    }
    
    if (x < 0) {
        if (x + count <= 0) return;
        count += x;
        x = 0;
    }
    
    if (x >= MAX_CHAR_WIDTH) {
        return;
    }
    
    if (x + count > MAX_CHAR_WIDTH) {
        count = MAX_CHAR_WIDTH - x;
    }
    
    /* Mark text pixels (bitmap fonts: alpha > 127 = opaque) */
    if (alpha > 127) {
        while (count > 0 && x < MAX_CHAR_WIDTH) {
            char_buffer[y][x] = g_ctx.text_color;
            x++;
            count--;
        }
    }
}

/**
 * @brief Render single character into char_buffer using µGFX's renderer
 * @note This avoids direct mcufont calls which might be causing crashes
 */
static coord_t render_char_to_buffer_safe(mf_char c, font_t font, 
                                         color_t text_color, color_t bg_color) {
    /* Get character dimensions from µGFX */
    coord_t char_width = gdispGetCharWidth(c, font);
    coord_t font_height = gdispGetFontMetric(font, fontHeight);
    
    /* Validate dimensions */
    if (char_width <= 0 || char_width > MAX_CHAR_WIDTH || 
        font_height <= 0 || font_height > MAX_CHAR_HEIGHT) {
        return 0;
    }
    
    /* Clear entire buffer with background */
    for (coord_t row = 0; row < MAX_CHAR_HEIGHT; row++) {
        for (coord_t col = 0; col < MAX_CHAR_WIDTH; col++) {
            char_buffer[row][col] = bg_color;
        }
    }
    
    /* Try using µGFX's internal rendering via gdispGDrawChar */
    /* We'll render to a temporary area and read back pixels */
    /* This is safer than direct mcufont calls */
    
    /* Actually, let's try mcufont but with the font passed correctly */
    const struct mf_font_s *mf_font = (const struct mf_font_s *)font;
    
    /* Setup context for callback */
    g_ctx.text_color = text_color;
    g_ctx.bg_color = bg_color;
    
    /* Try to render - if this crashes, we know it's the mcufont call */
    uint8_t width = mf_render_character(mf_font, 0, 0, c, pixel_callback, NULL);
    
    return (width > MAX_CHAR_WIDTH) ? MAX_CHAR_WIDTH : width;
}

/**
 * @brief SIMPLER VERSION: Just use gdispDrawChar directly without buffering
 */
static void draw_char_directly(coord_t x, coord_t y, mf_char c, font_t font,
                               color_t text_color, color_t bg_color,
                               coord_t char_width, coord_t font_height) {
    /* Fill background */
    gdispFillArea(x, y, char_width, font_height, bg_color);
    
    /* Draw character using µGFX - this should be safe */
    gdispDrawChar(x, y, c, font, text_color);
}

/**
 * @brief Stream character from buffer to display
 */
static inline void stream_char_buffer(coord_t x, coord_t y, coord_t width, 
                                     coord_t height) {
    /* Validate parameters before streaming */
    if (width <= 0 || height <= 0 || width > MAX_CHAR_WIDTH || height > MAX_CHAR_HEIGHT) {
        return;
    }
    
    /* Stream character row-by-row */
    for (coord_t row = 0; row < height; row++) {
        gdispStreamStart(x, y + row, width, 1);
        
        for (coord_t col = 0; col < width; col++) {
            gdispStreamColor(char_buffer[row][col]);
        }
        
        gdispStreamStop();
    }
}

/**
 * @brief Custom draw function - renders text with flicker-free streaming
 */
static void fastLabelDraw(GWidgetObject *gw, void *param) {
    (void)param;
    
    GFastLabelObject *fl = (GFastLabelObject *)gw;
    
    /* Safety checks */
    if (!gw || !fl) {
        return;
    }
    
    /* Get widget properties */
    coord_t x = gw->g.x;
    coord_t y = gw->g.y;
    coord_t cx = gw->g.width;
    coord_t cy = gw->g.height;
    color_t bg_color = gw->pstyle->background;
    
    /* Skip if no text */
    if (!fl->text || !fl->text[0]) {
        /* Just fill with background */
        gdispGFillArea(gw->g.display, x, y, cx, cy, bg_color);
        return;
    }
    
    /* Get font - check if valid */
    font_t font = gw->g.font;
    if (!font) {
        /* No font, just fill background */
        gdispGFillArea(gw->g.display, x, y, cx, cy, bg_color);
        return;
    }
    
    color_t text_color = gw->pstyle->enabled.text;
    
    /* Calculate text dimensions */
    coord_t text_width = gdispGetStringWidth(fl->text, font);
    coord_t font_height = gdispGetFontMetric(font, fontHeight);
    
    /* Sanity check dimensions */
    if (font_height <= 0 || font_height > MAX_CHAR_HEIGHT) {
        gdispGFillArea(gw->g.display, x, y, cx, cy, bg_color);
        return;
    }
    
    /* Calculate text position based on justification */
    coord_t text_x = x;
    switch (fl->justify) {
        case justifyCenter:
            text_x = x + (cx - text_width) / 2;
            break;
        case justifyRight:
            text_x = x + cx - text_width;
            break;
        case justifyLeft:
        default:
            text_x = x;
            break;
    }
    
    /* Vertically center text */
    coord_t text_y = y + (cy - font_height) / 2;
    
    /* Setup rendering context */
    g_ctx.char_x = text_x;
    g_ctx.char_y = text_y;
    g_ctx.text_color = text_color;
    g_ctx.bg_color = bg_color;
    g_ctx.font = (const struct mf_font_s *)font;
    
    /* Validate font pointer */
    if (!g_ctx.font) {
        gdispGFillArea(gw->g.display, x, y, cx, cy, bg_color);
        return;
    }
    
    /* Fill background above text */
    coord_t text_start_row = text_y - y;
    if (text_start_row > 0) {
        gdispGFillArea(gw->g.display, x, y, cx, text_start_row, bg_color);
    }
    
    /* Render and stream each character */
    coord_t char_x = text_x;
    const char *p = fl->text;
    
    while (*p && char_x < x + cx) {
        /* Get character width */
        coord_t char_width = gdispGetCharWidth(*p, font);
        
        if (char_width <= 0) {
            p++;
            continue;
        }
        
        /* Clip if extends beyond widget */
        if (char_x + char_width > x + cx) {
            char_width = (x + cx) - char_x;
        }
        
        /* OPTION 1: Try buffered rendering with streaming (original approach) */
        #if 0
        coord_t rendered_width = render_char_to_buffer_safe(*p, font, text_color, bg_color);
        if (rendered_width > 0 && char_x >= x) {
            stream_char_buffer(char_x, text_y, rendered_width, font_height);
        }
        #endif
        
        /* OPTION 2: Direct rendering (simpler, should not crash) */
        if (char_width > 0 && char_x >= x) {
            draw_char_directly(char_x, text_y, *p, font, text_color, bg_color,
                             char_width, font_height);
        }
        
        char_x += char_width;
        p++;
    }
    
    /* Fill background to right of text */
    if (char_x < x + cx) {
        gdispGFillArea(gw->g.display, char_x, text_y, 
                      (x + cx) - char_x, font_height, bg_color);
    }
    
    /* Fill background below text */
    coord_t text_end_y = text_y + font_height;
    if (text_end_y < y + cy) {
        gdispGFillArea(gw->g.display, x, text_end_y, 
                      cx, (y + cy) - text_end_y, bg_color);
    }
}

/**
 * @brief Widget destruction - free allocated text
 */
static void fastLabelDestroy(GHandle gh) {
    GFastLabelObject *fl = (GFastLabelObject *)gh;
    
    if (fl->useAlloc && fl->text) {
        gfxFree((void *)fl->text);
        fl->text = NULL;
    }
}

/**
 * @brief Widget Virtual Method Table
 */
static const gwidgetVMT fastLabelVMT = {
    {
        "FastLabel",                // Class name
        sizeof(GFastLabelObject),   // Object size
        fastLabelDestroy,           // Destruction handler
        _gwidgetRedraw,             // Redraw handler
        0,                          // After clear (not used)
    },
    fastLabelDraw,                  // Custom draw function
	#if GINPUT_NEED_MOUSE
		{
			0,						// Process mose down events (NOT USED)
			0,						// Process mouse up events (NOT USED)
			0,						// Process mouse move events (NOT USED)
		},
	#endif
	#if GINPUT_NEED_KEYBOARD || GWIN_NEED_KEYBOARD
		{
			0						// Process keyboard key down events
		},
	#endif
	#if GINPUT_NEED_TOGGLE
		{
			0,						// No toggle role
			0,						// Assign Toggles (NOT USED)
			0,						// Get Toggles (NOT USED)
			0,						// Process toggle off event (NOT USED)
			0,						// Process toggle on event (NOT USED)
		},
	#endif
	#if GINPUT_NEED_DIAL
		{
			0,						// No dial roles
			0,						// Assign Dials (NOT USED)
			0, 						// Get Dials (NOT USED)
			0,						// Procees dial move events (NOT USED)
		},
	#endif
};

/**
 * @brief Create a fast label widget
 */
GHandle gwinGFastLabelCreate(GDisplay *g, GFastLabelObject *widget, 
                             GWidgetInit *pInit) {
    if (!(widget = (GFastLabelObject *)_gwidgetCreate(g, &widget->w, pInit, &fastLabelVMT))) {
        return NULL;
    }
    
    /* Initialize fast label specific fields */
    widget->text = pInit->text;
    widget->justify = justifyLeft;
    widget->useAlloc = gFalse;
    
    gwinSetVisible((GHandle)widget, pInit->g.show);
    
    return (GHandle)widget;
}

/**
 * @brief Set text for fast label
 */
void gwinFastLabelSetText(GHandle gh, const char *text, gBool useAlloc) {
    GFastLabelObject *fl = (GFastLabelObject *)gh;
    
    /* Free old text if allocated */
    if (fl->useAlloc && fl->text) {
        gfxFree((void *)fl->text);
    }
    
    /* Set new text */
    if (useAlloc && text) {
        size_t len = strlen(text) + 1;
        char *newtext = (char *)gfxAlloc(len);
        if (newtext) {
            memcpy(newtext, text, len);
            fl->text = newtext;
            fl->useAlloc = gTrue;
        } else {
            fl->text = text;
            fl->useAlloc = gFalse;
        }
    } else {
        fl->text = text;
        fl->useAlloc = gFalse;
    }
    
    /* Trigger redraw */
    _gwidgetRedraw(gh);
}

/**
 * @brief Set text justification
 */
void gwinFastLabelSetJustify(GHandle gh, justify_t justify) {
    GFastLabelObject *fl = (GFastLabelObject *)gh;
    fl->justify = justify;
    _gwidgetRedraw(gh);
}

/**
 * @brief Set font for fast label
 */
void gwinFastLabelSetFont(GHandle gh, font_t font) {
    GWidgetObject *gw = (GWidgetObject *)gh;
    
    if (!gw || !font) {
        return;
    }
    
    /* Set the font in the base widget structure */
    gw->g.font = font;
    
    /* Trigger redraw with new font */
    _gwidgetRedraw(gh);
}
