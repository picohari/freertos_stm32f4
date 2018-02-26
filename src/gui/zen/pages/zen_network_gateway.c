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

#include "pages/zen_network_gateway.h"
#include "pages/zen_network.h"
#include "helpers/network_util.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageNetworkGateway;

/* BUTTONS */
GHandle ghBtn_CancelGateway;
GHandle ghBtn_SetGateway;

/* IMAGES */

/* EVENT LISTENER */
static GListener gl;

/* INPUT FIELDS AND KEYBOARD */
static GHandle ghTexteditFirstBlock;
static GHandle ghTexteditSecondBlock;
static GHandle ghTexteditThirdBlock;
static GHandle ghTexteditFourthBlock;
static GHandle ghKeyboard;
static GHandle ghLabel_ErrorGateway;

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

void create_PageNetworkGateway(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageNetworkGateway
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
	ghContainer_PageNetworkGateway = gwinContainerCreate(0, &wi, 0);

	// Create the keyboard
	wi.g.show = TRUE;
	wi.g.x = 0; 
	wi.g.y = 58;
	wi.g.width = gdispGetWidth() / 2 + 80; 
	wi.g.height = gdispGetHeight() / 2 + 22;
	wi.g.parent = ghContainer_PageNetworkGateway;
	ghKeyboard = gwinKeyboardCreate(0, &wi);

	// TextEditFirstBlock
	wi.g.show = TRUE;
	wi.g.x = 45;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkGateway;
	wi.text = "";
	ghTexteditFirstBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditSecondBlock
	wi.g.show = TRUE;
	wi.g.x = 105;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkGateway;
	ghTexteditSecondBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditThirdBlock
	wi.g.show = TRUE;
	wi.g.x = 165;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkGateway;
	ghTexteditThirdBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditFourthBlock
	wi.g.show = TRUE;
	wi.g.x = 225;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkGateway;
	ghTexteditFourthBlock = gwinTexteditCreate(0, &wi, 3);
	
	// create button widget: ghBtn_SetGateway
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 58;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageNetworkGateway;
	//wi.text = MENU_TITLE_OK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_done;
	wi.customStyle = &color_eight;
	ghBtn_SetGateway = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_CancelGateway
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageNetworkGateway;
	//wi.text = MENU_TITLE_CANCEL;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_cancel;
	wi.customStyle = &color_five;
	ghBtn_CancelGateway = gwinButtonCreate(0, &wi);

	// create the Label widget: ghLabel_ErrorGateway
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;
	wi.g.x = 45;
	wi.g.y = 35;
	wi.g.width = 180;
	wi.g.height = 20;
	wi.g.parent = ghContainer_PageNetworkGateway;
	wi.text = "";
	ghLabel_ErrorGateway = gwinLabelCreate(NULL, &wi);	
	
	geventListenerInit(&gl);
	geventAttachSource(&gl, ginputGetKeyboard(0), 0);
	gwinKeyboardSetLayout(ghKeyboard, &numpadKeyboard);

	// 4294967295 is 255.255.255.255
	set_gateway(4294967295);
	
}


static void guiNetworkGatewayMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

	uint32_t gateway = get_gateway();

	char firstBlock[4];
	char secondBlock[4];
	char thirdBlock[4];
	char fourthBlock[4];

	snprintf(firstBlock, sizeof(firstBlock), "%u", ((gateway >> 24) & 0xFF));
	snprintf(secondBlock, sizeof(secondBlock), "%u", ((gateway >> 16) & 0xFF));
	snprintf(thirdBlock, sizeof(thirdBlock), "%u", ((gateway >> 8) & 0xFF));
	snprintf(fourthBlock, sizeof(fourthBlock), "%u", ((gateway >> 0) & 0xFF));

	gwinSetText(ghTexteditFirstBlock, firstBlock, TRUE);
	gwinSetText(ghTexteditSecondBlock, secondBlock, TRUE);
	gwinSetText(ghTexteditThirdBlock, thirdBlock, TRUE);
	gwinSetText(ghTexteditFourthBlock, fourthBlock, TRUE);	

	// This function was manually added to the µGFX library under ugfx/src/gwin/gwin_textedit.h 
	gwinTextEditSetCursorPosition((GTexteditObject*) ghTexteditFirstBlock, strlen(firstBlock));

}

static void guiNetworkGatewayMenu_onClose(GUIWindow *win) {

	(void) win;

	gwinSetText(ghLabel_ErrorGateway, "", TRUE);

}

static int guiNetworkGatewayMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_CancelGateway) {
            	guiWindow_Show(&winNetworkMenu);

            } else if(peb->gwin == ghBtn_SetGateway) {

            	unsigned int firstBlock;
            	unsigned int secondBlock;
            	unsigned int thirdBlock;
            	unsigned int fourthBlock;

            	sscanf(gwinGetText(ghTexteditFirstBlock), "%u", &firstBlock);
            	sscanf(gwinGetText(ghTexteditSecondBlock), "%u", &secondBlock);
            	sscanf(gwinGetText(ghTexteditThirdBlock), "%u", &thirdBlock);
            	sscanf(gwinGetText(ghTexteditFourthBlock), "%u", &fourthBlock);

            	if(firstBlock > 255 || secondBlock > 255 || thirdBlock > 255 || fourthBlock > 255) {
            		gwinSetText(ghLabel_ErrorGateway, "Invalid Gateway Address!", TRUE);
            		return 1;
            	}

            	uint32_t new_gateway = (firstBlock << 24) + (secondBlock << 16) + (thirdBlock << 8) + (fourthBlock << 0);

            	// Set the newly entered Gateway
            	set_gateway(new_gateway);

            	guiWindow_Show(&winNetworkMenu);
            	
            } else {
            	return 0;
            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winNetworkGatewayMenu = {

/* Title   */	 "Network Gateway Settings Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiNetworkGatewayMenu_onShow,
/* onClose */    guiNetworkGatewayMenu_onClose,
/* onEvent */    guiNetworkGatewayMenu_handleEvent,
/* handle  */    0

};