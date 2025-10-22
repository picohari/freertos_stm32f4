/* ============================================================================
 * File: fast_numbers.c
 * Description: Implementation using built-in 5x7 monospace font data
 * ============================================================================ */

#include "gfx.h"
#include "fast_numbers.h"
#include <stdio.h>
#include <stdbool.h>

/* Simple 5x7 monospace font for digits 0-9, '.', '-', ' ' */
/* Format: Each digit is 7 rows of 5 bits */
static const uint8_t font_5x7[13][7] = {
    // '0'
    {0x0E, 0x11, 0x11, 0x11, 0x11, 0x11, 0x0E},
    // '1'  
    {0x04, 0x0C, 0x04, 0x04, 0x04, 0x04, 0x0E},
    // '2'
    {0x0E, 0x11, 0x01, 0x02, 0x04, 0x08, 0x1F},
    // '3'
    {0x0E, 0x11, 0x01, 0x06, 0x01, 0x11, 0x0E},
    // '4'
    {0x02, 0x06, 0x0A, 0x12, 0x1F, 0x02, 0x02},
    // '5'
    {0x1F, 0x10, 0x1E, 0x01, 0x01, 0x11, 0x0E},
    // '6'
    {0x06, 0x08, 0x10, 0x1E, 0x11, 0x11, 0x0E},
    // '7'
    {0x1F, 0x01, 0x02, 0x04, 0x08, 0x08, 0x08},
    // '8'
    {0x0E, 0x11, 0x11, 0x0E, 0x11, 0x11, 0x0E},
    // '9'
    {0x0E, 0x11, 0x11, 0x0F, 0x01, 0x02, 0x0C},
    // '.' (decimal point)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C},
    // '-' (minus sign)
    {0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00},
    // ' ' (space)
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};

/* Font scaling - default 2x (10x14 effective size) */
static uint8_t font_scale = 2;

/* Character spacing in pixels (added to base width) */
static uint8_t char_spacing = 2;

#define CHAR_WIDTH_BASE  5
#define CHAR_HEIGHT_BASE 7

/**
 * @brief Set font scale
 */
void FastNumbers_SetFontSize(uint8_t size) {
    font_scale = (size == 0) ? 1 : (size == 1) ? 2 : 3;
}

/**
 * @brief Set character spacing
 */
void FastNumbers_SetSpacing(uint8_t spacing) {
    char_spacing = spacing;
}

/**
 * @brief Get character index in font table
 */
static inline int8_t get_char_index(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c == '.') return 10;
    if (c == '-') return 11;
    return 12;  // Space
}

/**
 * @brief Stream a single character at position with spacing
 */
static void stream_char(coord_t x, coord_t y, char c, color_t text_color, color_t bg_color) {
    int8_t idx = get_char_index(c);
    const uint8_t *glyph = font_5x7[idx];
    
    coord_t char_width = CHAR_WIDTH_BASE * font_scale;
    coord_t char_height = CHAR_HEIGHT_BASE * font_scale;
    coord_t total_width = char_width + char_spacing;  // Include spacing
    
    /* Start streaming this character INCLUDING spacing */
    gdispStreamStart(x, y, total_width, char_height);
    
    /* Stream each pixel with scaling */
    for (coord_t row = 0; row < char_height; row++) {
        uint8_t glyph_row = glyph[row / font_scale];
        
        /* Draw character pixels */
        for (coord_t col = 0; col < char_width; col++) {
            uint8_t bit_pos = 4 - (col / font_scale);  // MSB first
            bool is_pixel_set = (glyph_row >> bit_pos) & 0x01;
            
            color_t color = is_pixel_set ? text_color : bg_color;
            gdispStreamColor(color);
        }
        
        /* Add spacing pixels (background) */
        for (coord_t col = 0; col < char_spacing; col++) {
            gdispStreamColor(bg_color);
        }
    }
    
    gdispStreamStop();
}

/**
 * @brief Draw float value right-aligned
 */
void FastNumbers_DrawFloat(coord_t x, coord_t y, float value, uint8_t decimals,
                          uint8_t field_width, color_t text_color, color_t bg_color) {
    char buffer[16];
    
    /* Format number */
    if (decimals == 0) {
        snprintf(buffer, sizeof(buffer), "%*.0f", field_width, value);
    } else if (decimals == 1) {
        snprintf(buffer, sizeof(buffer), "%*.1f", field_width, value);
    } else if (decimals == 2) {
        snprintf(buffer, sizeof(buffer), "%*.2f", field_width, value);
    } else {
        snprintf(buffer, sizeof(buffer), "%*.3f", field_width, value);
    }
    
    /* Ensure null termination and limit length */
    buffer[field_width] = '\0';
    
    /* Calculate character dimensions including spacing */
    coord_t total_char_width = (CHAR_WIDTH_BASE * font_scale) + char_spacing;
    
    /* Draw characters right-to-left */
    coord_t char_x = x - total_char_width;  // Start from right edge
    
    for (int8_t i = field_width - 1; i >= 0; i--) {
        if (buffer[i] == '\0') break;
        
        stream_char(char_x, y, buffer[i], text_color, bg_color);
        char_x -= total_char_width;
    }
}

/**
 * @brief Draw integer right-aligned
 */
void FastNumbers_DrawInt(coord_t x, coord_t y, int32_t value, uint8_t field_width,
                        color_t text_color, color_t bg_color) {
    char buffer[16];
    
    /* Format number */
    snprintf(buffer, sizeof(buffer), "%*d", field_width, (int)value);
    buffer[field_width] = '\0';
    
    /* Calculate character dimensions including spacing */
    coord_t total_char_width = (CHAR_WIDTH_BASE * font_scale) + char_spacing;
    
    /* Draw characters right-to-left */
    coord_t char_x = x - total_char_width;
    
    for (int8_t i = field_width - 1; i >= 0; i--) {
        if (buffer[i] == '\0') break;
        
        stream_char(char_x, y, buffer[i], text_color, bg_color);
        char_x -= total_char_width;
    }
}