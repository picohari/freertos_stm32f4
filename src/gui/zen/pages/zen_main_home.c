
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
 
#include "pages/zen_main_home.h"
#include "pages/zen_main_one.h"
#include "pages/zen_main_two.h"



/* PAGE CONTAINER */
GHandle ghContainer_PageHome;


/* BUTTONS */


/* IMAGES */












void create_PageHome(void) {

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
	ghContainer_PageHome = gwinContainerCreate(0, &wi, 0);




}


static void guiMainHome_onShow(GUIWindow *win) {
	
	gui_set_title(win);

}


static void guiMainHome_onClose(GUIWindow *win) {

	(void) win;

}


static int guiMainHome_handleEvent(GUIWindow *win, GEvent *pe) {

    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton  *peb = (GEventGWinButton *)pe;

#if 0
            if (peb->gwin == ghBtn_T1T2)
                //guiWindow_Show (&winMainMenuOne);
            	fprintf(stderr, "T1/T2\n");
            else if (peb->gwin == ghButtonPlayer)
                guiWindow_Show (&winBrowseFile);

            else if (peb->gwin == ghButtonRecognize)
                guiWindow_Show (&winRecognizer);

            else if (peb->gwin == ghButtonWeather)
                guiWindow_Show (&winWeather);
            else if (peb->gwin == ghBtn_PageOne)
                guiWindow_Show(&winMainMenuOne);

            else
                return 0;
#endif

            return 1;
        }

        break;
    }

    return 0;
}





GUIWindow winMainHome = {

/* Title   */	 "Home",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiMainHome_onShow,
/* onClose */    guiMainHome_onClose,
/* onEvent */    guiMainHome_handleEvent,
/* handle  */    0

};




