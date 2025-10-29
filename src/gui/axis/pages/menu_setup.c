/* ============================================================================
 * File:        menu_setup.c
 * Description: 
 * ============================================================================ */

#include <stdbool.h>
#include "gfx.h"
#include "gui.h"



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
#include "gui_logger.h"

//#include "src/gwin/gwin_keyboard_layout.h"

/* PAGE CONTAINER & WIDGETS */
static GHandle ghCont_SetupBody;
static GHandle ghCont_SetupFooter;
static GHandle ghConsole;

/* LABELS */
static GHandle ghLabelMenuName;

/* LISTS */
static GHandle ghSetupList;
static GHandle ghKeyboard;

//Keyboard layouts


/* BUTTONS */
static GHandle ghBtn_MenuUp;
static GHandle ghBtn_MenuDown;
static GHandle ghBtn_MenuSetup3;
static GHandle ghBtn_MenuEnter;
static GHandle ghBtn_MenuBACK;

/* EVENTS */
static GListener glList;
static int8_t selected = 0;
static uint8_t number = 0;

/* Page Prototypes */
static void MenuSetup_onShow(void);
static void MenuSetup_onClose(void);
static bool MenuSetup_onEvent(MenuPageDef_t *page, GEvent *pe);



/* Create µGFX page */
void create_MenuSetup(void) {

	GWidgetInit		wi;
	
	//gwinSetDefaultFont(dejavu_sans_16);
	gwinSetDefaultStyle(&BlackWidgetStyle, FALSE);
	gdispClear(Black);

	gwinWidgetClearInit(&wi);

	//gwinSetDefaultFont(ctrld_16b);

    /* MenuSetup container - must be present on all pages if in menu tree */
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 320;
	wi.g.height = 240;
	wi.g.parent = 0;
	wi.text = "Menu: Setup";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;

	/* Menu assignments */
	menuPages[MENU_SETUP].title 	= wi.text;
	menuPages[MENU_SETUP].container = gwinContainerCreate(0, &wi, 0);
    menuPages[MENU_SETUP].id 		= MENU_SETUP;
    menuPages[MENU_SETUP].onShow 	= MenuSetup_onShow;
    menuPages[MENU_SETUP].onClose 	= MenuSetup_onClose;
	menuPages[MENU_SETUP].onEvent 	= MenuSetup_onEvent;
	menuPages[MENU_SETUP].onCycle 	= 0;


	gwinWidgetClearInit(&wi);

	#if 1
	/* The first list widget */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 19;
	wi.g.width = 320;
	wi.g.height = 178;
	wi.g.parent = menuPages[MENU_SETUP].container;
	//wi.g.parent = ghCont_SetupBody;
	wi.text = "Settings";
	wi.customStyle = 0;
	ghSetupList = gwinListCreate(NULL, &wi, FALSE);

	gwinListAddItem(ghSetupList, "Local IP Address", TRUE);
	gwinListAddItem(ghSetupList, "Local Port", TRUE);
	gwinListAddItem(ghSetupList, "Remote IP Address", FALSE);
	gwinListAddItem(ghSetupList, "Remote Port", FALSE);
	gwinListAddItem(ghSetupList, "Netmask", FALSE);
	gwinListAddItem(ghSetupList, "Gateway", FALSE);
	gwinSetVisible(ghSetupList, TRUE);
	#endif


#if 0
	// Create the console - set colors before making it visible
	wi.g.show = TRUE;
	wi.g.x = 0; wi.g.y = 0;
	wi.g.width = gdispGetWidth(); wi.g.height = gdispGetHeight()/2;
	ghConsole = gwinConsoleCreate(0, &wi.g);
	gwinSetColor(ghConsole, GFX_BLACK);
	gwinSetBgColor(ghConsole, HTML2COLOR(0xF0F0F0));
	gwinShow(ghConsole);
	gwinClear(ghConsole);

	// Create the keyboard
	wi.g.show = TRUE;
	wi.g.x = 0; wi.g.y = gdispGetHeight()/2;
	wi.g.width = gdispGetWidth(); wi.g.height = gdispGetHeight()/2;
	ghKeyboard = gwinKeyboardCreate(0, &wi);
#endif

	/* Here comes the content for this page */
#if 0
	/* Body container */
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 320;
	wi.g.height = 198;
	//wi.g.parent = ghCont_HomeMain;
	wi.g.parent = menuPages[MENU_SETUP].container;
	wi.text = "";
	ghCont_SetupBody = gwinContainerCreate(0, &wi, GWIN_CONTAINER_BORDER);
#endif


	/* Status Label */
	wi.g.show = TRUE;
	wi.g.x = 2;
	wi.g.y = 1;
	wi.g.width  = 320;
	wi.g.height = 16;
	wi.g.parent = menuPages[MENU_SETUP].container;
	wi.text = "Menu: Network Setup";
	wi.customDraw = gwinLabelDrawJustifiedLeft;
	ghLabelMenuName = gwinLabelCreate(0, &wi);
	


#if 1
	/* Footer container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 198;
	wi.g.width  = 320;
	wi.g.height = 42;				// change to 38 if borders are enabled
	//wi.g.parent = ghCont_HomeMain;
	wi.g.parent = menuPages[MENU_SETUP].container;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghCont_SetupFooter = gwinContainerCreate(0, &wi, 0);


	/* Default parameters for buttons */
	gwinWidgetClearInit(&wi);

	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 65;
	wi.g.height = 42;
	wi.g.parent = ghCont_SetupFooter;
	wi.customStyle = &color_buttons;

	/* Button 1 */
	wi.g.x = 0;
	wi.text = "Up";
	ghBtn_MenuUp = gwinButtonCreate(0, &wi);

	/* Button 2 */
	wi.g.x = 64;
	wi.text = "Down";
	ghBtn_MenuDown = gwinButtonCreate(0, &wi);	

	/* Button 3 */
	wi.g.x = 128;
	wi.text = "";
	ghBtn_MenuSetup3  = gwinButtonCreate(0, &wi);	

	/* Button 4 */
	wi.g.x = 192;
	wi.text = "ENTER";
	ghBtn_MenuEnter = gwinButtonCreate(0, &wi);	

	/* Button 5 */
	wi.g.x = 256;
	wi.text = "BACK";
	ghBtn_MenuBACK = gwinButtonCreate(0, &wi);

	//gwinKeyboardSetLayout(ghKeyboard, &numpadKeyboard);
#endif
}

