/* ============================================================================
 * File:        setup_remoteip.c
 * Description: 
 * ============================================================================ */
 
#include <stdbool.h>
#include "gfx.h"
#include "gui.h"
//#include "src/gwin/gwin_keyboard.h"


#ifdef UGFXSIMULATOR
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
#endif

#include "style.h"
#include "gui_menu.h"
#include "axis_menu.h"
#include "gui_logger.h"



/* PAGE CONTAINER & WIDGETS */
//static GHandle ghCont_RemoteIPHeader;
//static GHandle ghCont_RemoteIPBody;
static GHandle ghCont_RemoteIPFooter;

static GHandle ghLabelMenuName;
static GHandle ghKeyboard;
static GHandle ghTexteditFirstBlock;
static GHandle ghTexteditSecondBlock;
static GHandle ghTexteditThirdBlock;
static GHandle ghTexteditFourthBlock;

/* BUTTONS */
static GHandle ghBtn_MenuSetup1;
static GHandle ghBtn_MenuSetup2;
static GHandle ghBtn_MenuSetup3;
static GHandle ghBtn_MenuSave;
static GHandle ghBtn_MenuReturn;

/* Page Prototypes */
static void SetupRemoteIP_onShow(void);
static void SetupRemoteIP_onClose(void);
static bool SetupRemoteIP_onEvent(MenuPageDef_t *page, GEvent *pe);


/* NumPad with digits only (with none of the above) */


/* Create µGFX page */
void create_SetupRemoteIP(void) {

	GWidgetInit		wi;
	gwinWidgetClearInit(&wi);

	gwinSetDefaultFont(ctrld_16b);

    /* MenuRemoteIP container - must be present on all pages if in menu tree */
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 320;
	wi.g.height = 240;
	wi.g.parent = 0;
	wi.text = "Setup: RemoteIP";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;

	/* Menu assignments */
	menuPages[SETUP_REMOTEIP].title 	= wi.text;
	menuPages[SETUP_REMOTEIP].container = gwinContainerCreate(0, &wi, 0);
    menuPages[SETUP_REMOTEIP].id 		= SETUP_REMOTEIP;
    menuPages[SETUP_REMOTEIP].onShow 	= SetupRemoteIP_onShow;
    menuPages[SETUP_REMOTEIP].onClose 	= SetupRemoteIP_onClose;
	menuPages[SETUP_REMOTEIP].onEvent 	= SetupRemoteIP_onEvent;


	/* Here comes the content for this page */

	gwinWidgetClearInit(&wi);
	
	/* Status Label */
	#if 1
	wi.g.show = TRUE;
	wi.g.x = 2;
	wi.g.y = 1;
	wi.g.width  = 320;
	wi.g.height = 16;
	wi.g.parent = menuPages[SETUP_REMOTEIP].container;
	wi.text = "Edit: Remote IP Address";
	wi.customDraw = gwinLabelDrawJustifiedLeft;
	ghLabelMenuName = gwinLabelCreate(0, &wi);
	#endif
	
	// Virtual keyboard
	gwinWidgetClearInit(&wi);
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = (gdispGetHeight()/2) - 30;
	wi.g.width = gdispGetWidth();
	wi.g.height = gdispGetHeight()/2 - 42 + 30;
	wi.g.parent = menuPages[SETUP_REMOTEIP].container;
	ghKeyboard = gwinKeyboardCreate(0, &wi);


	// TextEditFirstBlock
	wi.g.y = 30;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.show = TRUE;
	wi.g.parent = menuPages[SETUP_REMOTEIP].container;
	wi.text = "";

	wi.g.x = 45;
	ghTexteditFirstBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditSecondBlock
	wi.g.x = 105;
	ghTexteditSecondBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditThirdBlock
	wi.g.x = 165;
	ghTexteditThirdBlock = gwinTexteditCreate(0, &wi, 3);

	// TextEditFourthBlock
	wi.g.x = 225;
	ghTexteditFourthBlock = gwinTexteditCreate(0, &wi, 3);





	/* Footer container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 198;
	wi.g.width  = 320;
	wi.g.height = 42;				// change to 38 if borders are enabled
	//wi.g.parent = ghCont_HomeMain;
	wi.g.parent = menuPages[SETUP_REMOTEIP].container;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghCont_RemoteIPFooter = gwinContainerCreate(0, &wi, 0);

	/* Default parameters for buttons */
	gwinWidgetClearInit(&wi);

	wi.g.show = gTrue;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 65;
	wi.g.height = 42;
	wi.g.parent = ghCont_RemoteIPFooter;
	wi.customStyle = &color_buttons;

	/* Button 1 */
	wi.g.x = 0;
	wi.text = "";
	ghBtn_MenuSetup1 = gwinButtonCreate(0, &wi);

	/* Button 2 */
	wi.g.x = 64;
	wi.text = "";
	ghBtn_MenuSetup2 = gwinButtonCreate(0, &wi);	

	/* Button 3 */
	wi.g.x = 128;
	wi.text = "";
	ghBtn_MenuSetup3 = gwinButtonCreate(0, &wi);	

	/* Button 4 */
	wi.g.x = 192;
	wi.text = "SAVE";
	ghBtn_MenuSave = gwinButtonCreate(0, &wi);	

	/* Button 5 */
	wi.g.x = 256;
	wi.text = "BACK";
	ghBtn_MenuReturn = gwinButtonCreate(0, &wi);

}



