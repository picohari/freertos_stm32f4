#include <stdio.h>

#include "gfx.h"
#include "gui.h"

#ifdef UGFXSIMULATOR

	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <linux/input.h>

#else

	#include "stm32f4xx_hal.h"
	#include "cmsis_os.h"
	#include "XCore407I.h"

#endif

#include "zen_menu.h"
#include "skin/zenstyle.h"
#include "gui_router.h"

#include "pages/zen_main_one.h"
#include "pages/zen_main_two.h"
#include "pages/zen_config.h"
#include "pages/zen_clean.h"
#include "pages/zen_timers.h"
#include "pages/zen_network.h"
#include "pages/zen_test_mode.h"
#include "pages/zen_status.h"
#include "pages/zen_help.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageOne;


/* BUTTONS */
GHandle ghBtn_Config;
GHandle ghBtn_Clean;
GHandle ghBtn_Timers;
GHandle ghBtn_Network;
GHandle ghBtn_Testmode;
GHandle ghBtn_Status;
GHandle ghBtn_Help;
GHandle ghBtn_PageTwo;


/* IMAGES */
//gdispImage ic_home;
gdispImage ic_settings;
gdispImage ic_local_drink;
gdispImage ic_alarm;
gdispImage ic_public;
gdispImage ic_heart_pulse;
gdispImage ic_search;
gdispImage ic_live_help;
gdispImage ic_forward;

//gdispImage ic_thumbs_up_down;



void create_PageOne(void) {

	GWidgetInit	wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageOne
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
	ghContainer_PageOne = gwinContainerCreate(0, &wi, 0);


	// create button widget: ghBtn_Config
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageOne;
	wi.text = MENU_TITLE_CONFIGURATION;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_settings;
	wi.customStyle = &color_one;
	ghBtn_Config = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_Config
	wi.g.show = TRUE;
	wi.g.x = 80;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageOne;
	wi.text = MENU_TITLE_CLEANING;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_local_drink;
	wi.customStyle = &color_two;
	ghBtn_Clean = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_Config
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageOne;
	wi.text = MENU_TITLE_TIMERS;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_alarm;
	wi.customStyle = &color_five;
	ghBtn_Timers = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_Config
	wi.g.show = TRUE;
	wi.g.x = 240;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageOne;
	wi.text = MENU_TITLE_NETWORK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_public;
	wi.customStyle = &color_four;
	ghBtn_Network = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_Config
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageOne;
	wi.text = MENU_TITLE_TESTMODE;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_heart_pulse;
	wi.customStyle = &color_six;
	ghBtn_Testmode = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_Config
	wi.g.show = TRUE;
	wi.g.x = 80;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageOne;
	wi.text = MENU_TITLE_STATUS;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_search;
	wi.customStyle = &color_eight;
	ghBtn_Status = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_Config
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageOne;
	wi.text = MENU_TITLE_HELP;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_live_help;
	wi.customStyle = &color_seven;
	ghBtn_Help = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_Config
	wi.g.show = TRUE;
	wi.g.x = 240;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageOne;
	wi.text = MENU_TITLE_NEXT;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_forward;
	wi.customStyle = &color_three;
	ghBtn_PageTwo = gwinButtonCreate(0, &wi);


}


static void guiMainMenuOne_onShow(GUIWindow *win) {

	gui_set_title(win);

}


static void guiMainMenuOne_onClose(GUIWindow *win) {

	(void) win;

}


static int guiMainMenuOne_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if (peb->gwin == ghBtn_Config) {
                //fprintf(stderr, "Config\n");
            	guiWindow_Show(&winConfigMenu);

            } else if(peb->gwin == ghBtn_Clean) {
            	guiWindow_Show(&winCleanMenu);
            
            } else if(peb->gwin == ghBtn_Timers) {
            	guiWindow_Show(&winTimersMenu);

            } else if(peb->gwin == ghBtn_Network) {
            	guiWindow_Show(&winNetworkMenu);

            } else if(peb->gwin == ghBtn_Testmode) {	
            	guiWindow_Show(&winTestModeMenu);

            } else if(peb->gwin == ghBtn_Status) {	
            	guiWindow_Show(&winStatusMenu);

            } else if(peb->gwin == ghBtn_Help) {	
            	guiWindow_Show(&winHelpMenu);	

#if 0
            else if (peb->gwin == ghButtonPlayer)
                guiWindow_Show (&winBrowseFile);

            else if (peb->gwin == ghButtonRecognize)
                guiWindow_Show (&winRecognizer);

            else if (peb->gwin == ghButtonWeather)
                guiWindow_Show (&winWeather);

#endif

            } else if (peb->gwin == ghBtn_PageTwo) {
                guiWindow_Show(&winMainMenuTwo);
            }

            else {
                return 0;
            }

            return 1;

        }

        break;
    }

    return 0;

}


GUIWindow winMainMenuOne = {

/* Title   */	 "Main Menu One",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiMainMenuOne_onShow,
/* onClose */    guiMainMenuOne_onClose,
/* onEvent */    guiMainMenuOne_handleEvent,
/* handle  */    0

};



