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


#include "pages/zen_menu.h"




/* The variables we need */
GListener glistener;

GHandle ghLabelPageTitle;

GHandle ghLabelClockTime;

// Fonts
font_t dejavu_sans_10;
font_t dejavu_sans_16;
font_t fixed_7x14;



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

	GWidgetInit		wi;

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

	wi.text = "11.12.1974 16:25:42";

	ghLabelClockTime = gwinLabelCreate(0, &wi);

}


void gui_set_time(GUIWindow *win) {

	(void) win;

	//gwinFillStringBox(ghLabelClockTime, 0, 0, 235, 24, "11. Dez 2017 21:36:42", justifyRight);
	//gwinRedraw(ghLabelClockTime);

}










#ifdef UGFXSIMULATOR
void myguiEventLoop(void) {
#else
void guiEventLoop(void) {
#endif

	GEvent *pe;

	/* WAITS for an Event - is blocking !!! */
	pe = geventEventWait(&glistener, 100);
	//pe = geventEventWait(&glistener, TIME_INFINITE);

	/* No event, skip ... */
    if (!pe)
        return;

    /* Check for Event-Function on current page and execute it.  */
	if (curWindow && curWindow->onEvent)
		if (curWindow->onEvent(curWindow, pe))
			return;

	/* If no Event-Function has been found for any page, perform the "default" button configuration */
	GEventGWinButton  *peb = (GEventGWinButton *)pe;

	switch(pe->type) {

		case GEVENT_GWIN_BUTTON:

			//gwinPrintf(ghConsole, "Button %s\n", gwinGetText(((GEventGWinButton *)pe)->gwin));
			
			/* CONFIG */
			if (peb->gwin == ghBtn_Config) {
				fprintf(stderr, "CONFIG:\n");
			} 

			/* CLEAN */
			else if (peb->gwin == ghBtn_Clean) {
				fprintf(stderr, "CLEAN:\n");
			}

#if 0
            else if (peb->gwin == ghBtn_PageTwo)
                guiWindow_Show(&winMainMenuTwo);

			/* NEXT */
			else if (peb->gwin == ghBtn_PageOne) {
                guiWindow_Show(&winMainMenuOne);
				//fprintf(stderr, "Next page..\n");
			}
#endif
			break;

		default:
			break;
	}

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
	gdispImageOpenFile(&ic_settings,       "rsc/ic_settings.gif");
	gdispImageOpenFile(&ic_local_drink,    "rsc/ic_local_drink.gif");
	gdispImageOpenFile(&ic_alarm,          "rsc/ic_alarm.gif");
	gdispImageOpenFile(&ic_public,         "rsc/ic_public.gif");
	gdispImageOpenFile(&ic_heart_pulse,	   "rsc/ic_heart_pulse.gif");
	gdispImageOpenFile(&ic_search,         "rsc/ic_search.gif");
	gdispImageOpenFile(&ic_live_help,      "rsc/ic_live_help.gif");
	gdispImageOpenFile(&ic_forward,        "rsc/ic_forward.gif");

	gdispImageOpenFile(&ic_fan,      	   "rsc/ic_fan.gif");
	gdispImageOpenFile(&ic_membrane,       "rsc/ic_membrane.gif");
	gdispImageOpenFile(&ic_unfill,         "rsc/ic_unfill.gif");
	gdispImageOpenFile(&ic_aligntop,       "rsc/ic_aligntop.gif");
	gdispImageOpenFile(&ic_timelapse,      "rsc/ic_timelapse.gif");
#endif

	// Create the menu pages
	create_PageHome();
	create_PageOne();
	create_PageTwo();
	
	// Create static block elements
	create_PageTitle();
	create_ClockTime();

	gui_set_time(NULL);


	//gui_create_lcd(8526);



	// Init menu items
	winMainHome.onInit(&winMainHome, 		ghContainer_PageHome);
	winMainMenuOne.onInit(&winMainMenuOne, 	ghContainer_PageOne);
	winMainMenuTwo.onInit(&winMainMenuTwo, 	ghContainer_PageTwo);

    // We want to listen for widget events
	geventListenerInit(&glistener);
	gwinAttachListener(&glistener);

#ifndef UGFXSIMULATOR
	gwinAttachDial(ghSliderADCvalue, 0 , 0);
#endif

	// Display initial window
	guiWindow_Show(&winMainHome);
	//guiWindow_Show(&winMainMenuOne);




#ifdef UGFXSIMULATOR

	#if 1

	while(1) {

		myguiEventLoop();

		if (curWindow == &winMainHome) {
			gui_create_lcd((uint16_t)gwinSliderGetPosition(ghSliderADCvalue));
		}

	}

	#endif
	   
	return 0;

#endif

}

