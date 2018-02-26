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

#include "pages/zen_aeration_a1.h"
#include "pages/zen_aeration.h"
#include "helpers/zen_util.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageAerationA1;


/* BUTTONS */
GHandle ghBtn_CancelAerationA1;
GHandle ghBtn_SetAerationA1;


/* IMAGES */


/* EVENT LISTENER */
static GListener gl;

/* LABELS */

/* INPUT FIELDS AND KEYBOARD */
static GHandle ghTexteditA1;
static GHandle ghKeyboard;
static GHandle ghLabel_ErrorA1;

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

void create_PageAerationA1(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageAerationA1
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
	ghContainer_PageAerationA1 = gwinContainerCreate(0, &wi, 0);

	// HANDLE THE PRESS OF THE DELETE KEY WHEN NO TextEdit IS SELECTED
	// Create the keyboard
	wi.g.show = TRUE;
	wi.g.x = 0; 
	wi.g.y = 58;
	wi.g.width = gdispGetWidth() / 2 + 80; 
	wi.g.height = gdispGetHeight() / 2 + 22;
	wi.g.parent = ghContainer_PageAerationA1;
	ghKeyboard = gwinKeyboardCreate(0, &wi);

	// TexteditAerationA1
	wi.g.show = TRUE;
	wi.g.x = 135;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageAerationA1;
	wi.text = "";
	ghTexteditA1 = gwinTexteditCreate(0, &wi, 2);

	
	// create button widget: ghBtn_SetAerationA1
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 58;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageAerationA1;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_done;
	wi.customStyle = &color_eight;
	ghBtn_SetAerationA1 = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_CancelAerationA1
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageAerationA1;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_cancel;
	wi.customStyle = &color_five;
	ghBtn_CancelAerationA1 = gwinButtonCreate(0, &wi);

	// create the Label widget: ghLabel_ErrorA1
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;
	wi.g.x = 110;
	wi.g.y = 35;
	wi.g.width = 100;
	wi.g.height = 20;
	wi.g.parent = ghContainer_PageAerationA1;
	wi.text = "";
	ghLabel_ErrorA1 = gwinLabelCreate(NULL, &wi);	

	geventListenerInit(&gl);
	geventAttachSource(&gl, ginputGetKeyboard(0), 0);
	gwinKeyboardSetLayout(ghKeyboard, &numpadKeyboard);
	
}


static void guiAerationA1Menu_onShow(GUIWindow *win) {

	gui_set_title(win);

	gui_set_title(win);

	char aerationA1AsArray[3];
    snprintf(aerationA1AsArray, sizeof(aerationA1AsArray), "%u", get_aeration_a1());

   	gwinSetText(ghTexteditA1, aerationA1AsArray, TRUE);

   	// This function was manually added to the µGFX library under ugfx/src/gwin/gwin_textedit.h
   	// Move the cursor away so it is not visible. We don't actually need it here, because we edit the text
   	// of the TextEdit with the help of the buttons and not with the keyboard 
   	gwinTextEditSetCursorPosition((GTexteditObject*) ghTexteditA1, strlen(aerationA1AsArray));

}

static void guiAerationA1Menu_onClose(GUIWindow *win) {

	(void) win;

	gwinSetText(ghLabel_ErrorA1, "", TRUE);

}

static int guiAerationA1Menu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

             if(peb->gwin == ghBtn_CancelAerationA1) {

            	guiWindow_Show(&winAerationMenu);

            } else if(peb->gwin == ghBtn_SetAerationA1) {

            	
            	if(strlen(gwinGetText(ghTexteditA1)) < 1) {
            		return 1;
            	}

            	unsigned short aerationA1;

            	sscanf(gwinGetText(ghTexteditA1), "%hu", &aerationA1);

            	if(aerationA1 > 25) {
            		gwinSetText(ghLabel_ErrorA1, "Invalid value!", TRUE);
            		return 1;
            	} 
            		
            	set_aeration_a1((uint8_t) aerationA1);
            	

            	guiWindow_Show(&winAerationMenu);

            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winAerationA1Menu = {

/* Title   */	 "A1 Configuration (0 - 25)",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiAerationA1Menu_onShow,
/* onClose */    guiAerationA1Menu_onClose,
/* onEvent */    guiAerationA1Menu_handleEvent,
/* handle  */    0

};