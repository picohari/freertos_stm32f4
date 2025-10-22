/* ============================================================================
 * File: rendering.h
 * Description: Flicker-free text rendering using GDISP streaming API
 *              with custom mcufont pixel callback
 * ============================================================================ */

 #ifndef RENDERING_H
 #define RENDERING_H
 
 #include "gfx.h"
 #include "mf_justify.h"
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /**
  * @brief Draw text in a box with flicker-free streaming rendering
  * @note Uses GDISP streaming to write background and text pixels row-by-row
  *       in a single continuous stream, eliminating flicker on FSMC displays
  * 
  * @param x X position of box
  * @param y Y position of box
  * @param cx Width of box
  * @param cy Height of box
  * @param str String to draw
  * @param font Font to use
  * @param text_color Text color
  * @param bg_color Background color
  * @param justify Text justification
  */
 void DrawStringBoxFlickerFree(coord_t x, coord_t y, coord_t cx, coord_t cy,
                               const char *str, font_t font, 
                               color_t text_color, color_t bg_color, 
                               justify_t justify);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* RENDERING_H */