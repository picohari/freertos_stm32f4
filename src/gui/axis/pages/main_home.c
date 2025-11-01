/* ============================================================================
 * File:        main_home.c
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

	#include "eth_if.h"
	#include "lwip/netif.h"

#endif

#include "style.h"
#include "gui_menu.h"
#include "gui_logger.h"


/* PAGE CONTAINER & WIDGETS */
GHandle ghCont_HomeHeader;
GHandle ghCont_HomeBody;
GHandle ghCont_HomeFooter;


/* ICONS */
GHandle ghIcon_Connection;
GHandle ghIcPowerOff;
GHandle ghIcPowerOn;


/* LABELS (static) */
GHandle ghLabelStatus;

/* LABELS (dynamic) */
GHandle ghLabelRotary;	


/* OTHER */


/* BUTTONS */
GHandle ghBtn_Menu1;
GHandle ghBtn_View;
GHandle ghBtn_Menu3;
GHandle ghBtn_Menu4;
GHandle ghBtn_Setup;


/* IMAGES */


/* OTHER STUFF */
GTimer gtBlinker;



/* Page Prototypes */
static void MainPage_onShow(void);
static void MainPage_onClose(void);
static bool MainPage_onEvent(MenuPageDef_t *page, GEvent *pe);
static void MainPage_onCycle(void);


/* Other prototypes */
//static void blinkTimer_callback(void* arg);


/* Create µGFX page */
void create_PageHome(void) {

	GWidgetInit		wi;
	gwinWidgetClearInit(&wi);

	gwinSetDefaultFont(ctrld_16b);

	/* Main home container - must be present on all pages if in menu tree */
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 320;
	wi.g.height = 240;
	wi.g.parent = 0;
	wi.text = "Home";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;

	/* Menu assignments */
	menuPages[PAGE_MAIN].title 		= wi.text;
	menuPages[PAGE_MAIN].container 	= gwinContainerCreate(0, &wi, 0);
    menuPages[PAGE_MAIN].id 		= PAGE_MAIN;
    menuPages[PAGE_MAIN].onShow 	= MainPage_onShow;
    menuPages[PAGE_MAIN].onClose 	= MainPage_onClose;
	menuPages[PAGE_MAIN].onEvent 	= MainPage_onEvent;
	menuPages[PAGE_MAIN].onCycle 	= MainPage_onCycle;


	/* Here comes the content for this page */

	/* Header container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 320;
	wi.g.height = 18;
	//wi.g.parent = ghCont_HomeMain;
	wi.g.parent = menuPages[PAGE_MAIN].container;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = &color_header;
	ghCont_HomeHeader = gwinContainerCreate(0, &wi, 0);

#if 1
	/* Status Label */
	wi.g.show = TRUE;
	wi.g.x = 2;
	wi.g.y = 1;
	wi.g.width  = 320;
	wi.g.height = 16;
	wi.g.parent = ghCont_HomeHeader;
	wi.text = "Idle";
	wi.customDraw = gwinLabelDrawJustifiedLeft;
	wi.customParam = 0;
	wi.customStyle = &color_header;
	ghLabelStatus = gwinLabelCreate(0, &wi);

	/* Connection Icon Widget */
	wi.g.show = TRUE;
	wi.g.x = 300;
	wi.g.y = 0;
	wi.g.width  = 18;
	wi.g.height = 18;
	wi.g.parent = ghCont_HomeHeader;
	wi.text = "";
	wi.customDraw = gwinLabelDrawJustifiedCenter;
	wi.customParam = 0;
	wi.customStyle = &color_header;
	ghIcon_Connection = gwinContainerCreate(0, &wi, 0);

	/* Draw Icon Power OFF */
	wi.g.show = TRUE;
	wi.g.x = 0; 
	wi.g.y = 0;
	wi.g.width  = 18;
	wi.g.height = 18;
	wi.g.parent = ghIcon_Connection;
	ghIcPowerOff = gwinImageCreate(0, &wi.g);
	gwinImageOpenFile(ghIcPowerOff, "ic_power_off.bmp");

	/* Draw Icon Power ON*/
	wi.g.show = FALSE;
	wi.g.x = 0; 
	wi.g.y = 0;
	wi.g.width = 18;
	wi.g.height = 18;
	wi.g.parent = ghIcon_Connection;
	ghIcPowerOn = gwinImageCreate(0, &wi.g);
	gwinImageOpenFile(ghIcPowerOn, "ic_power_on.bmp");
#endif /* Status label */


	/* Body container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 19;
	wi.g.width  = 320;
	wi.g.height = 178;
	//wi.g.parent = ghCont_HomeMain;
	wi.g.parent = menuPages[PAGE_MAIN].container;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = &color_black;
	ghCont_HomeBody = gwinContainerCreate(0, &wi, 0);


	/* Rotary encoder test label */
	wi.g.show = FALSE;
	wi.g.x = 5;
	wi.g.y = 100;
	wi.g.width  = 185;
	wi.g.height = 25;
	wi.g.parent = ghCont_HomeBody;
	wi.text = "0";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	ghLabelRotary = gwinLabelCreate(0, &wi);


	/* Footer container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 198;
	wi.g.width  = 320;
	wi.g.height = 42;				// change to 38 if borders are enabled
	wi.g.parent = menuPages[PAGE_MAIN].container;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghCont_HomeFooter = gwinContainerCreate(0, &wi, 0);

	/* Default parameters for buttons */
	gwinWidgetClearInit(&wi);

	wi.g.show = gTrue;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 65;
	wi.g.height = 42;
	wi.g.parent = ghCont_HomeFooter;
	wi.customStyle = &color_buttons;

	/* Button 1 */
	wi.g.x = 0;
	wi.text = "Move";
	ghBtn_Menu1 = gwinButtonCreate(0, &wi);

	/* Button 2 */
	wi.g.x = 64;
	wi.text = "View";
	ghBtn_View = gwinButtonCreate(0, &wi);	

	/* Button 3 */
	wi.g.x = 128;
	wi.text = "CMD";
	ghBtn_Menu3 = gwinButtonCreate(0, &wi);	

	/* Button 4 */
	wi.g.x = 192;
	wi.text = "Utils";
	ghBtn_Menu4 = gwinButtonCreate(0, &wi);	

	/* Button 5 */
	wi.g.x = 256;
	wi.text = "Setup";
	ghBtn_Setup = gwinButtonCreate(0, &wi);

}





