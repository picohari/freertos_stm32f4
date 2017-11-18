/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _GUI_ROUTER_H_
#define _GUI_ROUTER_H_

#include "gfx.h"




typedef struct _GUIWindow {
	const char 		*text;
    void (*onInit)  (struct _GUIWindow *win, GHandle handle);
    void (*onShow)  (struct _GUIWindow *win);
    void (*onClose) (struct _GUIWindow *win);
    int  (*onEvent) (struct _GUIWindow *win, GEvent *e);
    GHandle handle;
} GUIWindow;


extern GUIWindow *curWindow;


//void guiWindow_EventLoop(uint32_t timeout);
void guiWindow_Show   (GUIWindow *win);
void guiWindow_onInit (GUIWindow *win, GHandle handle);






#endif /* _GUI_ROUTER_H_ */