/* ============================================================================
 * File: fast_text.h
 * Description: Ultra-fast streaming text renderer for real-time display
 *              8x16 monospace font with full ASCII support (32-127)
 * ============================================================================ */

 #ifndef FAST_TEXT_H
 #define FAST_TEXT_H
 
 #include "gfx.h"
 
 /**
  * @brief Draw text string at position (left-aligned)
  * @param x X position (left edge)
  * @param y Y position (top edge)
  * @param text String to display
  * @param text_color Color for text
  * @param bg_color Background color
  */
 void FastText_DrawString(coord_t x, coord_t y, const char *text,
                         color_t text_color, color_t bg_color);
 
 /**
  * @brief Draw text string right-aligned
  * @param x X position (right edge)
  * @param y Y position
  * @param text String to display
  * @param text_color Text color
  * @param bg_color Background color
  */
 void FastText_DrawStringRight(coord_t x, coord_t y, const char *text,
                              color_t text_color, color_t bg_color);
 
 /**
  * @brief Draw float value right-aligned
  * @param x X position (right edge)
  * @param y Y position
  * @param value Float value
  * @param decimals Decimal places (0-3)
  * @param field_width Total character width
  * @param text_color Text color
  * @param bg_color Background color
  */
 void FastText_DrawFloat(coord_t x, coord_t y, float value, uint8_t decimals,
                        uint8_t field_width, color_t text_color, color_t bg_color);
 
 /**
  * @brief Draw integer right-aligned
  * @param x X position (right edge)
  * @param y Y position  
  * @param value Integer value
  * @param field_width Character width
  * @param text_color Text color
  * @param bg_color Background color
  */
 void FastText_DrawInt(coord_t x, coord_t y, int32_t value, uint8_t field_width,
                      color_t text_color, color_t bg_color);
 
 /**
  * @brief Get character dimensions
  * @param width Pointer to store width
  * @param height Pointer to store height
  */
 void FastText_GetCharSize(coord_t *width, coord_t *height);
 
 /**
  * @brief Set font scale (1x, 2x, 3x)
  * @param scale Scale factor (1=8x16, 2=16x32, 3=24x48)
  */
 void FastText_SetScale(uint8_t scale);
 
 #endif /* FAST_TEXT_H */
