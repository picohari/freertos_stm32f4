/* ============================================================================
 * File: gwin_fastlabel.h
 * Description: Custom µGFX label widget with flicker-free streaming rendering
 * ============================================================================ */

 #ifndef GWIN_FASTLABEL_H
 #define GWIN_FASTLABEL_H
 
 #include "gfx.h"
 #include <stdbool.h>
 
 #ifdef __cplusplus
 extern "C" {
 #endif
 
 /* Custom widget type */
 typedef struct GFastLabelObject {
     GWidgetObject w;        // Base widget
     const char *text;       // Label text (allocated or static)
     justify_t justify;      // Text justification
     bool useAlloc;          // Whether text is allocated
 } GFastLabelObject;
 
 typedef GFastLabelObject *GHandle_FastLabel;
 
 /**
  * @brief Create a fast label widget with flicker-free rendering
  * 
  * @param g Display to use (use NULL for default)
  * @param widget Pointer to GFastLabelObject structure
  * @param pInit Widget initialization structure
  * @return GHandle Handle to the created widget
  */
 GHandle gwinGFastLabelCreate(GDisplay *g, GFastLabelObject *widget, 
                              GWidgetInit *pInit);
 
 /**
  * @brief Set text for fast label
  * 
  * @param gh Handle to fast label widget
  * @param text Text to display
  * @param useAlloc If TRUE, allocates memory and copies text. If FALSE, uses pointer directly
  */
 void gwinFastLabelSetText(GHandle gh, const char *text, gBool useAlloc);
 
 /**
  * @brief Set text justification
  * 
  * @param gh Handle to fast label widget
  * @param justify Justification (justifyLeft, justifyCenter, justifyRight)
  */
 void gwinFastLabelSetJustify(GHandle gh, justify_t justify);
 
 /**
 * @brief Set font for fast label
 *
 * @param gh Handle to fast label widget
 * @param font Font to be used
 */
void gwinFastLabelSetFont(GHandle gh, font_t font) {
    GWidgetObject *gw = (GWidgetObject *)gh;


 #ifdef __cplusplus
 }
 #endif
 
 #endif /* GWIN_FASTLABEL_H */