/* ============================================================================
 * File:        {{FILE_NAME}}.c
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
static GHandle ghCont_{{PAGE_NAME}}Header;
static GHandle ghCont_{{PAGE_NAME}}Body;
static GHandle ghCont_{{PAGE_NAME}}Footer;


/* BUTTONS */
static GHandle ghBtn_MenuSetup1;
static GHandle ghBtn_MenuSetup2;
static GHandle ghBtn_MenuSetup3;
static GHandle ghBtn_MenuSetup4;
static GHandle ghBtn_MenuReturn;

/* Page Prototypes */
static void {{MENU_TYPE}}{{PAGE_NAME}}_onShow(void);
static void {{MENU_TYPE}}{{PAGE_NAME}}_onClose(void);
static bool {{MENU_TYPE}}{{PAGE_NAME}}_onEvent(MenuPageDef_t *page, GEvent *pe);
static void {{MENU_TYPE}}{{PAGE_NAME}}_onCycle(void);


/*

{{PAGE_NAME}}
Top level menu pages names (suffix to file):
eg. Home, Setup, View, IPv4, 

{{MENU_TYPE}}
Page/Menu type or category - kind of character description (Prefix to file):
eg. Page, Menu, Config

{{PAGE_NAME_UPPER}}
Pagename from Menusystem - member of enum MenuPage_t (see gui_menu.h)
Concatenation of MENU_TYPE + _ + PAGE_NAME


*/




/* Create µGFX page */
void create_{{MENU_TYPE}}{{PAGE_NAME}}(void) {

	GWidgetInit		wi;
	gwinWidgetClearInit(&wi);

	gwinSetDefaultFont(ctrld_16b);

    /* Menu{{PAGE_NAME}} container - must be present on all pages if in menu tree */
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 320;
	wi.g.height = 240;
	wi.g.parent = 0;
	wi.text = "{{MENU_TYPE}}: {{PAGE_NAME}}";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;

	/* Menu assignments */
	menuPages[{{PAGE_NAME_UPPER}}].title 	= wi.text;
	menuPages[{{PAGE_NAME_UPPER}}].container = gwinContainerCreate(0, &wi, 0);
    menuPages[{{PAGE_NAME_UPPER}}].id 		= {{PAGE_NAME_UPPER}};
    menuPages[{{PAGE_NAME_UPPER}}].onShow 	= {{MENU_TYPE}}{{PAGE_NAME}}_onShow;
    menuPages[{{PAGE_NAME_UPPER}}].onClose 	= {{MENU_TYPE}}{{PAGE_NAME}}_onClose;
	menuPages[{{PAGE_NAME_UPPER}}].onEvent 	= {{MENU_TYPE}}{{PAGE_NAME}}_onEvent;
	menuPages[{{PAGE_NAME_UPPER}}].onCycle 	= {{MENU_TYPE}}{{PAGE_NAME}}_onCycle;


	/* Here comes the content for this page */





	/* Footer container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 198;
	wi.g.width  = 320;
	wi.g.height = 42;				// change to 38 if borders are enabled
	//wi.g.parent = ghCont_HomeMain;
	wi.g.parent = menuPages[{{PAGE_NAME_UPPER}}].container;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghCont_{{PAGE_NAME}}Footer = gwinContainerCreate(0, &wi, 0);

	/* Default parameters for buttons */
	gwinWidgetClearInit(&wi);

	wi.g.show = gTrue;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 65;
	wi.g.height = 42;
	wi.g.parent = ghCont_{{PAGE_NAME}}Footer;
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
	wi.text = "BACK";
	ghBtn_MenuReturn = gwinButtonCreate(0, &wi);

}



static void {{MENU_TYPE}}{{PAGE_NAME}}_onShow(void) {
    LOG_MENU("{{PAGE_NAME_UPPER}}: onShow");
}


static void {{MENU_TYPE}}{{PAGE_NAME}}_onClose(void) {
    LOG_MENU("{{PAGE_NAME_UPPER}}: onClose");
}


static bool {{MENU_TYPE}}{{PAGE_NAME}}_onEvent(MenuPageDef_t *page, GEvent *pe) {
    (void)page; // not used yet

    switch (pe->type) {
		
        case GEVENT_GWIN_BUTTON: {
            GEventGWinButton *be = (GEventGWinButton *)pe;
            if (be->gwin == ghBtn_MenuReturn) {
                Menu_ShowPage({{RETURN_PAGE}});
				LOG_MENU("RETURN pressed!");
                return TRUE;
            }
            /*
            else if (be->gwin == ghBtn_Menu4) {
				//Menu_ShowPage(PAGE_ABOUT);
				//LOG_MENU("UTILS pressed!");
                return TRUE;
            }
            */
            break;
        }

        default:
            break;
    }
	
    return false; // not handled
}


static void {{MENU_TYPE}}{{PAGE_NAME}}_onCycle(void) {

	#ifndef UGFXSIMULATOR
	
	#endif
	}