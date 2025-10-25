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


/* PAGE CONTAINER & WIDGETS */
GHandle ghCont_SetupHeader;
GHandle ghCont_SetupBody;
GHandle ghCont_SetupFooter;


/* BUTTONS */
GHandle ghBtn_MenuSetup1;
GHandle ghBtn_MenuSetup2;
GHandle ghBtn_MenuSetup3;
GHandle ghBtn_MenuSetup4;
GHandle ghBtn_MenuReturn;

/* Page Prototypes */
static void MenuSetup_onShow(void);
static void MenuSetup_onClose(void);
static bool MenuSetup_onEvent(MenuPageDef_t *page, GEvent *pe);



/* Create µGFX page */
void create_MenuSetup(void) {

	GWidgetInit		wi;
	gwinWidgetClearInit(&wi);

	gwinSetDefaultFont(ctrld_16b);

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


	/* Here comes the content for this page */





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

	wi.g.show = gTrue;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 65;
	wi.g.height = 42;
	wi.g.parent = ghCont_SetupFooter;
	wi.customStyle = &color_buttons;

	/* Button 1 */
	wi.g.x = 0;
	wi.text = "1";
	ghBtn_MenuSetup1 = gwinButtonCreate(0, &wi);

	/* Button 2 */
	wi.g.x = 64;
	wi.text = "2";
	ghBtn_MenuSetup2 = gwinButtonCreate(0, &wi);	

	/* Button 3 */
	wi.g.x = 128;
	wi.text = "3";
	ghBtn_MenuSetup3 = gwinButtonCreate(0, &wi);	

	/* Button 4 */
	wi.g.x = 192;
	wi.text = "4";
	ghBtn_MenuSetup4 = gwinButtonCreate(0, &wi);	

	/* Button 5 */
	wi.g.x = 256;
	wi.text = "Return";
	ghBtn_MenuReturn = gwinButtonCreate(0, &wi);


    /* Call onShow manually once after init - place this always at the end of a page creation function */
    //if (menuPages[activePage].onShow)
    //    menuPages[activePage].onShow();
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
            if (be->gwin == ghBtn_MenuReturn) {
                Menu_ShowPage(PAGE_MAIN);
				LOG_MENU("RETURN pressed!");
                return true;
            }
            /*
            else if (be->gwin == ghBtn_Menu4) {
				//Menu_ShowPage(PAGE_ABOUT);
				//LOG_MENU("UTILS pressed!");
                return true;
            }
            */
            break;
        }

        default:
            break;
    }
    return false; // not handled
}
