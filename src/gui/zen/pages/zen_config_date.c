#include <stdio.h>
#include <string.h>

#include "gfx.h"
#include "gui.h"
#include "src/gwin/gwin_keyboard_layout.h"


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

#include "zen_config_date.h"
#include "pages/zen_config.h"
#include "helpers/date_and_time_util.h"

/* PAGE CONTAINER */
GHandle ghContainer_PageDateConfig;

/* BUTTONS */
GHandle ghBtn_CancelDateConfig;
GHandle ghBtn_SetDate;

/* IMAGES */


/* EVENT LISTENER */
static GListener gl;

/* INPUT FIELDS AND KEYBOARD */
static GHandle ghTexteditDate;
static GHandle ghTexteditMonth;
static GHandle ghTexteditYear;
static GHandle ghKeyboard;
static GHandle ghLabel_ErrorDate;

//Keyboard layouts
static const GVSpecialKey KeyboardSpecialKeys[] = {
	{ "\001", 0, GVKEY_SINGLESET, 1 },				// \001 (1)	= Shift Lower to Upper
	{ "\001", 0, GVKEY_INVERT|GVKEY_LOCKSET, 2 },	// \002 (2)	= Shift Upper to Upper Lock
	{ "\002", 0, GVKEY_INVERT|GVKEY_LOCKSET, 0 },	// \003 (3)	= Shift Upper Lock to Lower
	{ "123", 0, GVKEY_LOCKSET, 3 },					// \004 (4)	= Change to Numbers
	{ "\010", "\b", 0, 0 }							// \005 (5)	= Backspace
};

//NumPad with digits only (with none of the above)
static const char *numpadKeySetArray[] = { "789", "456", "123", "0\005", 0 };
static const GVKeySet numpadKeySet[] = { numpadKeySetArray, 0 };
static const GVKeyTable numpadKeyboard = { KeyboardSpecialKeys, numpadKeySet };


void create_PageDateConfig(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageDateConfig
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
	ghContainer_PageDateConfig = gwinContainerCreate(0, &wi, 0);

	// Create the keyboard
	wi.g.show = TRUE;
	wi.g.x = 0; 
	wi.g.y = 58;
	wi.g.width = gdispGetWidth() / 2 + 80; 
	wi.g.height = gdispGetHeight() / 2 + 22;
	wi.g.parent = ghContainer_PageDateConfig;
	ghKeyboard = gwinKeyboardCreate(0, &wi);

	// TextEditDate
	wi.g.show = TRUE;
	wi.g.x = 77;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageDateConfig;
	wi.text = "";
	ghTexteditDate = gwinTexteditCreate(0, &wi, 2);

	// TextEditMonth
	wi.g.show = TRUE;
	wi.g.x = 137;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageDateConfig;
	ghTexteditMonth = gwinTexteditCreate(0, &wi, 2);

	// TextEditYear
	wi.g.show = TRUE;
	wi.g.x = 197;
	wi.g.y = 5;
	wi.g.width = 50;
	wi.g.height = 30;
	wi.g.parent = ghContainer_PageDateConfig;
	ghTexteditYear = gwinTexteditCreate(0, &wi, 4);
	
	// create button widget: ghBtn_SetDate
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 58;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageDateConfig;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_done;
	wi.customStyle = &color_eight;
	ghBtn_SetDate = gwinButtonCreate(0, &wi);

	// create button widget: ghBtn_CancelDateConfig
	wi.g.show = TRUE;
	wi.g.x = gdispGetWidth() - 80;
	wi.g.y = 127;
	wi.g.width = 80;
	wi.g.height = 69;
	wi.g.parent = ghContainer_PageDateConfig;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_cancel;
	wi.customStyle = &color_five;
	ghBtn_CancelDateConfig = gwinButtonCreate(0, &wi);

	// create the Label widget: ghLabel_ErrorDate
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	wi.g.show = TRUE;
	wi.g.x = 77;
	wi.g.y = 35;
	wi.g.width = 160;
	wi.g.height = 20;
	wi.g.parent = ghContainer_PageDateConfig;
	wi.text = "";
	ghLabel_ErrorDate = gwinLabelCreate(NULL, &wi);	
	
	geventListenerInit(&gl);
	geventAttachSource(&gl, ginputGetKeyboard(0), 0);
	gwinKeyboardSetLayout(ghKeyboard, &numpadKeyboard);

}


static void guiDateConfigMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

	char date[3];
	char month[3];
	char year[5];

	snprintf(date, sizeof(date), "%02d", get_date());
	snprintf(month, sizeof(month), "%02d", get_month());
	snprintf(year, sizeof(year), "%d", get_year());

	gwinSetText(ghTexteditDate, date, TRUE);
	gwinSetText(ghTexteditMonth, month, TRUE);
	gwinSetText(ghTexteditYear, year, TRUE);

	// This function was manually added to the µGFX library under ugfx/src/gwin/gwin_textedit.h 
	gwinTextEditSetCursorPosition((GTexteditObject*) ghTexteditDate, strlen(date));
}


static void guiDateConfigMenu_onClose(GUIWindow *win) {

	(void) win;

	gwinSetText(ghLabel_ErrorDate, "", TRUE);
}


static int guiDateConfigMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

     switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

        	if(peb->gwin == ghBtn_CancelDateConfig) {

            	guiWindow_Show(&winConfigMenu);

            } else if(peb->gwin == ghBtn_SetDate) {
            	
            	unsigned int new_date;
            	unsigned int new_month;
            	unsigned int new_year;

            	// Save the old value for the year. If the user changes the year, 
            	// set the new value and then the number of days for February will be computed. 
            	// After that, we can check if the user entered correct values for the month and the date. 
            	// If not, just set back the year to the old value
            	uint16_t old_year = get_year();

            	sscanf(gwinGetText(ghTexteditYear), "%u", &new_year);
            	set_year((uint16_t) new_year);

            	sscanf(gwinGetText(ghTexteditMonth), "%u", &new_month);

            	if(new_month > 12) {
            		set_year(old_year);
            		gwinSetText(ghLabel_ErrorDate, "Invalid month!", TRUE);
            		break;
            	} 

            	set_month((uint8_t) new_month);

            	sscanf(gwinGetText(ghTexteditDate), "%u", &new_date);

            	if(new_date > get_days_in_current_month()) {
            		set_year(old_year);
            		gwinSetText(ghLabel_ErrorDate, "Invalid date!", TRUE);
            		break;
            	}

            	set_date((uint8_t) new_date);

            	update_date_and_time_label();

            	guiWindow_Show(&winConfigMenu);

            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winDateConfigMenu = {

/* Title   */	 "Date Configuration Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiDateConfigMenu_onShow,
/* onClose */    guiDateConfigMenu_onClose,
/* onEvent */    guiDateConfigMenu_handleEvent,
/* handle  */    0

};