static void blinkTimer_callback(void* arg)
{
    (void)arg;
 
    if (gwinGetVisible(ghIcPowerOff) == TRUE) {
		gwinHide(ghIcPowerOff);
	} else {
		gwinShow(ghIcPowerOff);
	}
}


static void MainPage_onShow(void) {
	LOG_MENU("MAIN: onShow");
	gtimerInit(&gtBlinker);
	gtimerStart(&gtBlinker, blinkTimer_callback, NULL, TRUE, 500);
}


static void MainPage_onClose(void) {
	LOG_MENU("MAIN: onClose");
	gtimerStop(&gtBlinker);
	gtimerDeinit(&gtBlinker);
}


static bool MainPage_onEvent(MenuPageDef_t *page, GEvent *pe) {
    (void)page; // not used yet

	/* Handle button events first */
    switch (pe->type) {
		
        case GEVENT_GWIN_BUTTON: {
            GEventGWinButton *be = (GEventGWinButton *)pe;
            if (be->gwin == ghBtn_Setup) {
                Menu_ShowPage(MENU_SETUP);
				//LOG_MENU("SETTINGS pressed!");
                return TRUE;
            }
            else if (be->gwin == ghBtn_View) {
				Menu_ShowPage(VIEW_IOSTATE);
				//LOG_MENU("UTILS pressed!");
                return TRUE;
            }
            break;
        }

        default:
            break;
    }

    return false; // not handled
}


static void MainPage_onCycle(void) {

#ifndef UGFXSIMULATOR
	/* Other events and interactive functions */
	if (netif_is_link_up(&gnetif)) {
		if (gtimerIsActive(&gtBlinker)) {
			gtimerStop(&gtBlinker);
		}
		gwinHide(ghIcPowerOff);
		gwinShow(ghIcPowerOn);
	} else {
		if (!gtimerIsActive(&gtBlinker)) {
			gtimerStart(&gtBlinker, blinkTimer_callback, NULL, TRUE, 500);
		}
		gwinHide(ghIcPowerOn);
	}
#endif
}
