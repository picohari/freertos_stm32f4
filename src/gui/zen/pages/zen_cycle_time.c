#include <stdio.h>

#include "gfx.h"
#include "gui.h"
#include "math.h"

#ifdef UGFXSIMULATOR

	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	//#include <linux/input.h>

#else

	#include "stm32f4xx_hal.h"
	#include "cmsis_os.h"
	#include "XCore407I.h"

#endif

#include "zen_menu.h"
#include "skin/zenstyle.h"
#include "gui_router.h"

#include "pages/zen_cycle_time.h"
#include "pages/zen_main_two.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageCycleTime;


/* BUTTONS */
GHandle ghBtn_BackCycleTime;

/* IMAGES */


// A set of data points that will be displayed in the graph
static const point data[5] = {
    { -40, -40 },
    { 70, 40 },
    { 140, 60 },
    { 210, 60 },
    { 280, 200 }
};
 
// The graph object
/*
static GGraphObject g;
 
// A graph styling
static GGraphStyle GraphStyle1 = {
    { GGRAPH_POINT_DOT, 0, Blue },          // Point
    { GGRAPH_LINE_NONE, 2, Gray },          // Line
    { GGRAPH_LINE_SOLID, 0, White },        // X axis
    { GGRAPH_LINE_SOLID, 0, White },        // Y axis
    { GGRAPH_LINE_DASH, 5, Gray, 50 },      // X grid
    { GGRAPH_LINE_DOT, 7, Yellow, 50 },     // Y grid
    GWIN_GRAPH_STYLE_POSITIVE_AXIS_ARROWS   // Flags
};
 
// Another graph styling 
static const GGraphStyle GraphStyle2 = {
    { GGRAPH_POINT_SQUARE, 5, Red },        // Point
    { GGRAPH_LINE_DOT, 2, Pink },           // Line
    { GGRAPH_LINE_SOLID, 0, White },        // X axis
    { GGRAPH_LINE_SOLID, 0, White },        // Y axis
    { GGRAPH_LINE_DASH, 5, Gray, 50 },      // X grid
    { GGRAPH_LINE_DOT, 7, Yellow, 50 },     // Y grid
    GWIN_GRAPH_STYLE_POSITIVE_AXIS_ARROWS   // Flags
};

GHandle gh;
*/

void create_PageCycleTime(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageCycleTime
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 24;
	wi.g.width = 320;
	wi.g.height = 192;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainer_PageCycleTime = gwinContainerCreate(0, &wi, 0);
	
	
	// create button widget: ghBtn_BackCycleTime
	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageCycleTime;
	wi.text = MENU_TITLE_BACK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_back;
	wi.customStyle = &color_three;
	ghBtn_BackCycleTime = gwinButtonCreate(0, &wi);


	/*
	wi.g.show = TRUE;
	wi.g.x = 10;
	wi.g.y = 10;
	wi.g.width = 300;
	wi.g.height = 160;
	wi.g.parent = ghContainer_PageCycleTime;	// Specify the parent container
	gh = gwinGraphCreate(0, &wi.g);

	// Set the graph origin and style
    gwinGraphSetOrigin(gh, gwinGetWidth(gh)/2, gwinGetHeight(gh)/2);
    gwinGraphSetStyle(gh, &GraphStyle1);
    gwinGraphDrawAxis(gh);
 
    // Draw a sine wave
    for(int i = 0; i < gwinGetWidth(gh); i++) {
        gwinGraphDrawPoint(gh, i - gwinGetWidth(gh)/2, 80*sin(2*0.2*M_PI*i/180));
    }
 
    // Modify the style
    gwinGraphStartSet(gh);
    GraphStyle1.point.color = Green;
    gwinGraphSetStyle(gh, &GraphStyle1);
	*/
	
}


static void guiCycleTimeMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

}

static void guiCycleTimeMenu_onClose(GUIWindow *win) {

	(void) win;

}

static int guiCycleTimeMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_BackCycleTime) {
            	guiWindow_Show(&winMainMenuTwo);

            } else {
            	return 0;
            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winCycleTimeMenu = {

/* Title   */	 "Cycle Time Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiCycleTimeMenu_onShow,
/* onClose */    guiCycleTimeMenu_onClose,
/* onEvent */    guiCycleTimeMenu_handleEvent,
/* handle  */    0

};