static void SetupRemoteIP_onShow(void) {
    LOG_MENU("SETUP_REMOTEIP: onShow");
	gwinSetVisible(ghKeyboard, TRUE);

	uint32_t ipv4_address;
	//axis_helper_getRemoteIP(&ipv4_address);

	char firstBlock[4];
	char secondBlock[4];
	char thirdBlock[4];
	char fourthBlock[4];

	snprintf(firstBlock,  sizeof(firstBlock), 	"%lu", ((ipv4_address >> 24) & 0xFF));
	snprintf(secondBlock, sizeof(secondBlock), 	"%lu", ((ipv4_address >> 16) & 0xFF));
	snprintf(thirdBlock,  sizeof(thirdBlock), 	"%lu", ((ipv4_address >> 8) & 0xFF));
	snprintf(fourthBlock, sizeof(fourthBlock), 	"%lu", ((ipv4_address >> 0) & 0xFF));

	gwinSetText(ghTexteditFirstBlock, 	firstBlock,  TRUE);
	gwinSetText(ghTexteditSecondBlock, 	secondBlock, TRUE);
	gwinSetText(ghTexteditThirdBlock, 	thirdBlock,  TRUE);
	gwinSetText(ghTexteditFourthBlock, 	fourthBlock, TRUE);	

}


static void SetupRemoteIP_onClose(void) {
    LOG_MENU("SETUP_REMOTEIP: onClose");
}


static bool SetupRemoteIP_onEvent(MenuPageDef_t *page, GEvent *pe) {
    (void)page; // not used yet

    switch (pe->type) {
		
        case GEVENT_GWIN_BUTTON: {
            GEventGWinButton *be = (GEventGWinButton *)pe;
            if (be->gwin == ghBtn_MenuReturn) {
                Menu_ShowPage(MENU_SETUP);
				LOG_MENU("RETURN pressed!");
                return TRUE;
            }

            else if (be->gwin == ghBtn_MenuSave) {
            	uint8_t firstBlock;
            	uint8_t secondBlock;
            	uint8_t thirdBlock;
            	uint8_t fourthBlock;

            	sscanf(gwinGetText(ghTexteditFirstBlock),  "%hhd", &firstBlock);
            	sscanf(gwinGetText(ghTexteditSecondBlock), "%hhd", &secondBlock);
            	sscanf(gwinGetText(ghTexteditThirdBlock),  "%hhd", &thirdBlock);
            	sscanf(gwinGetText(ghTexteditFourthBlock), "%hhd", &fourthBlock);
#if 0
            	if(firstBlock >= 255 || secondBlock >= 255 || thirdBlock >= 255 || fourthBlock >= 255) {
            		gwinSetText(ghLabelMenuName, "Invalid IP Address!", TRUE);
            		return 1;
            	}
#endif
            	uint32_t new_ipv4_address = (firstBlock << 24) + (secondBlock << 16) + (thirdBlock << 8) + (fourthBlock << 0);

            	// Set the newly entered IP Address
            	//axis_helper_setRemoteIP(&new_ipv4_address);

				LOG_MENU("SAVED IP: 0x%08x", new_ipv4_address);
                return TRUE;
            }

            break;
        }

        default:
            break;
    }

    return false; // not handled
}
