
#include <stdio.h>

#include "gfx.h"
#include "gui.h"

#ifdef UGFXSIMULATOR
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <linux/input.h>
#else
	#include "config.h"
	#include "task.h"
	#include "stm32f4xx_hal.h"
	#include "cmsis_os.h"
	#include "XCore407I.h"
	#include "gwin_fastlabel.h"
#endif

#include "style.h"

#include "ic_poweroff.h"

/* PAGE CONTAINER & WIDGETS */
GHandle ghContainer_PageHome;
GHandle ghContainer_Header;
GHandle ghContainer_Footer;

/* ICONS */
GHandle ghIcPowerOff;
GHandle ghIcPowerOn;
GHandle ghIcon_Connection;

/* LABELS (static) */
GHandle ghLabelStatus;

/* LABELS (dynamic) */
GHandle ghLabelAxis_X, ghLabelAxis_Y, ghLabelAxis_Z;

GHandle ghLabelG53_X;	// G53: current machine coordinates
GHandle ghLabelG53_Y;
GHandle ghLabelG53_Z;
GHandle ghLabelRotary;	

/* Declare fast label objects (can be global or in your window structure) */
static GFastLabelObject gFastLabelX;
static GFastLabelObject gFastLabelY;
static GFastLabelObject gFastLabelZ;

/* Handles */
GHandle ghFastLabelX;
GHandle ghFastLabelY;
GHandle ghFastLabelZ;


/* BUTTONS */
GHandle ghBtn_MENU;

/* IMAGES */


/* OTHER STUFF */
GTimer gtBlinker;



