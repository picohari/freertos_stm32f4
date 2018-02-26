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

#include "zen_menu.h"
#include "skin/zenstyle.h"
#include "gui_router.h"

#include "pages/zen_aeration.h"
#include "pages/zen_aeration_air_on.h"
#include "pages/zen_aeration_air_off.h"
#include "pages/zen_aeration_a1.h"
#include "pages/zen_aeration_a2.h"
#include "pages/zen_main_two.h"


/* PAGE CONTAINER */
GHandle ghBtn_AerationAirOn;
GHandle ghBtn_AerationAirOff;
GHandle ghBtn_AerationA1;
GHandle ghBtn_AerationA2;
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

	// create button widget: ghBtn_AerationAirOn
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageAeration;
	wi.text = MENU_TITLE_AIR_ON;
	wi.customStyle = &color_one;
	ghBtn_AerationAirOn = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_AerationAirOff
	wi.g.show = TRUE;
	wi.g.x = 80;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageAeration;
	wi.text = MENU_TITLE_AIR_OFF;
	wi.customStyle = &color_two;
	ghBtn_AerationAirOff = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_AerationA1
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageAeration;
	wi.text = MENU_TITLE_A1;
	wi.customStyle = &color_five;
	ghBtn_AerationA1 = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_AerationA2
	wi.g.show = TRUE;
	wi.g.x = 240;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageAeration;
	wi.text = MENU_TITLE_A2;
	wi.customStyle = &color_four;
	ghBtn_AerationA2 = gwinButtonCreate(0, &wi);


	// create button widget: ghBtn_BackAeration
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 96;
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

            } else if(peb->gwin == ghBtn_AerationAirOn) {
            	guiWindow_Show(&winAerationAirOnMenu);

            } else if(peb->gwin == ghBtn_AerationAirOff) {
            	guiWindow_Show(&winAerationAirOffMenu);

            } else if(peb->gwin == ghBtn_AerationA1) {
            	guiWindow_Show(&winAerationA1Menu);
            	
            } else if(peb->gwin == ghBtn_AerationA2) {
            	guiWindow_Show(&winAerationA2Menu);		

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