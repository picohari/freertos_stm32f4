
#include <stdio.h>

#include "gfx.h"
#include "gui.h"

#ifdef UGFXSIMULATOR

	#include <stdio.h>
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

	//#include "adc.h"
#endif

#include "zen_menu.h"
#include "skin/zenstyle.h"
#include "gui_router.h"

#include "pages/zen_menu.h"



/* PAGE CONTAINER */
GHandle ghContainer_PageHome;

GHandle ghLabelADCtitle;
GHandle ghLabelADCvalue;
GHandle ghLabelADCunit;
GHandle ghLabelADCindicOK;
GHandle ghLabelADCindicFAIL;

GHandle ghSliderADCvalue;
GHandle ghScaleADCvalue;

/* BUTTONS */
GHandle ghBtn_MENU;

/* IMAGES */


void create_PageHome(void) {

	GWidgetInit		wi;

	gwinWidgetClearInit(&wi);

	gwinSetDefaultFont(dejavu_sans_16);

	/* Base container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 24;
	wi.g.width = 320;
	wi.g.height = 192;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainer_PageHome = gwinContainerCreate(0, &wi, 0);

	/* Page container */
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 320;
	wi.g.height = 192;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "Container";
	wi.customDraw = gwinButtonDraw_Clear;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghBtn_MENU = gwinButtonCreate(0, &wi);

	/* Label */
	wi.g.show = TRUE;
	wi.g.x = 5;
	wi.g.y = 5;
	wi.g.width = 145;
	wi.g.height = 24;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "Pressure ADC:";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelADCtitle = gwinLabelCreate(0, &wi);

	/* Value*/
	wi.g.show = FALSE;
	wi.g.x = 160;
	wi.g.y = 5;
	wi.g.width = 70;
	wi.g.height = 24;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "136.23";
	wi.customDraw = gwinLabelDrawJustifiedRight;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelADCvalue = gwinLabelCreate(0, &wi);

	/* Unit */
	wi.g.show = TRUE;
	wi.g.x = 240;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 24;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "bar";
	wi.customDraw = gwinLabelDrawJustifiedLeft;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelADCunit = gwinLabelCreate(0, &wi);

	/* OK box on right side */
	gwinSetDefaultFont(fixed_7x14);
	gwinSetDefaultBgColor(Green);
	gwinSetDefaultColor(White);

	wi.g.show = FALSE;
	wi.g.x = 294;
	wi.g.y = 8;
	wi.g.width = 18;
	wi.g.height = 16;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelADCindicOK = gwinWindowCreate(0, (const struct GWindowInit *)&wi);
	gwinFillString(ghLabelADCindicOK, 2, 1, "OK");


	/* ERR box on right side */
	gwinSetDefaultFont(fixed_7x14);
	gwinSetDefaultBgColor(Red);
	gwinSetDefaultColor(White);

	wi.g.show = FALSE;
	wi.g.x = 290;
	wi.g.y = 8;
	wi.g.width = 25;
	wi.g.height = 16;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghLabelADCindicFAIL = gwinWindowCreate(0, (const struct GWindowInit *)&wi);


	/* Bar gauge */
	wi.g.show = TRUE;
	wi.g.x = 5;
	wi.g.y = 30;
	wi.g.width = 310;
	wi.g.height = 4;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "";
	wi.customDraw = gwinSliderDraw_Bars;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghSliderADCvalue = gwinSliderCreate(0, &wi);

#ifndef UGFXSIMULATOR
	gwinSliderSetRange(ghSliderADCvalue, 0, 1024);
	ginputSetDialRange(0, 1024);
	//gwinSliderSetPosition(ghSliderADCvalue, 75);
#endif

	gwinSetDefaultBgColor(Red);
	gwinSetDefaultColor(White);


#if 1
	wi.g.show = FALSE;
	wi.g.x = 5;
	wi.g.y = 48;
	wi.g.width = 117;
	wi.g.height = 45;
	wi.g.parent = ghContainer_PageHome;
	wi.text = "0";
	wi.customDraw = gwinLabelDrawJustifiedLeft;
	wi.customParam = 0;
	wi.customStyle = 0;
	//ghScaleADCvalue = gwinWindowCreate(0, (const struct GWindowInit *)&wi);
	ghScaleADCvalue =gwinLabelCreate(0, &wi);
#endif



#if 0
	const point a[6] = { {1, 1}, { 2, 0}, { 8, 0}, {9, 1}, {8, 2}, {2, 2} };
	const point b[6] = { {9, 1}, {10, 2}, {10, 8}, {9, 9}, {8, 8}, {8, 2} };
	const point c[6] = { {9, 9}, {10,10}, {10,16}, {9,17}, {8,16}, {8,10} };
	const point d[6] = { {9,17}, { 8,18}, { 2,18}, {1,17}, {2,16}, {8,16} };
	const point e[6] = { {1,17}, { 0,16}, { 0,10}, {1, 9}, {2,10}, {2,16} };
	const point f[6] = { {1, 9}, { 0, 8}, { 0, 2}, {1, 1}, {2, 2}, {2, 8} };
	const point g[6] = { {1, 9}, { 2, 8}, { 8, 8}, {9, 9}, {8,10}, {2,10} };
#endif

#if 0
	sevensegment_update_coord(0, 0);

	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, av, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, bv, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, cv, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, dv, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, ev, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, fv, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, gv, 6);
