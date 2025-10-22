/* ============================================================================
 * File: gdisp_update.h
 * Description: Direct GDISP drawing for machine state with change detection
 * ============================================================================ */

 #ifndef GDISP_UPDATE_H
 #define GDISP_UPDATE_H
 
 #include "gfx.h"
 #include "linuxcnc_ctrl.h"  // Or wherever your MachineState_t is defined
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /**
  * @brief Definition for a display field using direct GDISP drawing
  */
 typedef struct {
     coord_t x;              // X position on screen
     coord_t y;              // Y position on screen
     coord_t width;          // Box width
     coord_t height;         // Box height
     font_t font;            // Font to use
     color_t text_color;     // Text color
     color_t bg_color;       // Background color
     justify_t justify;      // Text justification
     const char *format;     // printf format string
     bool enabled;           // Field is active
 } DisplayField_t;
 
 /**
  * @brief Initialize display fields for main home screen
  * @note Call this once during GUI initialization
  * 
  * Call example:
  * GDISP_InitMainHomeFields(
  *     font_DejaVuSans16,  // Font for all fields
  *     White,              // Text color
  *     Black               // Background color
  * );
  */
 void GDISP_InitMainHomeFields(font_t font, color_t text_color, color_t bg_color);
 
 /**
  * @brief Update machine state display on main home screen
  * @note Only updates if winMainHome is active and values have changed
  * 
  * @param new_state Pointer to current machine state structure
  * @param current_window Pointer to currently active GUIWindow
  */
 void GDISP_UpdateMainHome(const MachineState_t *new_state, GUIWindow *current_window);
 
 /**
  * @brief Force redraw all fields on main home screen
  * @note Useful after screen clear or window switch
  * 
  * @param state Pointer to current machine state structure
  */
 void GDISP_ForceRedrawMainHome(const MachineState_t *state);
 
 /**
  * @brief Manually configure a specific display field
  * @note Use this to customize individual field positions/appearance
  * 
  * @param field_index Index of field (0=X, 1=Y, 2=Z, 3=Feed, 4=Spindle)
  * @param x X position
  * @param y Y position
  * @param width Box width
  * @param height Box height
  * @param font Font to use
  * @param text_color Text color
  * @param bg_color Background color
  * @param justify Text justification (justifyLeft, justifyCenter, justifyRight)
  */
 void GDISP_ConfigureField(uint8_t field_index, coord_t x, coord_t y, 
                           coord_t width, coord_t height, font_t font,
                           color_t text_color, color_t bg_color, justify_t justify);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* GDISP_UPDATE_H */