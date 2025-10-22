/* ============================================================================
 * File: display_update.c
 * Description: Implementation of display update
 * ============================================================================ */

 #include "display_update.h"
 #include <stdio.h>
 #include <string.h>
 
 /* Forward declaration of main home window */
 extern GUIWindow winMainHome;
 
 /* Local buffer for formatting - keep small! */
 #define FORMAT_BUFFER_SIZE 16
 
 /**
  * @brief Update machine state display
  */
 void Display_UpdateMachineState(const MachineState_t *new_state,
                                GUIWindow *current_window,
                                GHandle hX, GHandle hY, GHandle hZ) {
     /* Safety checks */
     if (new_state == NULL || current_window == NULL) {
         return;
     }
     
     /* Only update if on main home screen */
     if (current_window != &winMainHome) {
         return;
     }
     
     /* Local buffer for formatting */
     char buffer[FORMAT_BUFFER_SIZE];
     
     /* Copy values from packed struct to avoid warnings */
     float pos_x = new_state->pos_x;
     float pos_y = new_state->pos_y;
     float pos_z = new_state->pos_z;
     
     /* Update X coordinate */
     if (hX) {
         snprintf(buffer, sizeof(buffer), "%.3f", pos_x);
         gwinFastLabelSetText(hX, buffer, gTrue);  // useAlloc = TRUE
     }
     
     /* Update Y coordinate */
     if (hY) {
         snprintf(buffer, sizeof(buffer), "%.3f", pos_y);
         gwinFastLabelSetText(hY, buffer, gTrue);
     }
     
     /* Update Z coordinate */
     if (hZ) {
         snprintf(buffer, sizeof(buffer), "%.3f", pos_z);
         gwinFastLabelSetText(hZ, buffer, gTrue);
     }

 }