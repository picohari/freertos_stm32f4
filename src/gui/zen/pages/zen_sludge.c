#include <stdio.h>

#include "gfx.h"
#include "gui.h"

#ifdef UGFXSIMULATOR

	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	//#include <linux/input.h>

#else

	#include "stm32f4xx_hal.h"
	#include "cmsis_os.h"
	#include "XCore407I.h"

#endif

#include "skin/zenstyle.h"
#include "gui_router.h"

#include "pages/zen_sludge.h"
#include "pages/zen_main_two.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageSludge;


/* BUTTONS */
GHandle ghBtn_BackSludge;

/* IMAGES */


void create_PageSludge(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageSludge
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 24;
	wi.g.width = 320;
	wi.g.height = 192;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainer_PageSludge = gwinContainerCreate(0, &wi, 0);

	
	// create button widget: ghBtn_BackSludge
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageSludge;
	wi.text = MENU_TITLE_BACK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_back;
	wi.customStyle = &color_three;
	ghBtn_BackSludge = gwinButtonCreate(0, &wi);
	
}


static void guiSludgeMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

}

static void guiSludgeMenu_onClose(GUIWindow *win) {

	(void) win;

}

static int guiSludgeMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_BackSludge) {
            	guiWindow_Show(&winMainMenuTwo);

            } else {
            	return 0;
            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winSludgeMenu = {

/* Title   */	 "Sludge Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiSludgeMenu_onShow,
/* onClose */    guiSludgeMenu_onClose,
/* onEvent */    guiSludgeMenu_handleEvent,
/* handle  */    0

};