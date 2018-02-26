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

#include "pages/zen_aeration_a2.h"
#include "pages/zen_aeration.h"
#include "helpers/zen_util.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageAerationA2;


/* BUTTONS */
GHandle ghBtn_CancelAerationA2;
GHandle ghBtn_SetAerationA2;


/* IMAGES */


/* EVENT LISTENER */
static GListener gl;

/* LABELS */

/* INPUT FIELDS AND KEYBOARD */
static GHandle ghTexteditA2;
static GHandle ghKeyboard;
static GHandle ghLabel_ErrorA2;

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

void create_PageAerationA2(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageAerationA2
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
	ghContainer_PageAerationA2 = gwinContainerCreate(0, &wi, 0);

	// HANDLE THE PRESS OF THE DELETE KEY WHEN NO TextEdit IS SELECTED
	// Create the keyboard
	wi.g.show = TRUE;
	wi.g.x = 0; 
	wi.g.y = 58;
	wi.g.width = gdispGetWidth() / 2 + 80; 
	wi.g.height = gdispGetHeight() / 2 + 22;
	wi.g.parent = ghContainer_PageAerationA2;
	ghKeyboard = gwinKeyboardCreate(0, &wi);

	// TexteditAerationA2
	wi.g.show = TRUE;
	wi.g.x = 135;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageAerationA2;
	wi.text = "";
	ghTexteditA2 = gwinTexteditCreate(0, &wi, 2);

	
	// create button widget: ghBtn_SetAerationA2
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 58;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageAerationA2;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_done;
	wi.customStyle = &color_eight;
	ghBtn_SetAerationA2 = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_CancelAerationA2
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageAerationA2;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_cancel;
	wi.customStyle = &color_five;
	ghBtn_CancelAerationA2 = gwinButtonCreate(0, &wi);

	// create the Label widget: ghLabel_ErrorA2
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;
	wi.g.x = 110;
	wi.g.y = 35;
	wi.g.width = 100;
	wi.g.height = 20;
	wi.g.parent = ghContainer_PageAerationA2;
	wi.text = "";
	ghLabel_ErrorA2 = gwinLabelCreate(NULL, &wi);	

	geventListenerInit(&gl);
	geventAttachSource(&gl, ginputGetKeyboard(0), 0);
	gwinKeyboardSetLayout(ghKeyboard, &numpadKeyboard);
	
}


static void guiAerationA2Menu_onShow(GUIWindow *win) {

	gui_set_title(win);

	gui_set_title(win);

	char aerationA2AsArray[3];
    snprintf(aerationA2AsArray, sizeof(aerationA2AsArray), "%u", get_aeration_a2());

   	gwinSetText(ghTexteditA2, aerationA2AsArray, TRUE);

   	// This function was manually added to the µGFX library under ugfx/src/gwin/gwin_textedit.h
   	// Move the cursor away so it is not visible. We don't actually need it here, because we edit the text
   	// of the TextEdit with the help of the buttons and not with the keyboard 
   	gwinTextEditSetCursorPosition((GTexteditObject*) ghTexteditA2, strlen(aerationA2AsArray));

}

static void guiAerationA2Menu_onClose(GUIWindow *win) {

	(void) win;

	gwinSetText(ghLabel_ErrorA2, "", TRUE);

}

static int guiAerationA2Menu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

             if(peb->gwin == ghBtn_CancelAerationA2) {

            	guiWindow_Show(&winAerationMenu);

            } else if(peb->gwin == ghBtn_SetAerationA2) {

            	
            	if(strlen(gwinGetText(ghTexteditA2)) < 1) {
            		return 1;
            	}

            	unsigned short aerationA2;

            	sscanf(gwinGetText(ghTexteditA2), "%hu", &aerationA2);

            	if(aerationA2 > 25) {
            		gwinSetText(ghLabel_ErrorA2, "Invalid value!", TRUE);
            		return 1;
            	} 
            		
            	set_aeration_a2((uint8_t) aerationA2);
            	

            	guiWindow_Show(&winAerationMenu);

            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winAerationA2Menu = {

/* Title   */	 "A2 Configuration (0 - 25)",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiAerationA2Menu_onShow,
/* onClose */    guiAerationA2Menu_onClose,
/* onEvent */    guiAerationA2Menu_handleEvent,
/* handle  */    0

};