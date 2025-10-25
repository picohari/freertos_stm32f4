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

//#include <stdio.h>
#include <stdbool.h>

#include "gfx.h"
#include "gui.h"

#include "gui_menu.h"
#include "axis_menu.h"

#ifdef UGFXSIMULATOR
	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	#include <linux/input.h>

	#include "emulator.h"		/* Here we place all dummy code required only for simulation */
#else
	#include "calibration.h"
	#include "stm32f4xx_hal.h"
	#include "cmsis_os.h"
	#include "XCore407I.h"

	#include <string.h>
	#include <stdbool.h>
	#include "linuxcnc_ctrl.h"
	#include "encoder.h"
	#include "fast_text.h"
	//#include "display_update.h"
	//#include "streaming_test.h"
	//#include "gwin_fastlabel.h"
#endif



/* The variables we need */
GListener glistener;


/* SHARED Fonts */
font_t ctrld_16b;
font_t neep_12x24b;
font_t term_12b;

/* SHARED IMAGES */
//gdispImage ic_back;



/* Called from gui task in main.c on real hardware.
   In simulator, we call this function in local loop. */
void guiEventLoop(void) {

	
	/* Update all fast rendering values (only on real thing) */
#ifndef UGFXSIMULATOR

	/* Fetch current machine data - anyway of current page being rendered */
	MachineState_t machine_gui;
	LinuxCNC_GetState(&machine_gui);

	/* Show realtime info only on main page */
	if (Menu_GetActive() == PAGE_MAIN) {
	
		/* G53 Absolute position in large font */
		FastText_SetScale(2);
		
		/* Draw labels (left-aligned) */
		FastText_DrawString(5,  25, "X:", White, Black);
		FastText_DrawString(5,  55, "Y:", White, Black);
		FastText_DrawString(5,  85, "Z:", White, Black);
		
		/* Draw XYZ coordinate at position 200,50 (right edge) */
		// "  -1234.567" in 9-character field, 3 decimals
		FastText_DrawFloat(190, 25, machine_gui.G53_x, 3, 9, White, Black);
		FastText_DrawFloat(190, 55, machine_gui.G53_y, 3, 9, White, Black);
		FastText_DrawFloat(190, 85, machine_gui.G53_z, 3, 9, White, Black);
		
		//FastText_SetScale(1);
		//FastText_DrawString(195, 28, "ABS", White, Black);
	}

#endif
	
	
	
	
	#if 0
	/* Streaming tests */
	static bool test_done = false;
    if (!test_done) {
        Test_RunAllStreamingTests();
        test_done = true;
		}
	#endif

	#if 0
    /* Update rotary encoder */
    static float last_rotary = 0.0f;
    float current_rotary = Encoder_GetPosition();
    if (current_rotary != last_rotary) {
        char temp_buffer[16];
        snprintf(temp_buffer, sizeof(temp_buffer), "%0.1f", current_rotary);
        gwinSetText(ghLabelRotary, temp_buffer, 1);
        last_rotary = current_rotary;
		}
	#endif
	
	
	/* EVENT Handling */
	
	/* Handle all other events on gui */
	GEvent *pe;
	
	/* WAITS for an Event - is blocking !!! */
	pe = geventEventWait(&glistener, gDelayNone);
	
	/* No event, skip ... */
    if (!pe)
		return;

	/* Check if the active page changed externally (rare but possible)
	if (activePage != lastActive) {
		if (menuPages[activePage].onShow) {
			menuPages[activePage].onShow();
		}
		lastActive = activePage;
	}
	*/
	
	/* Dispatch to current page handler if available */
	if (menuPages[activePage].onEvent) {
		/* Handle Event-Function on current page and execute it. */
		bool handled = menuPages[activePage].onEvent(&menuPages[activePage], pe);
		/* if (handled) return; // event fully processed by this page */
	}

	/* Optional: global event handling here */
	switch (pe->type) {
		case GEVENT_GWIN_BUTTON:
			//LOG_DEBUG("Unhandled button event.");
			break;

		default:
			break;
	}

}



#ifdef UGFXSIMULATOR
int main(void) {
	/* Initialize the display (for simulator)
	   On real hardware this is done inside task creation.
	*/
	gfxInit();
#else
	/* Start creating all windows and widgets for GUI */
void guiCreate(void) {
#endif	

	/* Prepare fonts */
	ctrld_16b   = gdispOpenFont("ctrld_16b");
	neep_12x24b = gdispOpenFont("neep_12x24b");
	term_12b    = gdispOpenFont("term_12b");

	
	/* Create images */


	/* Create all widgets for home (all hidden first!)*/
	create_PageHome();
	create_MenuSetup();
	
	
    /* We want to listen for widget events */
	geventListenerInit(&glistener);
	gwinAttachListener(&glistener);
	

	/* Display initial window first - should be called *after* pages creation! */
	Menu_Init(PAGE_MAIN);



/* For simulation purpose, we need a never-ending loop checking for the events here.
   On the embedded application, this is done by a task. */
#ifdef UGFXSIMULATOR
	while(1) {
		guiEventLoop();
	}
	return 0;	// Never get here in simulator
#endif
}

