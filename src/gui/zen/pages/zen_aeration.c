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

#include "pages/zen_aeration.h"
#include "pages/zen_main_two.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageAeration;

/* BUTTONS */
GHandle ghBtn_BackAeration;

/* IMAGES */


void create_PageAeration(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageAeration
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
	ghContainer_PageAeration = gwinContainerCreate(0, &wi, 0);

	
	// create button widget: ghBtn_BackAeration
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageAeration;
	wi.text = MENU_TITLE_BACK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_back;
	wi.customStyle = &color_three;
	ghBtn_BackAeration = gwinButtonCreate(0, &wi);
	
}


static void guiAerationMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

}

static void guiAerationMenu_onClose(GUIWindow *win) {

	(void) win;

}

static int guiAerationMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_BackAeration) {
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



GUIWindow winAerationMenu = {

/* Title   */	 "Aeration Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiAerationMenu_onShow,
/* onClose */    guiAerationMenu_onClose,
/* onEvent */    guiAerationMenu_handleEvent,
/* handle  */    0

};