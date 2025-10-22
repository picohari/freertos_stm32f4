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

	#include <string.h>
	#include <stdbool.h>
	#include "linuxcnc_ctrl.h"
	#include "display_update.h"
	#include "encoder.h"
	#include "gwin_fastlabel.h"
	#include "streaming_test.h"
	#include "fast_numbers.h"
#endif

#include "gui_router.h"
#include "axis_gui.h"



/* The variables we need */
GListener glistener;


/* SHARED Fonts */
font_t ctrld_16b;
font_t neep_12x24b;


/* SHARED IMAGES */
//gdispImage ic_back;



/* Called from gui task in main.c on real hardware.
   In simulator, we call this function in local loop. */
void guiEventLoop(void) {

	/* Update all labels and values (only on real thing)*/
#ifndef UGFXSIMULATOR
	
	/* Fetch current machine data */
	MachineState_t machine_gui;
	LinuxCNC_GetState(&machine_gui);

    // Simple update call - no change detection needed!
    //Display_UpdateMachineState(&machine_gui, curWindow, ghFastLabelX, ghFastLabelY, ghFastLabelZ);

	// Draw X coordinate at position 200,50 (right edge)
    // "  123.456" in 8-character field, 3 decimals
    FastNumbers_DrawFloat(200, 50, machine_gui.pos_x, 3, 8, White, Black);
    
    // Draw Y coordinate
    FastNumbers_DrawFloat(200, 80, machine_gui.pos_y, 3, 8, White, Black);
    
    // Draw Z coordinate  
    FastNumbers_DrawFloat(200, 110, machine_gui.pos_z, 3, 8, White, Black);

	#if 0
	char coord_buffer[16];
	float x_coord = machine_gui.pos_x;
	snprintf(coord_buffer, sizeof(coord_buffer), "%0.3f", x_coord);
	gwinSetText(ghLabelG53_X, coord_buffer, 1);
	#endif

	#if 0
	static bool test_done = false;
    if (!test_done) {
        Test_RunAllStreamingTests();
        test_done = true;
    }
	#endif

	#if 1
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

#endif

	/* Handle all other events on gui */
	GEvent *pe;

	/* WAITS for an Event - is blocking !!! */
	pe = geventEventWait(&glistener, 100);

	/* No event, skip ... */
    if (!pe)
        return;

    /* Check for Event-Function on current page and execute it.  */
	if (curWindow && curWindow->onEvent)
		if (curWindow->onEvent(curWindow, pe))
			return;
}




#ifdef UGFXSIMULATOR
int main(void) {
	/* Initialize the display (for simulator)
	   On real hardware this is done inside task creation.
	*/
	gfxInit();
#else
	void guiCreate(void) {
#endif	


	/* Prepare fonts */
	ctrld_16b   = gdispOpenFont("ctrld_16b");
	neep_12x24b = gdispOpenFont("neep_12x24b");

	FastNumbers_SetFontSize(2);  // 2x scale = 10x14 pixels per char
	FastNumbers_SetSpacing(3);
	
	/* Create images */


	/* Create all widgets for home  */
	create_PageHome();

	/* Call onInit event of home */
	winMainHome.onInit(&winMainHome, ghContainer_PageHome);

	#if 0
	/* Initialize the display field mappings */
    GUI_InitDisplayFields(ghLabelG53_X, ghLabelG53_Y, ghLabelG53_Z, 
		NULL, NULL);  // Pass NULL if you don't have feed/spindle labels yet
	#endif

    /* We want to listen for widget events */
	geventListenerInit(&glistener);
	gwinAttachListener(&glistener);

	/* Display initial window */
	guiWindow_Show(&winMainHome);
	


/* For simulation purpose, we need a never-ending loop checking for the events here.
   On the embedded application, this is done by a task. */
#ifdef UGFXSIMULATOR

	while(1) {

		guiEventLoop();
	}

	return 0;

#endif

}