void create_PageHome(void) {

	GWidgetInit		wi;

	gwinWidgetClearInit(&wi);

	/* Header container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 320;
	wi.g.height = 18;
	wi.g.parent = 0;
	wi.text = "Header";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = &color_header;
	ghContainer_Header = gwinContainerCreate(0, &wi, 0);

	gwinSetDefaultFont(ctrld_16b);

	/* Status Label */
	wi.g.show = TRUE;
	wi.g.x = 2;
	wi.g.y = 1;
	wi.g.width = 320;
	wi.g.height = 16;
	wi.g.parent = ghContainer_Header;
	wi.text = "Idle";
	wi.customDraw = gwinLabelDrawJustifiedLeft;
	wi.customParam = 0;
	wi.customStyle = &color_header;
	ghLabelStatus = gwinLabelCreate(0, &wi);



	/* Connection Icon Widget */
	wi.g.show = TRUE;
	wi.g.x = 300;
	wi.g.y = 0;
	wi.g.width = 18;
	wi.g.height = 18;
	wi.g.parent = ghContainer_Header;
	wi.text = "";
	wi.customDraw = gwinLabelDrawJustifiedCenter;
	wi.customParam = 0;
	wi.customStyle = &color_header;
	ghIcon_Connection = gwinContainerCreate(0, &wi, 0);

	/* Draw Icon Power OFF */
	wi.g.show = FALSE;
	wi.g.x = 0; 
	wi.g.y = 0;
	wi.g.width = 18;
	wi.g.height = 18;
	wi.g.parent = ghIcon_Connection;
	ghIcPowerOff = gwinImageCreate(0, &wi.g);
	gwinImageOpenFile(ghIcPowerOff, "ic_power_off.bmp");

	/* Draw Icon Power ON*/
	wi.g.show = FALSE;
	wi.g.x = 0; 
	wi.g.y = 0;
	wi.g.width = 18;
	wi.g.height = 18;
	wi.g.parent = ghIcon_Connection;
	ghIcPowerOn = gwinImageCreate(0, &wi.g);
	gwinImageOpenFile(ghIcPowerOn, "ic_power_on.bmp");



	/* Display Coordinates */
	#if 0
	/* Base container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 20;
	wi.g.width = 320;
	wi.g.height = 200;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainer_PageHome = gwinContainerCreate(0, &wi, 0);

	gwinSetDefaultFont(neep_12x24b);

	/* X Label */
	wi.g.show = TRUE;
	wi.g.x = 4;
	wi.g.y = 20;
	wi.g.width = 25;
	wi.g.height = 26;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "X";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = &axislabels_unhomed;
	ghLabelAxis_X = gwinButtonCreate(0, &wi);
	
	/* Y Label */
	wi.g.show = TRUE;
	wi.g.x = 4;
	wi.g.y = 50;
	wi.g.width = 25;
	wi.g.height = 26;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "Y";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = &axislabels_homed;
	ghLabelAxis_Y = gwinButtonCreate(0, &wi);

	/* Z Label */
	wi.g.show = TRUE;
	wi.g.x = 4;
	wi.g.y = 80;
	wi.g.width = 25;
	wi.g.height = 26;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "Z";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = &axislabels_active;
	ghLabelAxis_Z = gwinButtonCreate(0, &wi);

	#endif
	
	#if 0
    // Initialize X coordinate label
    gwinWidgetClearInit(&wi);
	gwinSetDefaultFont(neep_12x24b);
	
    wi.g.show = gTrue;
    wi.g.x = 50;
    wi.g.y = 100;
    wi.g.width = 150;
    wi.g.height = 35;
    wi.text = "0.000";
    wi.customStyle = &color_white;  // Or use default
    ghFastLabelX = gwinGFastLabelCreate(0, &gFastLabelX, &wi);
    gwinFastLabelSetJustify(ghFastLabelX, justifyRight);
	
    // Similar for Y and Z...
    wi.g.y = 140;
    ghFastLabelY = gwinGFastLabelCreate(0, &gFastLabelY, &wi);
    gwinFastLabelSetJustify(ghFastLabelY, justifyRight);
    
    wi.g.y = 180;
    ghFastLabelZ = gwinGFastLabelCreate(0, &gFastLabelZ, &wi);
    gwinFastLabelSetJustify(ghFastLabelZ, justifyRight);

	#endif


	gwinSetDefaultFont(neep_12x24b);

	#if 0
		
	/* X Coordinates */
	wi.g.show = TRUE;
	wi.g.x = 30;
	wi.g.y = 20;
	wi.g.width = 120;
	wi.g.height = 24;
	wi.g.parent = 0;
	wi.text = "-1000.000";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelG53_X = gwinLabelCreate(0, &wi);

	/* Y Coordinates */
	wi.g.show = TRUE;
	wi.g.x = 30;
	wi.g.y = 50;
	wi.g.width = 120;
	wi.g.height = 24;
	wi.g.parent = 0;
	wi.text = "600.000";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelG53_Y = gwinLabelCreate(0, &wi);

	/* Y Coordinates */
	wi.g.show = TRUE;
	wi.g.x = 30;
	wi.g.y = 80;
	wi.g.width = 120;
	wi.g.height = 24;
	wi.g.parent = 0;
	wi.text = "80.000";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelG53_Z = gwinLabelCreate(0, &wi);
	
	#endif

	/* Rotary encoder test */
	wi.g.show = TRUE;
	wi.g.x = 30;
	wi.g.y = 150;
	wi.g.width = 120;
	wi.g.height = 24;
	wi.g.parent = 0;
	wi.text = "0";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelRotary = gwinLabelCreate(0, &wi);


}





static void blinkTimer_callback(void* arg)
{
    (void)arg;
 
    if (gwinGetVisible(ghIcPowerOn) == TRUE) {
		gwinHide(ghIcPowerOn);
	} else {
		gwinShow(ghIcPowerOn);
	}
}

static void guiMainHome_onShow(GUIWindow *win) {

	(void) win;

	gtimerInit(&gtBlinker);
	gtimerStart(&gtBlinker, blinkTimer_callback, NULL, TRUE, 500);
}


static void guiMainHome_onClose(GUIWindow *win) {

	(void) win;

}


static int guiMainHome_handleEvent(GUIWindow *win, GEvent *pe) {

    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton  *peb = (GEventGWinButton *)pe;

            if (peb->gwin == ghBtn_MENU) {
                //guiWindow_Show (&winMainMenuOne);
            }

            else
                return 0;

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




