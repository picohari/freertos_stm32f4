/* ============================================================================
 * File: fast_numbers.h
 * Description: Ultra-fast streaming number renderer for real-time display
 *              Monospace, right-aligned, streams background+text in one pass
 * ============================================================================ */

#ifndef FAST_NUMBERS_H
#define FAST_NUMBERS_H

#include "gfx.h"

/**
 * @brief Draw a floating point number with streaming (fast, flicker-free)
 * @param x X position (right edge of number field)
 * @param y Y position (top edge)
 * @param value Float value to display
 * @param decimals Number of decimal places (e.g., 3 for "123.456")
 * @param field_width Total character width including sign, digits, and decimal point
 * @param text_color Color for digits
 * @param bg_color Background color
 * 
 * Example: FastNumbers_DrawFloat(200, 50, 123.456, 3, 8, Green, Black)
 *          Displays "  123.456" right-aligned in 8-character field
 */
void FastNumbers_DrawFloat(coord_t x, coord_t y, float value, uint8_t decimals,
                          uint8_t field_width, color_t text_color, color_t bg_color);

/**
 * @brief Draw an integer with streaming
 * @param x X position (right edge)
 * @param y Y position
 * @param value Integer value
 * @param field_width Character width
 * @param text_color Text color
 * @param bg_color Background color
 */
void FastNumbers_DrawInt(coord_t x, coord_t y, int32_t value, uint8_t field_width,
                        color_t text_color, color_t bg_color);

/**
 * @brief Set font size (call once during init)
 * @param size Font size: 0=small (8x12), 1=medium (12x16), 2=large (16x24)
 */
void FastNumbers_SetFontSize(uint8_t size);

/**
 * @brief Set character spacing in pixels (default is 2)
 * @param spacing Extra pixels between characters (0-5 recommended)
 */
void FastNumbers_SetSpacing(uint8_t spacing);

#endif /* FAST_NUMBERS_H */
