/*
 * Copyright (c) 2012, 2013, Joel Bodenmann aka Tectu <joel@unormal.org>
 * Copyright (c) 2012, 2013, Andrew Hannam aka inmarket
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>

#include "gfx.h"

#include "gui.h"
#include "gui_router.h"

#include "helpers/date_and_time_util.h"

#ifdef UGFXSIMULATOR

	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <linux/input.h>

#else

	#include "calibration.h"

	#include "stm32f4xx_hal.h"
	#include "cmsis_os.h"
	#include "XCore407I.h"

#endif


#include "skin/zenstyle.h"
#include "pages/zen_menu.h"


/* The variables we need */
GListener glistener;

GHandle ghLabelPageTitle;

GHandle ghLabelClockTime;


// SHARED Fonts
font_t dejavu_sans_10;
font_t dejavu_sans_16;
font_t fixed_7x14;

/* SHARED IMAGES */
gdispImage ic_back;
gdispImage ic_cancel;
gdispImage ic_done;


static void create_PageTitle(void) {

	GWidgetInit		wi;

	gwinWidgetClearInit(&wi);

	wi.g.show = TRUE;
	wi.g.x = 5;
	wi.g.y = 1;
	wi.g.width = 235;
	wi.g.height = 23;

	wi.text = "";

	gwinSetDefaultBgColor(HTML2COLOR(0x262626));

	ghLabelPageTitle = gwinWindowCreate(0, (const struct GWindowInit *)&wi);

	// Must be called after handler initialization
	gwinSetBgColor(ghLabelPageTitle, HTML2COLOR(0x262626));
	gwinSetColor(ghLabelPageTitle, 	 White);
	
}


void gui_set_title(GUIWindow *win) {

	gwinFillStringBox(ghLabelPageTitle, 0, 0, 235, 24, win->text, justifyLeft);
	gwinRedraw(ghLabelPageTitle);

}


static void create_ClockTime(void) {

	GWidgetInit	wi;

	gwinWidgetClearInit(&wi);

	gwinSetDefaultFont(dejavu_sans_16);

	wi.g.show = TRUE;
	wi.g.x = 5;
	wi.g.y = 218;
	wi.g.width = 310;
	wi.g.height = 24;
	//wi.g.parent = ghContainer_Footer;
	wi.customDraw = gwinLabelDrawJustifiedRight;
	wi.customStyle = &footerstyle;

	//wi.text = "31.01.2018 12:00:00";
	wi.text = "31.01.2018 12:00";

	//compute_days_in_month();

	ghLabelClockTime = gwinLabelCreate(0, &wi);

}


void gui_set_time(GUIWindow *win) {

	(void) win;

	//gwinFillStringBox(ghLabelClockTime, 0, 0, 235, 24, "11. Dez 2017 21:36:42", justifyRight);
	//gwinRedraw(ghLabelClockTime);

}


void guiEventLoop(void) {


	GEvent *pe;

	/* WAITS for an Event - is blocking !!! */
	pe = geventEventWait(&glistener, 100);
	//pe = geventEventWait(&glistener, TIME_INFINITE);

	//update_time();

	/* No event, skip ... */
    if (!pe)
        return;

    /* Check for Event-Function on current page and execute it.  */
	if (curWindow && curWindow->onEvent)
		if (curWindow->onEvent(curWindow, pe))
			return;

#if 0
	/* If no Event-Function has been found for any page, perform the "default" button configuration */
	// GEventGWinButton  *peb = (GEventGWinButton *)pe;

	switch(pe->type) {

		case GEVENT_GWIN_BUTTON:

			//gwinPrintf(ghConsole, "Button %s\n", gwinGetText(((GEventGWinButton *)pe)->gwin));
			
			/* CONFIG */
			//if (peb->gwin == ghBtn_Config) {
			//	fprintf(stderr, "CONFIG:\n");
			//} 

			/* CLEAN */
			//else if (peb->gwin == ghBtn_Clean) {

            else if (peb->gwin == ghBtn_PageTwo)
                guiWindow_Show(&winMainMenuTwo);

			/* NEXT */
			else if (peb->gwin == ghBtn_PageOne) {
                guiWindow_Show(&winMainMenuOne);
				//fprintf(stderr, "Next page..\n");
			}
			break;

		default:
			break;
	}

#endif

}