#endif

#if 0
	sevensegment_update_coord(30, 0);

	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, av, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, bv, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, cv, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, dv, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, ev, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, fv, 6);
	gwinFillConvexPoly(ghScaleADCvalue, 2, 2, gv, 6);
#endif

	//gwinDrawPoly(ghScaleADCvalue, 2, 8, a, 6);
	//gwinFillConvexPoly(ghScaleADCvalue, 2, 2, b, 6);
	//gwinFillConvexPoly(ghScaleADCvalue, 2, 2, c, 6);
	//gwinFillConvexPoly(ghScaleADCvalue, 2, 2, d, 6);
	//gwinFillConvexPoly(ghScaleADCvalue, 2, 2, e, 6);
	//gwinFillConvexPoly(ghScaleADCvalue, 2, 2, f, 6);
	//gwinFillConvexPoly(ghScaleADCvalue, 2, 2, g, 6);

#if 0
	gwinDrawLine(ghScaleADCvalue, 0, 0, 310, 0);

	uint16_t i;

	for (i = 0; i <= 310; i += 6) {
		gwinDrawLine(ghScaleADCvalue, i, 0, i, 5);
	}

	for (i = 0; i <= 310; i += 15) {
		//gdispDrawLine(i, 70, i, 78, White);
	}

	for (i = 0; i <= 310; i += 30) {
		gwinDrawLine(ghScaleADCvalue, i, 0, i, 10);
	}


	uint8_t n;
	char* buffer[5];
	for (i = 0; i <= 310; i += 30) {
		sprintf(buffer, "%d", n++);
		gwinDrawString(ghScaleADCvalue, i-3, 16, buffer);
		//gdispDrawStringBox(i, 55, i+12, 70, "0", fixed_7x14, White, justifyCenter);
	}	
#endif


#if 0
	gdispDrawLine(5, 70, 315, 70, HTML2COLOR(0x4f4f4f));

	uint16_t i;

	for (i = 5; i <= 315; i += 6) {
		gdispDrawLine(i, 70, i, 75, HTML2COLOR(0x4f4f4f));
	}

	for (i = 5; i <= 315; i += 15) {
		//gdispDrawLine(i, 70, i, 78, White);
	}

	for (i = 5; i <= 315; i += 30) {
		gdispDrawLine(i, 70, i, 80, HTML2COLOR(0x4f4f4f));
	}


	uint8_t n;
	char* buffer[5];
	for (i = 5; i <= 315; i += 30) {
		sprintf(buffer, "%d", n++);
		gdispDrawString(i-3, 86, buffer, fixed_7x14, HTML2COLOR(0x858585));
		//gdispDrawStringBox(i, 55, i+12, 70, "0", fixed_7x14, White, justifyCenter);
	}	

	//gdispDrawStringBox(1, 55, 10, 70, "0", fixed_7x14, White, justifyCenter);
#endif
}



static void guiMainHome_onShow(GUIWindow *win) {
	gui_set_title(win);

	char adc_string[16] = "";

#ifdef UGFXSIMULATOR

	//sprintf(adc_string, "%.3f%c C", 103.456789, 176);

	//gwinShow(ghLabelADCindicOK);

#else
	//sprintf(adc_string, "%d", uhADCxConvertedValue);

	//GSourceHandle  adc_value;

	//adc_value = ginputGetDial(0);
	//sprintf(adc_string, "%d", gwinSliderGetPosition(ghSliderADCvalue));
#endif
	
	//gwinSetText(ghLabelADCvalue, adc_string, 1);	
	//gwinSetText(ghScaleADCvalue, adc_string, 1);	



	#if 0
	sevensement_draw_1( 30, 0);
	sevensement_draw_2( 60, 0);
	sevensement_draw_3( 90, 0);
	sevensement_draw_4(120, 0);
	sevensement_draw_5(150, 0);
	sevensement_draw_6(180, 0);
	sevensement_draw_7(210, 0);
	sevensement_draw_8(240, 0);
	sevensement_draw_9(270, 0);
	#endif
}


static void guiMainHome_onClose(GUIWindow *win) {

	(void) win;

}


static int guiMainHome_handleEvent(GUIWindow *win, GEvent *pe) {

    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton  *peb = (GEventGWinButton *)pe;

            if (peb->gwin == ghBtn_MENU) {
            	fprintf(stderr, "Menu:\n");
                guiWindow_Show (&winMainMenuOne);
            }
#if 0
            else if (peb->gwin == ghButtonPlayer)
                guiWindow_Show (&winBrowseFile);

            else if (peb->gwin == ghButtonRecognize)
                guiWindow_Show (&winRecognizer);

            else if (peb->gwin == ghButtonWeather)
                guiWindow_Show (&winWeather);
#endif
            else if (peb->gwin == ghBtn_PageOne)
                guiWindow_Show(&winMainMenuOne);

            else
                return 0;

            return 1;
        }

        break;
    }

    return 0;
}



GUIWindow winMainHome = {

/* Title   */	 "Home",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiMainHome_onShow,
/* onClose */    guiMainHome_onClose,
/* onEvent */    guiMainHome_handleEvent,
/* handle  */    0

};




