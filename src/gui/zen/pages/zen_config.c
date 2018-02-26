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

#include "zen_config.h"
#include "zen_config_date.h"
#include "zen_config_time.h"
#include "pages/zen_main_one.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageConfig;

/* BUTTONS */
GHandle ghBtn_Date;
GHandle ghBtn_Time;
GHandle ghBtn_BackConfig;

/* IMAGES */
gdispImage ic_date;
gdispImage ic_time;


void create_PageConfig(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageConfig
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
	ghContainer_PageConfig = gwinContainerCreate(0, &wi, 0);

	
	// create button widget: ghBtn_Date
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageConfig;
	wi.text = MENU_TITLE_DATE;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_date;
	wi.customStyle = &color_one;
	ghBtn_Date = gwinButtonCreate(0, &wi);

	
	// create button widget: ghBtn_Time
	wi.g.show = TRUE;
	wi.g.x = 80;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageConfig;
	wi.text = MENU_TITLE_TIME;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_time;
	wi.customStyle = &color_two;
	ghBtn_Time = gwinButtonCreate(0, &wi);


	
	// create button widget: ghBtn_BackClean
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageConfig;
	wi.text = MENU_TITLE_BACK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_back;
	wi.customStyle = &color_three;
	ghBtn_BackConfig = gwinButtonCreate(0, &wi);
	
	

}


static void guiConfigMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

}

static void guiConfigMenu_onClose(GUIWindow *win) {

	(void) win;

}

static int guiConfigMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_Date) {

            	guiWindow_Show(&winDateConfigMenu);
                //fprintf(stderr, "Date\n");

            } else if(peb->gwin == ghBtn_Time) {

				guiWindow_Show(&winTimeConfigMenu);
                //fprintf(stderr, "Time\n");
            
            } else if(peb->gwin == ghBtn_BackConfig) {
            	guiWindow_Show(&winMainMenuOne);

            } else {
            	return 0;
            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winConfigMenu = {

/* Title   */	 "Configuraton Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiConfigMenu_onShow,
/* onClose */    guiConfigMenu_onClose,
/* onEvent */    guiConfigMenu_handleEvent,
/* handle  */    0

};