#include <stdio.h>
#include <string.h>

#include "gfx.h"
#include "gui.h"
#include "src/gwin/gwin_keyboard_layout.h"

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

#include "zen_config_time.h"
#include "pages/zen_t1_t2_config_t1.h"
#include "pages/zen_t1_t2.h"
#include "helpers/zen_util.h"

/* PAGE CONTAINER */
GHandle ghContainer_PageT1T2ConfigT1;

/* BUTTONS */
GHandle ghBtn_CancelT1;
GHandle ghBtn_SetT1;

/* IMAGES */

/* EVENT LISTENER */
static GListener gl;

/* INPUT FIELDS AND KEYBOARD */
static GHandle ghTexteditHours;
static GHandle ghTexteditMinutes;
//static GHandle ghTexteditSeconds;
static GHandle ghKeyboard;
static GHandle ghLabel_ErrorTime;


//Keyboard layouts
static const GVSpecialKey KeyboardSpecialKeys[] = {
	{ "\001", 0, GVKEY_SINGLESET, 1 },				// \001 (1)	= Shift Lower to Upper
	{ "\001", 0, GVKEY_INVERT|GVKEY_LOCKSET, 2 },	// \002 (2)	= Shift Upper to Upper Lock
	{ "\002", 0, GVKEY_INVERT|GVKEY_LOCKSET, 0 },	// \003 (3)	= Shift Upper Lock to Lower
	{ "123", 0, GVKEY_LOCKSET, 3 },					// \004 (4)	= Change to Numbers
	{ "\010", "\b", 0, 0 }							// \005 (5)	= Backspace
};

//NumPad with digits only (with none of the above)
static const char *numpadKeySetArray[] = { "789", "456", "123", "0\005", 0 };
static const GVKeySet numpadKeySet[] = { numpadKeySetArray, 0 };
static const GVKeyTable numpadKeyboard = { KeyboardSpecialKeys, numpadKeySet };

void create_PageT1T2ConfigT1(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageT1T2ConfigT1
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
	ghContainer_PageT1T2ConfigT1 = gwinContainerCreate(0, &wi, 0);

	// HANDLE THE PRESS OF THE DELETE KEY WHEN NO TextEdit IS SELECTED
	// Create the keyboard
	wi.g.show = TRUE;
	wi.g.x = 0; 
	wi.g.y = 58;
	wi.g.width = gdispGetWidth() / 2 + 80; 
	wi.g.height = gdispGetHeight() / 2 + 22;
	wi.g.parent = ghContainer_PageT1T2ConfigT1;
	ghKeyboard = gwinKeyboardCreate(0, &wi);

	// TextEditHours
	wi.g.show = TRUE;
	wi.g.x = 105;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageT1T2ConfigT1;
	wi.text = "";
	ghTexteditHours = gwinTexteditCreate(0, &wi, 2);

	// TextEditMinutes
	wi.g.show = TRUE;
	wi.g.x = 165;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageT1T2ConfigT1;
	ghTexteditMinutes = gwinTexteditCreate(0, &wi, 2);

	// TextEditSeconds
	/*
	wi.g.show = TRUE;
	wi.g.x = 197;
	wi.g.y = 10;
	wi.g.width = 50;
	wi.g.height = 30;
	ghTexteditSeconds = gwinTexteditCreate(0, &wi, 2);
	*/
	
	// create button widget: ghBtn_SetT1
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 58;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageT1T2ConfigT1;
	//wi.text = MENU_TITLE_OK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_done;
	wi.customStyle = &color_eight;
	ghBtn_SetT1 = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_CancelT1
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageT1T2ConfigT1;
	//wi.text = MENU_TITLE_CANCEL;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_cancel;
	wi.customStyle = &color_five;
	ghBtn_CancelT1 = gwinButtonCreate(0, &wi);

	// create the Label widget: ghLabel_ErrorTime
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;
	wi.g.x = 105;
	wi.g.y = 35;
	wi.g.width = 160;
	wi.g.height = 20;
	wi.g.parent = ghContainer_PageT1T2ConfigT1;
	wi.text = "";
	ghLabel_ErrorTime = gwinLabelCreate(NULL, &wi);
	
	geventListenerInit(&gl);
	geventAttachSource(&gl, ginputGetKeyboard(0), 0);
	gwinKeyboardSetLayout(ghKeyboard, &numpadKeyboard);

}


static void guiT1ConfigMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

	char hours[3];
	char minutes[3];
	//char seconds[3];

	snprintf(hours, sizeof(hours), "%02d", get_hours_t1());
	snprintf(minutes, sizeof(minutes), "%02d", get_minutes_t1());
	//snprintf(seconds, sizeof(seconds), "%02d", get_seconds_t1());

	gwinSetText(ghTexteditHours, hours, TRUE);
	gwinSetText(ghTexteditMinutes, minutes, TRUE);
	//gwinSetText(ghTexteditSeconds, seconds, TRUE);

	// This function was manually added to the µGFX library under ugfx/src/gwin/gwin_textedit.h 
	gwinTextEditSetCursorPosition((GTexteditObject*) ghTexteditHours, strlen(hours));

}


static void guiT1ConfigMenu_onClose(GUIWindow *win) {

	(void) win;

	gwinSetText(ghLabel_ErrorTime, "", TRUE);
}


static int guiT1ConfigMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

        	if(peb->gwin == ghBtn_CancelT1) {

            	guiWindow_Show(&winT1T2Menu);

            } else if(peb->gwin == ghBtn_SetT1) {
             	
            	unsigned int new_hours;
            	unsigned int new_minutes;
            	//unsigned int new_seconds;

            	sscanf(gwinGetText(ghTexteditHours), "%u", &new_hours);

            	if(new_hours > 23) {
            		gwinSetText(ghLabel_ErrorTime, "Invalid hour!", TRUE);
            		return 1;
            	}

            	sscanf(gwinGetText(ghTexteditMinutes), "%u", &new_minutes);

            	if(new_minutes >= 60) {
            		gwinSetText(ghLabel_ErrorTime, "Invalid minute!", TRUE);
            		return 1;
            	}

            	//sscanf(gwinGetText(ghTexteditSeconds), "%u", &new_seconds);

            	set_hours_t1((uint8_t) new_hours);
            	set_minutes_t1((uint8_t) new_minutes);
            	//set_seconds((uint8_t) new_seconds);
            	// Reset the seconds every time the hours or the minutes are changed
            	set_seconds_t1((uint8_t) 0);

            	guiWindow_Show(&winT1T2Menu);

            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winT1T2ConfigT1Menu = {

/* Title   */	 "T1 Config Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiT1ConfigMenu_onShow,
/* onClose */    guiT1ConfigMenu_onClose,
/* onEvent */    guiT1ConfigMenu_handleEvent,
/* handle  */    0

};