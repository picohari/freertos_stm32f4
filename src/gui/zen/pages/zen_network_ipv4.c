#include <stdio.h>
#include <string.h>

#include "gfx.h"
#include "gui.h"
#include "src/gwin/gwin_keyboard_layout.h"
#include "stdio.h"

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

#include "skin/zenstyle.h"
#include "gui_router.h"

#include "pages/zen_network_ipv4.h"
#include "pages/zen_network.h"
#include "helpers/network_util.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageNetworkIpv4;

/* BUTTONS */
GHandle ghBtn_CancelIpv4;
GHandle ghBtn_SetIpv4;

/* IMAGES */
gdispImage ic_cancel;
gdispImage ic_done;


/* EVENT LISTENER */
static GListener gl;

/* INPUT FIELDS AND KEYBOARD */
static GHandle ghTexteditFirstBlock;
static GHandle ghTexteditSecondBlock;
static GHandle ghTexteditThirdBlock;
static GHandle ghTexteditFourthBlock;
static GHandle ghKeyboard;
static GHandle ghLabel_ErrorIpv4;

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

// Define the new ip address as a static variable, so it remains in memory after the onClose function is called. 
// If the new_ipv4_address variable is not static, the ipv4_address char pointer in the network_util will point
// to a wrong address after the onClose function is called, because the new_ipv4_address variable here won't exist anymore
static char new_ipv4_address[16];


void create_PageNetworkIpv4(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageNetworkIpv4
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
	ghContainer_PageNetworkIpv4 = gwinContainerCreate(0, &wi, 0);

	// Create the keyboard
	wi.g.show = TRUE;
	wi.g.x = 0; 
	wi.g.y = 58;
	wi.g.width = gdispGetWidth() / 2 + 80; 
	wi.g.height = gdispGetHeight() / 2 + 22;
	wi.g.parent = ghContainer_PageNetworkIpv4;
	ghKeyboard = gwinKeyboardCreate(0, &wi);

	// TextEditFirstBlock
	wi.g.show = TRUE;
	wi.g.x = 45;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkIpv4;
	wi.text = "";
	ghTexteditFirstBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditSecondBlock
	wi.g.show = TRUE;
	wi.g.x = 105;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkIpv4;
	ghTexteditSecondBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditThirdBlock
	wi.g.show = TRUE;
	wi.g.x = 165;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkIpv4;
	ghTexteditThirdBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditFourthBlock
	wi.g.show = TRUE;
	wi.g.x = 225;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkIpv4;
	ghTexteditFourthBlock = gwinTexteditCreate(0, &wi, 3);
	
	// create button widget: ghBtn_SetIpv4
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 58;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageNetworkIpv4;
	//wi.text = MENU_TITLE_OK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_done;
	wi.customStyle = &color_eight;
	ghBtn_SetIpv4 = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_CancelIpv4
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageNetworkIpv4;
	//wi.text = MENU_TITLE_CANCEL;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_cancel;
	wi.customStyle = &color_five;
	ghBtn_CancelIpv4 = gwinButtonCreate(0, &wi);

	// create the Label widget: ghLabel_ErrorIpv4
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;
	wi.g.x = 45;
	wi.g.y = 35;
	wi.g.width = 160;
	wi.g.height = 20;
	wi.g.parent = ghContainer_PageNetworkIpv4;
	wi.text = "";
	ghLabel_ErrorIpv4 = gwinLabelCreate(NULL, &wi);	
	
	geventListenerInit(&gl);
	geventAttachSource(&gl, ginputGetKeyboard(0), 0);
	gwinKeyboardSetLayout(ghKeyboard, &numpadKeyboard);

	set_ipv4_address("127.000.000.001");
	
}

static void guiNetworkIpv4Menu_onShow(GUIWindow *win) {

	gui_set_title(win);

	char* ipv4_address = get_ipv4_address();

	char firstBlock[4];
	char secondBlock[4];
	char thirdBlock[4];
	char fourthBlock[4];

	// The IPv4 address will be in the form xxx.xxx.xxx.xxx
	// so ignore the dots when copying to the blocks
	strncpy(firstBlock, ipv4_address, 3);
	firstBlock[3] = '\0';

	strncpy(secondBlock, ipv4_address + 4, 3);
	secondBlock[3] = '\0';

	strncpy(thirdBlock, ipv4_address + 8, 3);
	thirdBlock[3] = '\0';

	strncpy(fourthBlock, ipv4_address + 12, 3);
	fourthBlock[3] = '\0';

	gwinSetText(ghTexteditFirstBlock, firstBlock, TRUE);
	gwinSetText(ghTexteditSecondBlock, secondBlock, TRUE);
	gwinSetText(ghTexteditThirdBlock, thirdBlock, TRUE);
	gwinSetText(ghTexteditFourthBlock, fourthBlock, TRUE);

	// This function was manually added to the µGFX library under ugfx/src/gwin/gwin_textedit.h 
	gwinTextEditSetCursorPosition((GTexteditObject*) ghTexteditFirstBlock, strlen(firstBlock));

}

static void guiNetworkIpv4Menu_onClose(GUIWindow *win) {

	(void) win;

	gwinSetText(ghLabel_ErrorIpv4, "", TRUE);

}

static int guiNetworkIpv4Menu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_CancelIpv4) {
            	guiWindow_Show(&winNetworkMenu);

            } else if(peb->gwin == ghBtn_SetIpv4) {

            	unsigned int firstBlock;
            	unsigned int secondBlock;
            	unsigned int thirdBlock;
            	unsigned int fourthBlock;

            	sscanf(gwinGetText(ghTexteditFirstBlock), "%u", &firstBlock);
            	sscanf(gwinGetText(ghTexteditSecondBlock), "%u", &secondBlock);
            	sscanf(gwinGetText(ghTexteditThirdBlock), "%u", &thirdBlock);
            	sscanf(gwinGetText(ghTexteditFourthBlock), "%u", &fourthBlock);

            	if(firstBlock > 255 || secondBlock > 255 || thirdBlock > 255 || fourthBlock > 255) {
            		gwinSetText(ghLabel_ErrorIpv4, "Invalid IP Address!", TRUE);
            		break;
            	}

            	snprintf(new_ipv4_address, sizeof(new_ipv4_address), "%03d.%03d.%03d.%03d", firstBlock, secondBlock, thirdBlock, fourthBlock);

            	// Set the newly entered IP Address
            	set_ipv4_address(new_ipv4_address);

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



GUIWindow winNetworkIpv4Menu = {

/* Title   */	 "IPv4 Settings Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiNetworkIpv4Menu_onShow,
/* onClose */    guiNetworkIpv4Menu_onClose,
/* onEvent */    guiNetworkIpv4Menu_handleEvent,
/* handle  */    0

};