/* ============================================================================
 * File: gdisp_update.c
 * Description: Implementation of direct GDISP drawing with change detection
 * ============================================================================ */

 #include "gui.h"
 #include "gui_update.h"
 #include "rendering.h"
 #include <stdio.h>
 #include <string.h>
 
 /* Forward declaration of external window */
 extern GUIWindow winMainHome;
 
 /* Field indices for easy reference */
 enum {
     FIELD_POS_X = 0,
     FIELD_POS_Y,
     FIELD_POS_Z,
     FIELD_FEED_OVERRIDE,
     FIELD_SPINDLE_SPEED,
     NUM_DISPLAY_FIELDS
 };
 
 /* Display field definitions */
 static DisplayField_t display_fields[NUM_DISPLAY_FIELDS] = {
     // X position - defaults, will be configured
     {30, 38, 120, 24, 0, White, Black, justifyRight, "%.3f", true},
     // Y position
     {30, 68, 120, 24, 0, White, Black, justifyRight, "%.3f", true},
     // Z position
     {30, 98, 120, 24, 0, White, Black, justifyRight, "%.3f", true},
     // Feed override
     {30, 128, 120, 24, 0, White, Black, justifyRight, "%.1f", true},
     // Spindle speed
     {30, 158, 120, 24, 0, White, Black, justifyRight, "%.1f", true},
 };
 
 /* Local unpacked copy of last displayed state to avoid packed struct warnings */
 static struct {
     float pos_x;
     float pos_y;
     float pos_z;
     float feed_override;
     float spindle_speed;
     uint8_t homed;
     uint8_t num_axes;
     uint32_t status_flags;
     uint32_t last_update_ms;
 } last_displayed_state = {0};
 
 /* Flag to track if we need initial draw */
 static bool needs_initial_draw = true;
 
 /**
  * @brief Internal function to draw a single float field if changed
  */
 static inline bool DrawFloatField(const DisplayField_t *field, float new_value, 
                                   float *last_value) {
     if (!field->enabled) {
         return false;
     }
     
     // Only redraw if value changed or initial draw needed
     if (new_value != *last_value || needs_initial_draw) {
         char buffer[16];
         snprintf(buffer, sizeof(buffer), field->format, new_value);
         
         // Single atomic operation: draw background + text
         DrawStringBoxFlickerFree(field->x, field->y, field->width, field->height,
                           buffer, field->font, field->text_color, 
                           field->bg_color, field->justify);
         
         *last_value = new_value;
         return true;
     }
     return false;
 }
 
 /**
  * @brief Initialize display fields for main home screen
  */
 void GDISP_InitMainHomeFields(font_t font, color_t text_color, color_t bg_color) {
     // Apply font and colors to all fields
     for (uint8_t i = 0; i < NUM_DISPLAY_FIELDS; i++) {
         display_fields[i].font = font;
         display_fields[i].text_color = text_color;
         display_fields[i].bg_color = bg_color;
     }
     
     // Reset last displayed state
     memset(&last_displayed_state, 0, sizeof(last_displayed_state));
     needs_initial_draw = true;
 }
 
 /**
  * @brief Manually configure a specific display field
  */
 void GDISP_ConfigureField(uint8_t field_index, coord_t x, coord_t y, 
                           coord_t width, coord_t height, font_t font,
                           color_t text_color, color_t bg_color, justify_t justify) {
     if (field_index >= NUM_DISPLAY_FIELDS) {
         return;
     }
     
     display_fields[field_index].x = x;
     display_fields[field_index].y = y;
     display_fields[field_index].width = width;
     display_fields[field_index].height = height;
     display_fields[field_index].font = font;
     display_fields[field_index].text_color = text_color;
     display_fields[field_index].bg_color = bg_color;
     display_fields[field_index].justify = justify;
 }
 
 /**
  * @brief Update machine state display on main home screen
  */
 void GDISP_UpdateMainHome(const MachineState_t *new_state, GUIWindow *current_window) {
     // Safety checks
     if (new_state == NULL || current_window == NULL) {
         return;
     }
     
     // Only update if we're on the main home screen
     if (current_window != &winMainHome) {
         return;
     }
     
     /* Copy values from packed struct to local unpacked variables to avoid warnings */
     float pos_x = new_state->pos_x;
     float pos_y = new_state->pos_y;
     float pos_z = new_state->pos_z;
     float feed_override = new_state->feed_override;
     float spindle_speed = new_state->spindle_speed;
     
     /* Update position fields using direct GDISP drawing */
     DrawFloatField(&display_fields[FIELD_POS_X], pos_x, &last_displayed_state.pos_x);
     DrawFloatField(&display_fields[FIELD_POS_Y], pos_y, &last_displayed_state.pos_y);
     DrawFloatField(&display_fields[FIELD_POS_Z], pos_z, &last_displayed_state.pos_z);
     
     /* Update feed and spindle */
     DrawFloatField(&display_fields[FIELD_FEED_OVERRIDE], feed_override, 
                    &last_displayed_state.feed_override);
     DrawFloatField(&display_fields[FIELD_SPINDLE_SPEED], spindle_speed, 
                    &last_displayed_state.spindle_speed);
     
     /* Update non-float fields if needed */
     if (new_state->homed != last_displayed_state.homed) {
         // TODO: Draw homed status indicator if needed
         last_displayed_state.homed = new_state->homed;
     }
     
     if (new_state->status_flags != last_displayed_state.status_flags) {
         // TODO: Draw status flags if needed
         last_displayed_state.status_flags = new_state->status_flags;
     }
     
     // Clear initial draw flag after first update
     if (needs_initial_draw) {
         needs_initial_draw = false;
     }
 }
 
 /**
  * @brief Force redraw all fields on main home screen
  */
 void GDISP_ForceRedrawMainHome(const MachineState_t *state) {
     if (state == NULL) {
         return;
     }
     
     // Set flag to force redraw
     needs_initial_draw = true;
     
     // Trigger update which will redraw everything
     GDISP_UpdateMainHome(state, &winMainHome);
 }