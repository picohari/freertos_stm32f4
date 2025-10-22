/* ============================================================================
 * File: display_update.h
 * Description: Simple display update using fast label widgets
 * ============================================================================ */

 #ifndef DISPLAY_UPDATE_H
 #define DISPLAY_UPDATE_H
 
 #include "gfx.h"
 #include "gui_router.h"
 #include "gwin_fastlabel.h"
 #include "linuxcnc_ctrl.h"
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /**
  * @brief Update machine state on display
  * @note Only updates if window is active. Uses fast labels for flicker-free rendering.
  * 
  * @param new_state Pointer to current machine state
  * @param current_window Pointer to currently active window
  * @param hX Fast label handle for X coordinate
  * @param hY Fast label handle for Y coordinate  
  * @param hZ Fast label handle for Z coordinate
  */
 void Display_UpdateMachineState(const MachineState_t *new_state,
                                GUIWindow *current_window,
                                GHandle hX, GHandle hY, GHandle hZ);
 
 #ifdef __cplusplus
 }
 #endif
 
 #endif /* DISPLAY_UPDATE_H */
 