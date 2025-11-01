/* ============================================================================
 * File:        view_iostate.c
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

	#include "linuxcnc_ctrl.h"
#endif

#include "style.h"
#include "gui_menu.h"
#include "gui_logger.h"

#include "gfx_led.h"


/* PAGE CONTAINER & WIDGETS */
static GHandle ghCont_IOstateHeader;
static GHandle ghCont_IOstateBody;
static GHandle ghCont_IOstateFooter;


/* BUTTONS */
static GHandle ghBtn_MenuSetup1;
static GHandle ghBtn_MenuSetup2;
static GHandle ghBtn_MenuSetup3;
static GHandle ghBtn_MenuSetup4;
static GHandle ghBtn_MenuReturn;

/* Page Prototypes */
static void PageIOstate_onShow(void);
static void PageIOstate_onClose(void);
static bool PageIOstate_onEvent(MenuPageDef_t *page, GEvent *pe);
static void PageIOstate_onCycle(void);

static GHandle ghLabelLedIO;
static GHandle ghLed1;
static GHandle ghLed2;
static GHandle ghLed3;
static GHandle ghLed4;
static GHandle ghLed5;
static GHandle ghLed6;
static GHandle ghLed7;
static GHandle ghLed8;

static GHandle ghLabelBtnIO;
static GHandle ghBtn_IO1;
static GHandle ghBtn_IO2;
static GHandle ghBtn_IO3;
static GHandle ghBtn_IO4;
static GHandle ghBtn_IO5;
static GHandle ghBtn_IO6;
static GHandle ghBtn_IO7;
static GHandle ghBtn_IO8;

static gdispImage _imgDialFace;

static GHandle ghLabelCTRL;
static GHandle ghCTRLstatus;