#ifdef UGFXSIMULATOR
int main(void) {

	// Initialize the display (for simulator)
	gfxInit();

#else
void guiCreate(void) {
#endif	


	// Prepare fonts
	//dejavu_sans_10 = gdispOpenFont("DejaVuSans10");
	dejavu_sans_16 = gdispOpenFont("DejaVuSans16");
	fixed_7x14     = gdispOpenFont("Fixed7x14");

	// Set the widget defaults
	gwinSetDefaultFont(dejavu_sans_16);
	gwinSetDefaultStyle(&BlackWidgetStyle, FALSE);
	gdispClear(Black);

	// Create static border
	gdispFillArea(0, 0,   320, 24,  HTML2COLOR(0x262626) );
	gdispFillArea(0, 216, 320, 240, HTML2COLOR(0x262626) );

#if 1
	
	// Create images
	gdispImageOpenFile(&ic_settings,    "rsc/ic_settings.gif");
	gdispImageOpenFile(&ic_local_drink, "rsc/ic_local_drink.gif");
	gdispImageOpenFile(&ic_alarm,       "rsc/ic_alarm.gif");
	gdispImageOpenFile(&ic_public,      "rsc/ic_public.gif");
	
	gdispImageOpenFile(&ic_heart_pulse,	"rsc/ic_heart_pulse.gif");
	gdispImageOpenFile(&ic_search,      "rsc/ic_search.gif");
	gdispImageOpenFile(&ic_live_help,   "rsc/ic_live_help.gif");

	gdispImageOpenFile(&ic_fan,      	"rsc/ic_fan.gif");
	gdispImageOpenFile(&ic_membrane,    "rsc/ic_membrane.gif");
	gdispImageOpenFile(&ic_unfill,      "rsc/ic_unfill.gif");
	gdispImageOpenFile(&ic_aligntop,    "rsc/ic_aligntop.gif");
	gdispImageOpenFile(&ic_timelapse,   "rsc/ic_timelapse.gif");

	gdispImageOpenFile(&ic_date,      	"rsc/ic_date.gif");
	gdispImageOpenFile(&ic_time,      	"rsc/ic_time.gif");
	
	gdispImageOpenFile(&ic_back,      	"rsc/ic_back.gif");
	gdispImageOpenFile(&ic_done,      	"rsc/ic_done.gif");
	gdispImageOpenFile(&ic_cancel,      "rsc/ic_cancel.gif");
	gdispImageOpenFile(&ic_forward,     "rsc/ic_forward.gif");





	// gdispImageOpenFile(&ic_add,      		"rsc/ic_add.gif");
	// gdispImageOpenFile(&ic_remove,      	"rsc/ic_remove.gif");

#endif

	/* Main page HOME  */
	create_PageHome();

	/* Menu pages ONE */
	create_PageOne();

		create_PageConfig();
			create_PageDateConfig();
			create_PageTimeConfig();

		create_PageClean();

		create_PageTimers();

		create_PageNetwork();
			create_PageNetworkIpv4();
			create_PageNetworkGateway();
			create_PageNetworkSubnetMask();

		create_PageTestMode();

		create_PageStatus();

		create_PageHelp();

	/* Menu pages TWO */
	create_PageTwo();

		create_PageMembrane();

		create_PageT1T2();
			create_PageT1T2ConfigT1();
			create_PageT1T2ConfigT2();

		create_PageCycleMode();
		
		create_PageCycleTime();

		create_PageAeration();
			create_PageAerationAirOn(); 
			create_PageAerationAirOff(); 
			create_PageAerationA1();
			create_PageAerationA2(); 

		create_PageOverflow();
		
		create_PageSludge();

	

	// Create static block elements
	create_PageTitle();
	create_ClockTime();

	gui_set_time(NULL);


	//gui_create_lcd(8526);

	/* HOME ONE */
	winMainHome.onInit(&winMainHome, ghContainer_PageHome);

	/* PAGE ONE */
	winMainMenuOne.onInit(&winMainMenuOne, ghContainer_PageOne);
	
		winConfigMenu.onInit(&winConfigMenu, ghContainer_PageConfig);
			winDateConfigMenu.onInit(&winDateConfigMenu, ghContainer_PageDateConfig);
			winTimeConfigMenu.onInit(&winTimeConfigMenu, ghContainer_PageTimeConfig);

		winCleanMenu.onInit(&winCleanMenu, ghContainer_PageClean);

		winTimersMenu.onInit(&winTimersMenu, ghContainer_PageTimers);

		winNetworkMenu.onInit(&winNetworkMenu, ghContainer_PageNetwork);
			winNetworkIpv4Menu.onInit(&winNetworkIpv4Menu, ghContainer_PageNetworkIpv4);
			winNetworkGatewayMenu.onInit(&winNetworkGatewayMenu, ghContainer_PageNetworkGateway);
			winNetworkSubnetMaskMenu.onInit(&winNetworkSubnetMaskMenu, ghContainer_PageNetworkSubnetMask);

		winTestModeMenu.onInit(&winTestModeMenu, ghContainer_PageTestMode);

		winStatusMenu.onInit(&winStatusMenu, ghContainer_PageStatus);

		winHelpMenu.onInit(&winHelpMenu, ghContainer_PageHelp);


	/* PAGE TWO */
	winMainMenuTwo.onInit(&winMainMenuTwo, ghContainer_PageTwo);

		winMembraneMenu.onInit(&winMembraneMenu, ghContainer_PageMembrane);

		winT1T2Menu.onInit(&winT1T2Menu, ghContainer_PageT1T2);
			winT1T2ConfigT1Menu.onInit(&winT1T2ConfigT1Menu, ghContainer_PageT1T2ConfigT1);
			winT1T2ConfigT2Menu.onInit(&winT1T2ConfigT2Menu, ghContainer_PageT1T2ConfigT2);

		winCycleModeMenu.onInit(&winCycleModeMenu, ghContainer_PageCycleMode);

		winCycleTimeMenu.onInit(&winCycleTimeMenu, ghContainer_PageCycleTime);

		winAerationMenu.onInit(&winAerationMenu, ghContainer_PageAeration);
			winAerationAirOnMenu.onInit(&winAerationAirOnMenu, ghContainer_PageAerationAirOn);
			winAerationAirOffMenu.onInit(&winAerationAirOffMenu, ghContainer_PageAerationAirOff);
			winAerationA1Menu.onInit(&winAerationA1Menu, ghContainer_PageAerationA1);
			winAerationA2Menu.onInit(&winAerationA2Menu, ghContainer_PageAerationA2);

		winOverflowMenu.onInit(&winOverflowMenu, ghContainer_PageOverflow);

		winSludgeMenu.onInit(&winSludgeMenu, ghContainer_PageSludge);



    // We want to listen for widget events
	geventListenerInit(&glistener);
	gwinAttachListener(&glistener);


#ifndef UGFXSIMULATOR
	//gwinAttachDial(ghSliderADCvalue, 0 , 0);
#endif

	// Display initial window
	//guiWindow_Show(&winMainHome);
	guiWindow_Show(&winMainMenuOne);
	



/* For simulation purpose, we need a never-ending loop checking for the events here.
   On the embedded application, this is done by a task. */
#ifdef UGFXSIMULATOR

	while(1) {

		guiEventLoop();

		if (curWindow == &winMainHome) {
			//gui_create_lcd((uint16_t)gwinSliderGetPosition(ghSliderADCvalue));
		}

	}

	return 0;

#endif

}