static void select_next_item(void) {
    int count = gwinListItemCount(ghSetupList);
    selected++;
    if (selected >= count) selected = 0;  // wrap around
    gwinListSetSelected(ghSetupList, selected, TRUE);
}

static void select_prev_item(void) {
    int count = gwinListItemCount(ghSetupList);
    selected--;
    if (selected < 0) selected = count - 1;  // wrap around
    gwinListSetSelected(ghSetupList, selected, TRUE);
}


static void MenuSetup_onShow(void) {
    LOG_MENU("SETUP: onShow");
}


static void MenuSetup_onClose(void) {
    LOG_MENU("SETUP: onClose");
}


static bool MenuSetup_onEvent(MenuPageDef_t *page, GEvent *pe) {
    (void)page; // not used yet

    switch (pe->type) {
		
        case GEVENT_GWIN_BUTTON: {
			GEventGWinButton *be = (GEventGWinButton *)pe;

            if (be->gwin == ghBtn_MenuBACK) {
                Menu_ShowPage(PAGE_MAIN);
				//LOG_MENU("BACK");
                return true;
            }

            else if (be->gwin == ghBtn_MenuDown) {
				select_next_item();
				//LOG_MENU("UTILS pressed!");
                return true;
            }

			else if (be->gwin == ghBtn_MenuUp) {
				select_prev_item();
				//LOG_MENU("UTILS pressed!");
                return true;
            }

			else if (be->gwin == ghBtn_MenuEnter) {
				//const char *item = gwinListGetSelectedText(ghSetupList);
				//LOG_MENU("Selected: %s", item);
				//gwinListSetSelected(ghSetupList, selected, TRUE);

				number = gwinListGetSelected(ghSetupList);
				LOG_MENU("Selected: %d", number);
			}

            //break;

        }

		case GEVENT_GWIN_LIST: {

			switch (number) {
				case 0:
					Menu_ShowPage(SETUP_LOCALIP);
					break;

				case 1:
					//Menu_ShowPage(SETUP_LOCALPORT);
					break;

				case 2:
					Menu_ShowPage(SETUP_REMOTEIP);
					return TRUE;

				case 3:
					//Menu_ShowPage(SETUP_REMOTEPORT);
					return TRUE;

				case 4:
					Menu_ShowPage(SETUP_NETMASK);
					return TRUE;

				case 5:
					Menu_ShowPage(SETUP_GATEWAY);
					return TRUE;
					
				default:
					break;
			}

            break;
        }

        default:
            break;
	}
    
    return false; // not handled
}
