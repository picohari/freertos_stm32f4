/* ============================================================================
 * File: rendering.c
 * Description: Implementation using GDISP streaming for flicker-free rendering
 * ============================================================================ */

 #include "rendering.h"
 #include "mf_justify.h"
 #include <string.h>
 
 /* Maximum line buffer width - adjust to your display width */
 #define MAX_LINE_WIDTH 320
 
 /* Single row buffer for streaming */
 static color_t line_buffer[MAX_LINE_WIDTH];
 
 /* Maximum character width for single character rendering */
 #define MAX_CHAR_WIDTH 24
 #define MAX_CHAR_HEIGHT 40
 
 /* Small buffer for one character (much less RAM than full text!) */
 static color_t char_buffer[MAX_CHAR_HEIGHT][MAX_CHAR_WIDTH];
 
 /* Render context structure */
 typedef struct {
     coord_t box_x;              // Box origin X
     coord_t box_y;              // Box origin Y
     coord_t box_width;          // Box width
     coord_t box_height;         // Box height
     coord_t text_x;             // Text start X (after justification)
     coord_t text_y;             // Text start Y (vertically centered)
     coord_t current_y;          // Current absolute Y being rendered
     color_t text_color;         // Text color
     color_t bg_color;           // Background color
     const struct mf_font_s *font;  // mcufont font structure
 } render_context_t;
 
 static render_context_t g_ctx;
 
 /**
  * @brief mcufont pixel callback - marks pixels in character buffer
  */
 static void pixel_callback(int16_t x, int16_t y, uint8_t count, 
                           uint8_t alpha, void *state) {
     render_context_t *ctx = (render_context_t*)state;
     
     /* Check bounds for character buffer */
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
     
     /* Mark text pixels in character buffer */
     if (alpha > 127) {
         while (count > 0 && x < MAX_CHAR_WIDTH) {
             char_buffer[y][x] = ctx->text_color;
             x++;
             count--;
         }
     }
 }
 
 /**
  * @brief mcufont character callback - renders one character
  * @note Called by mf_render_aligned for each character in the string
  */
 static uint8_t character_callback(int16_t x, int16_t y, mf_char character, 
                                  void *state) {
     render_context_t *ctx = (render_context_t*)state;
     
     /* Render this character's glyph, which will call pixel_callback for each pixel run */
     return mf_render_character(ctx->font, x, y, character, pixel_callback, state);
 }
 
 /**
  * @brief Render single character into char_buffer
  */
 static coord_t render_char_to_buffer(mf_char c, const struct mf_font_s *font,
                                      render_context_t *ctx) {
     /* Clear char buffer with background */
     for (coord_t row = 0; row < MAX_CHAR_HEIGHT; row++) {
         for (coord_t col = 0; col < MAX_CHAR_WIDTH; col++) {
             char_buffer[row][col] = ctx->bg_color;
         }
     }
     
     /* Render single character at origin */
     uint8_t width = mf_render_character(font, 0, 0, c, pixel_callback, ctx);
     
     return width;
 }
 
 /**
  * @brief Stream single character from char_buffer to display
  */
 static inline void stream_char_buffer(coord_t x, coord_t y, coord_t width, 
                                      coord_t height) {
     /* Stream character row by row */
     for (coord_t row = 0; row < height; row++) {
         gdispStreamStart(x, y + row, width, 1);
         
         for (coord_t col = 0; col < width; col++) {
             gdispStreamColor(char_buffer[row][col]);
         }
         
         gdispStreamStop();
     }
 }
 
 /**
  * @brief Draw text in a box with flicker-free streaming rendering
  */
 void DrawStringBoxFlickerFree(coord_t x, coord_t y, coord_t cx, coord_t cy,
                               const char *str, font_t font, 
                               color_t text_color, color_t bg_color,
                               justify_t justify) {
     if (str == NULL || font == NULL || cx == 0 || cy == 0) {
         return;
     }
     
     /* Limit to buffer size */
     if (cx > MAX_LINE_WIDTH) {
         cx = MAX_LINE_WIDTH;
     }
     
     /* Calculate text dimensions */
     coord_t text_width = gdispGetStringWidth(str, font);
     coord_t font_height = gdispGetFontMetric(font, fontHeight);
     
     /* Calculate text X position based on justification */
     coord_t text_x = x;
     switch (justify) {
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
     
     /* Vertically center text in box */
     coord_t text_y = y + (cy - font_height) / 2;
     
     /* Setup render context */
     g_ctx.box_x = x;
     g_ctx.box_y = y;
     g_ctx.box_width = cx;
     g_ctx.box_height = cy;
     g_ctx.text_x = text_x;
     g_ctx.text_y = text_y;
     g_ctx.text_color = text_color;
     g_ctx.bg_color = bg_color;
     g_ctx.font = (const struct mf_font_s *)font;
     
     /* Fill background above text */
     coord_t text_start_row = text_y - y;
     if (text_start_row > 0) {
         gdispFillArea(x, y, cx, text_start_row, bg_color);
     }
     
     /* Render and stream each character individually */
     coord_t char_x = text_x;
     const char *p = str;
     
     while (*p && char_x < x + cx) {
         /* Render this character into char_buffer */
         coord_t char_width = render_char_to_buffer(*p, g_ctx.font, &g_ctx);
         
         /* Clip if character extends beyond box */
         if (char_x + char_width > x + cx) {
             char_width = (x + cx) - char_x;
         }
         
         /* Stream character directly to display */
         if (char_width > 0) {
             stream_char_buffer(char_x, text_y, char_width, font_height);
         }
         
         char_x += char_width;
         p++;
     }
     
     /* Fill background to the right of text */
     if (char_x < x + cx) {
         gdispFillArea(char_x, text_y, (x + cx) - char_x, font_height, bg_color);
     }
     
     /* Fill background below text */
     coord_t text_end_row = text_y + font_height;
     if (text_end_row < y + cy) {
         gdispFillArea(x, text_end_row, cx, (y + cy) - text_end_row, bg_color);
     }
 }