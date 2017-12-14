
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

#include "skin/zenstyle.h"
#include "gui_router.h"

 
#include "pages/zen_menu.h"
//#include "pages/zen_main_one.h"
//#include "pages/zen_main_two.h"



/* PAGE CONTAINER */
GHandle ghContainer_PageTwo;


/* BUTTONS */
GHandle ghBtn_T1T2;
GHandle ghBtn_Cyclemode;
GHandle ghBtn_Cycleparam;
GHandle ghBtn_Memnumber;
GHandle ghBtn_Aeration;
GHandle ghBtn_Overflow;
GHandle ghBtn_Suctionset;
GHandle ghBtn_PageOne;


/* IMAGES */
gdispImage ic_fan;
gdispImage ic_membrane;
gdispImage ic_unfill;
gdispImage ic_aligntop;
gdispImage ic_timelapse;











void create_PageTwo(void) {

	GWidgetInit		wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageTwo
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
	ghContainer_PageTwo = gwinContainerCreate(0, &wi, 0);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageTwo;
	wi.text = MENU_TITLE_T1_T2;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_timelapse;
	wi.customStyle = &color_one;
	ghBtn_T1T2 = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 80;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageTwo;
	wi.text = MENU_TITLE_CYCLEMODE;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_local_drink;
	wi.customStyle = &color_two;
	ghBtn_Cyclemode = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageTwo;
	wi.text = MENU_TITLE_CYCLEPARAM;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_alarm;
	wi.customStyle = &color_five;
	ghBtn_Cycleparam = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 240;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageTwo;
	wi.text = MENU_TITLE_MEMNUMBER;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_membrane;
	wi.customStyle = &color_four;
	ghBtn_Memnumber = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageTwo;
	wi.text = MENU_TITLE_AERATION;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_fan;
	wi.customStyle = &color_six;
	ghBtn_Aeration = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 80;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageTwo;
	wi.text = MENU_TITLE_OVERFLOW;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_aligntop;
	wi.customStyle = &color_eight;
	ghBtn_Overflow = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageTwo;
	wi.text = MENU_TITLE_SUCTIONSET;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_unfill;
	wi.customStyle = &color_seven;
	ghBtn_Suctionset = gwinButtonCreate(0, &wi);

	// create button widget: ghButton1
	wi.g.show = TRUE;
	wi.g.x = 240;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageTwo;
	wi.text = MENU_TITLE_BACK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_forward;
	wi.customStyle = &color_three;
	ghBtn_PageOne = gwinButtonCreate(0, &wi);


}


static void guiMainMenuTwo_onShow(GUIWindow *win) {
	
	gui_set_title(win);

}


static void guiMainMenuTwo_onClose(GUIWindow *win) {

	(void) win;

}


static int guiMainMenuTwo_handleEvent(GUIWindow *win, GEvent *pe) {

    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton  *peb = (GEventGWinButton *)pe;

            if (peb->gwin == ghBtn_T1T2)
                //guiWindow_Show (&winMainMenuOne);
            	fprintf(stderr, "T1/T2\n");
#if 0
            else if (peb->gwin == ghButtonPlayer)
                guiWindow_Show (&winBrowseFile);

            else if (peb->gwin == ghButtonRecognize)
                guiWindow_Show (&winRecognizer);

            else if (peb->gwin == ghButtonWeather)
                guiWindow_Show (&winWeather);
#endif
            else if (peb->gwin == ghBtn_PageOne)
                guiWindow_Show(&winMainHome);

            else
                return 0;

            return 1;
        }

        break;
    }

    return 0;
}





GUIWindow winMainMenuTwo = {

/* Title   */	 "Main Menu Two",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiMainMenuTwo_onShow,
/* onClose */    guiMainMenuTwo_onClose,
/* onEvent */    guiMainMenuTwo_handleEvent,
/* handle  */    0

};




