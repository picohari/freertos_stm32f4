#include <stdio.h>

#include "gfx.h"
#include "gui.h"
#include "stdio.h"

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

#include "skin/zenstyle.h"
#include "gui_router.h"

#include "pages/zen_membrane.h"
#include "pages/zen_main_two.h"
#include "helpers/membrane_util.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageMembrane;

/* BUTTONS */
GHandle ghBtn_CancelMembrane;
GHandle ghBtn_SetMembrane;
GHandle ghBtn_IncreaseMembraneNumber;
GHandle ghBtn_DecreaseMembraneNumber;

/* IMAGES */
gdispImage ic_cancel;
gdispImage ic_done;
gdispImage ic_add;
gdispImage ic_remove;

/* INPUT FIELDS AND KEYBOARD */
static GHandle ghTexteditMembraneNumber;
static GHandle ghLabel_ErrorMembrane;

static uint8_t membrane_count = 1;

void create_PageMembrane(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageMembrane
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
	ghContainer_PageMembrane = gwinContainerCreate(0, &wi, 0);

	// TextEditFirstBlock
	wi.g.show = TRUE;
	wi.g.x = 135;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageMembrane;
	wi.text = "";
	ghTexteditMembraneNumber = gwinTexteditCreate(0, &wi, 1);

	// create button widget: ghBtn_SetMembrane
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageMembrane;
	//wi.text = MENU_TITLE_OK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_done;
	wi.customStyle = &color_eight;
	ghBtn_SetMembrane = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_CancelMembrane
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageMembrane;
	//wi.text = MENU_TITLE_CANCEL;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_cancel;
	wi.customStyle = &color_five;
	ghBtn_CancelMembrane = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_DecreaseMembraneNumber
	wi.g.show = TRUE;
	wi.g.x = 50;
	wi.g.y = 5;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageMembrane;
	//wi.text = MENU_TITLE_DECREASE;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_remove;
	wi.customStyle = &color_one;
	ghBtn_DecreaseMembraneNumber = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_IncreaseMembraneNumber
	wi.g.show = TRUE;
	wi.g.x = 190;
	wi.g.y = 5;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageMembrane;
	//wi.text = MENU_TITLE_INCREASE;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_add;
	wi.customStyle = &color_one;
	ghBtn_IncreaseMembraneNumber = gwinButtonCreate(0, &wi);

	// create the Label widget: ghLabel_ErrorMembrane
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;
	wi.g.x = 45;
	wi.g.y = 80;
	wi.g.width = 160;
	wi.g.height = 20;
	wi.g.parent = ghContainer_PageMembrane;
	wi.text = "";
	ghLabel_ErrorMembrane = gwinLabelCreate(NULL, &wi);	
	
}

static void guiMembraneMenu_onShow(GUIWindow *win) {

	gui_set_title(win);
	
	membrane_count = get_membrane_count();

	char memCountArray[2];
    snprintf(memCountArray, sizeof(memCountArray), "%u", membrane_count);

    gwinSetText(ghTexteditMembraneNumber, memCountArray, TRUE);

    // This function was manually added to the µGFX library under ugfx/src/gwin/gwin_textedit.h
    // Move the cursor away so it is not visible. We don't actually need it here, because we edit the text
    // of the TextEdit with the help of the buttons and not with the keyboard 
	gwinTextEditSetCursorPosition((GTexteditObject*) ghTexteditMembraneNumber, 10);

}

static void guiMembraneMenu_onClose(GUIWindow *win) {

	(void) win;

	gwinSetText(ghLabel_ErrorMembrane, "", TRUE);

}

static int guiMembraneMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_CancelMembrane) {

            	guiWindow_Show(&winMainMenuTwo);

            } else if(peb->gwin == ghBtn_SetMembrane) {

            	guiWindow_Show(&winMainMenuTwo);
            
            } else if(peb->gwin == ghBtn_IncreaseMembraneNumber) {

            	if(membrane_count < 6) {
            		membrane_count++;
            		set_membrane_count(membrane_count);
            	}
            	
            	char memCountArray[2];
            	snprintf(memCountArray, sizeof(memCountArray), "%u", membrane_count);

            	gwinSetText(ghTexteditMembraneNumber, memCountArray, TRUE);

            } else if(peb->gwin == ghBtn_DecreaseMembraneNumber) {

            	if(membrane_count > 1) {
            		membrane_count--;
            		set_membrane_count(membrane_count);
            	}

            	char memCountArray[2];
            	snprintf(memCountArray, sizeof(memCountArray), "%u", membrane_count);

            	gwinSetText(ghTexteditMembraneNumber, memCountArray, TRUE);

            } else {
            	return 0;
            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winMembraneMenu = {

/* Title   */	 "Membrane Number Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiMembraneMenu_onShow,
/* onClose */    guiMembraneMenu_onClose,
/* onEvent */    guiMembraneMenu_handleEvent,
/* handle  */    0

};