/* Create µGFX page */
void create_PageIOstate(void) {

	GWidgetInit		wi;
	gwinWidgetClearInit(&wi);

	gwinSetDefaultFont(ctrld_16b);

    /* MenuIOstate container - must be present on all pages if in menu tree */
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 320;
	wi.g.height = 240;
	wi.g.parent = 0;
	wi.text = "Page: IOstate";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;

	/* Menu assignments */
	menuPages[VIEW_IOSTATE].title 	  = wi.text;
	menuPages[VIEW_IOSTATE].container = gwinContainerCreate(0, &wi, 0);
    menuPages[VIEW_IOSTATE].id 		  = VIEW_IOSTATE;
    menuPages[VIEW_IOSTATE].onShow 	  = PageIOstate_onShow;
    menuPages[VIEW_IOSTATE].onClose   = PageIOstate_onClose;
	menuPages[VIEW_IOSTATE].onEvent   = PageIOstate_onEvent;
	menuPages[VIEW_IOSTATE].onCycle   = PageIOstate_onCycle;


	/* Here comes the content for this page */

	/* IO Label */
	uint8_t spacing = 34;
	wi.g.show = TRUE;
	wi.g.x = 3;
	wi.g.y = 20;
	wi.g.width  = 40;
	wi.g.height = 20;
	wi.g.parent = menuPages[VIEW_IOSTATE].container;
	wi.text = "IO:";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	ghLabelLedIO = gwinLabelCreate(0, &wi);
	
	/* IO Leds */
	wi.g.show = TRUE;
	wi.customDraw = 0;
	wi.g.parent = menuPages[VIEW_IOSTATE].container;
	wi.g.width = 20;
	wi.g.height = 20;
	wi.g.x = 20;
	wi.g.y = 20;
	
	wi.g.x = wi.g.x + spacing;
	wi.text = "Led1";
	ghLed1 = ledCreate(0, &wi, &_imgDialFace, GFX_WHITE);
	wi.g.x = wi.g.x + spacing;
	wi.text = "Led2";
	ghLed2 = ledCreate(0, &wi, &_imgDialFace, GFX_GREEN);
	wi.g.x = wi.g.x + spacing;
	wi.text = "Led3";
	ghLed3 = ledCreate(0, &wi, &_imgDialFace, GFX_ORANGE);
	wi.g.x = wi.g.x + spacing;
	wi.text = "Led4";
	ghLed4 = ledCreate(0, &wi, &_imgDialFace, GFX_RED);
	wi.g.x = wi.g.x + spacing;
	wi.text = "Led5";
	ghLed5 = ledCreate(0, &wi, &_imgDialFace, GFX_PURPLE);
	wi.g.x = wi.g.x + spacing;
	wi.text = "Led6";
	ghLed6 = ledCreate(0, &wi, &_imgDialFace, GFX_BLUE);
	wi.g.x = wi.g.x + spacing;
	wi.text = "Led7";
	ghLed7 = ledCreate(0, &wi, &_imgDialFace, GFX_NAVY);
	wi.g.x = wi.g.x + spacing;
	wi.text = "Led8";
	ghLed8 = ledCreate(0, &wi, &_imgDialFace, GFX_GRAY);

	ledSetState(ghLed1, TRUE);
	ledSetState(ghLed2, TRUE);
	ledSetState(ghLed3, TRUE);
	ledSetState(ghLed4, TRUE);
	ledSetState(ghLed5, TRUE);
	ledSetState(ghLed6, TRUE);
	ledSetState(ghLed7, TRUE);
	ledSetState(ghLed8, TRUE);

	/* CONTROL Label */
	spacing = 34;
	wi.g.show = TRUE;
	wi.g.x = 3;
	wi.g.y = 50;
	wi.g.width  = 40;
	wi.g.height = 30;
	wi.g.parent = menuPages[VIEW_IOSTATE].container;
	wi.text = "BTN:";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	ghLabelBtnIO = gwinLabelCreate(0, &wi);
	
	wi.g.x = 49;
	wi.g.width  = 30;
	wi.g.height = 30;
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = &color_buttons;
	//wi.customDraw = gwinLabelDrawJustifiedCenter;
	wi.text = "Off";
	ghBtn_IO1 = gwinButtonCreate(0, &wi);
	
	wi.g.x = wi.g.x + spacing;
	ghBtn_IO2 = gwinButtonCreate(0, &wi);
	wi.g.x = wi.g.x + spacing;
	ghBtn_IO3 = gwinButtonCreate(0, &wi);
	wi.g.x = wi.g.x + spacing;
	ghBtn_IO4 = gwinButtonCreate(0, &wi);
	wi.g.x = wi.g.x + spacing;
	ghBtn_IO5 = gwinButtonCreate(0, &wi);
	wi.g.x = wi.g.x + spacing;
	ghBtn_IO6 = gwinButtonCreate(0, &wi);
	wi.g.x = wi.g.x + spacing;
	ghBtn_IO7 = gwinButtonCreate(0, &wi);
	wi.g.x = wi.g.x + spacing;
	ghBtn_IO8 = gwinButtonCreate(0, &wi);


















	/* Footer container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 198;
	wi.g.width  = 320;
	wi.g.height = 42;				// change to 38 if borders are enabled
	//wi.g.parent = ghCont_HomeMain;
	wi.g.parent = menuPages[VIEW_IOSTATE].container;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghCont_IOstateFooter = gwinContainerCreate(0, &wi, 0);

	/* Default parameters for buttons */
	gwinWidgetClearInit(&wi);

	wi.g.show = gTrue;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width  = 65;
	wi.g.height = 42;
	wi.g.parent = ghCont_IOstateFooter;
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



static void PageIOstate_onShow(void) {
    LOG_MENU("VIEW_IOSTATE: onShow");
}


static void PageIOstate_onClose(void) {
    LOG_MENU("VIEW_IOSTATE: onClose");
}


static bool PageIOstate_onEvent(MenuPageDef_t *page, GEvent *pe) {
    (void)page; // not used yet

    switch (pe->type) {
		
        case GEVENT_GWIN_BUTTON: {
            GEventGWinButton *be = (GEventGWinButton *)pe;
            if (be->gwin == ghBtn_MenuReturn) {
                Menu_ShowPage(PAGE_MAIN);
				//LOG_MENU("RETURN pressed!");
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

static void PageIOstate_onCycle(void) {

#ifndef UGFXSIMULATOR

	/* Current HAL state */
	HalState_t     hal_gui;
	LinuxCNC_GetHalState(&hal_gui);

	hal_gui.fb.io & (1 << 0) ? ledSetState(ghLed8, TRUE) : ledSetState(ghLed8, FALSE);
	hal_gui.fb.io & (1 << 1) ? ledSetState(ghLed7, TRUE) : ledSetState(ghLed7, FALSE);
	hal_gui.fb.io & (1 << 2) ? ledSetState(ghLed6, TRUE) : ledSetState(ghLed6, FALSE);
	hal_gui.fb.io & (1 << 3) ? ledSetState(ghLed5, TRUE) : ledSetState(ghLed5, FALSE);
	hal_gui.fb.io & (1 << 4) ? ledSetState(ghLed4, TRUE) : ledSetState(ghLed4, FALSE);
	hal_gui.fb.io & (1 << 5) ? ledSetState(ghLed3, TRUE) : ledSetState(ghLed3, FALSE);
	hal_gui.fb.io & (1 << 6) ? ledSetState(ghLed2, TRUE) : ledSetState(ghLed2, FALSE);
	hal_gui.fb.io & (1 << 7) ? ledSetState(ghLed1, TRUE) : ledSetState(ghLed1, FALSE);

#endif
}