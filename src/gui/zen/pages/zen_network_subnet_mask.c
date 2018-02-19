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

#include "pages/zen_network_subnet_mask.h"
#include "pages/zen_network.h"
#include "helpers/network_util.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageNetworkSubnetMask;

/* BUTTONS */
GHandle ghBtn_CancelSubnetMask;
GHandle ghBtn_SetSubnetMask;

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
static GHandle ghLabel_ErrorSubnetMask;

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

static char new_subnet_mask[16];

void create_PageNetworkSubnetMask(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageNetworkSubnetMask
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
	ghContainer_PageNetworkSubnetMask = gwinContainerCreate(0, &wi, 0);

	// Create the keyboard
	wi.g.show = TRUE;
	wi.g.x = 0; 
	wi.g.y = 58;
	wi.g.width = gdispGetWidth() / 2 + 80; 
	wi.g.height = gdispGetHeight() / 2 + 22;
	wi.g.parent = ghContainer_PageNetworkSubnetMask;
	ghKeyboard = gwinKeyboardCreate(0, &wi);

	// TextEditFirstBlock
	wi.g.show = TRUE;
	wi.g.x = 45;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkSubnetMask;
	wi.text = "";
	ghTexteditFirstBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditSecondBlock
	wi.g.show = TRUE;
	wi.g.x = 105;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkSubnetMask;
	ghTexteditSecondBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditThirdBlock
	wi.g.show = TRUE;
	wi.g.x = 165;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkSubnetMask;
	ghTexteditThirdBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditFourthBlock
	wi.g.show = TRUE;
	wi.g.x = 225;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageNetworkSubnetMask;
	ghTexteditFourthBlock = gwinTexteditCreate(0, &wi, 3);
	
	// create button widget: ghBtn_SetSubnetMask
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 58;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageNetworkSubnetMask;
	//wi.text = MENU_TITLE_OK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_done;
	wi.customStyle = &color_eight;
	ghBtn_SetSubnetMask = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_CancelSubnetMask
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageNetworkSubnetMask;
	//wi.text = MENU_TITLE_CANCEL;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_cancel;
	wi.customStyle = &color_five;
	ghBtn_CancelSubnetMask = gwinButtonCreate(0, &wi);

	// create the Label widget: ghLabel_ErrorSubnetMask
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;
	wi.g.x = 45;
	wi.g.y = 35;
	wi.g.width = 180;
	wi.g.height = 20;
	wi.g.parent = ghContainer_PageNetworkSubnetMask;
	wi.text = "";
	ghLabel_ErrorSubnetMask = gwinLabelCreate(NULL, &wi);	
	
	geventListenerInit(&gl);
	geventAttachSource(&gl, ginputGetKeyboard(0), 0);
	gwinKeyboardSetLayout(ghKeyboard, &numpadKeyboard);

	set_subnet_mask("225.225.225.225");
	
}


static void guiNetworkSubnetMaskMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

	char* subnet_mask = get_subnet_mask();

	char firstBlock[4];
	char secondBlock[4];
	char thirdBlock[4];
	char fourthBlock[4];

	// The gateway will be in the form xxx.xxx.xxx.xxx
	// so ignore the dots when copying to the blocks
	strncpy(firstBlock, subnet_mask, 3);
	firstBlock[3] = '\0';

	strncpy(secondBlock, subnet_mask + 4, 3);
	secondBlock[3] = '\0';

	strncpy(thirdBlock, subnet_mask + 8, 3);
	thirdBlock[3] = '\0';

	strncpy(fourthBlock, subnet_mask + 12, 3);
	fourthBlock[3] = '\0';

	gwinSetText(ghTexteditFirstBlock, firstBlock, TRUE);
	gwinSetText(ghTexteditSecondBlock, secondBlock, TRUE);
	gwinSetText(ghTexteditThirdBlock, thirdBlock, TRUE);
	gwinSetText(ghTexteditFourthBlock, fourthBlock, TRUE);

	// This function was manually added to the µGFX library under ugfx/src/gwin/gwin_textedit.h 
	gwinTextEditSetCursorPosition((GTexteditObject*) ghTexteditFirstBlock, strlen(firstBlock));

}

static void guiNetworkSubnetMaskMenu_onClose(GUIWindow *win) {

	(void) win;

	gwinSetText(ghLabel_ErrorSubnetMask, "", TRUE);

}

static int guiNetworkSubnetMaskMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_CancelSubnetMask) {
            	guiWindow_Show(&winNetworkMenu);

            } else if(peb->gwin == ghBtn_SetSubnetMask) {

            	unsigned int firstBlock;
            	unsigned int secondBlock;
            	unsigned int thirdBlock;
            	unsigned int fourthBlock;

            	sscanf(gwinGetText(ghTexteditFirstBlock), "%u", &firstBlock);
            	sscanf(gwinGetText(ghTexteditSecondBlock), "%u", &secondBlock);
            	sscanf(gwinGetText(ghTexteditThirdBlock), "%u", &thirdBlock);
            	sscanf(gwinGetText(ghTexteditFourthBlock), "%u", &fourthBlock);

            	if(firstBlock > 255 || secondBlock > 255 || thirdBlock > 255 || fourthBlock > 255) {
            		gwinSetText(ghLabel_ErrorSubnetMask, "Invalid Subnet Mask!", TRUE);
            		break;
            	}

            	snprintf(new_subnet_mask, sizeof(new_subnet_mask), "%03d.%03d.%03d.%03d", firstBlock, secondBlock, thirdBlock, fourthBlock);
            	
            	// Set the newly entered Subnet Mask
            	set_subnet_mask(new_subnet_mask);

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



GUIWindow winNetworkSubnetMaskMenu = {

/* Title   */	 "Network Subnet Mask Settings Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiNetworkSubnetMaskMenu_onShow,
/* onClose */    guiNetworkSubnetMaskMenu_onClose,
/* onEvent */    guiNetworkSubnetMaskMenu_handleEvent,
/* handle  */    0

};