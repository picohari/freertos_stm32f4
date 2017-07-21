#include "colors.h"
#include "widgetstyles.h"
#include "gui.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "src/gwin/gwin_keyboard_layout.h"
#include "pages/pages.h"

#ifndef UGFXSIMULATOR
	#include "calibration.h"
#endif

#define PRINTER(name) (#name)

// GListeners
GListener glistener;

// GHandles
GHandle ghContainer;
GHandle ghKeyboard;
GHandle ghTextedit;
GHandle ghLabel;

// Fonts
font_t dejavu_sans_16;

// Images
gdispImage intewa;
gdispImage _leer;
gdispImage _obenlinks;
gdispImage _obenmitte;
gdispImage _obenrechts;
gdispImage _untenlinks;
gdispImage _untenmitte;
gdispImage _untenrechts;
gdispImage einstellungen;
gdispImage zurueck;
gdispImage weiter;
gdispImage bestaetigen;

//Keyboard layouts
static const GVSpecialKey KeyboardSpecialKeys[] = {
	{ "\001", 0, GVKEY_SINGLESET, 1 },				// \001 (1)	= Shift Lower to Upper
	{ "\001", 0, GVKEY_INVERT|GVKEY_LOCKSET, 2 },	// \002 (2)	= Shift Upper to Upper Lock
	{ "\002", 0, GVKEY_INVERT|GVKEY_LOCKSET, 0 },	// \003 (3)	= Shift Upper Lock to Lower
	{ "123", 0, GVKEY_LOCKSET, 3 },					// \004 (4)	= Change to Numbers
	{ "\010", "\b", 0, 0 },							// \005 (5)	= Backspace
	{ "\015", "\r", 0, 0 },							// \006 (6)	= Enter 1
	{ "\015", "\r", 0, 0 },							// \007 (7)	= Enter 2 (Short keycap)
	{ "Sym", 0, GVKEY_LOCKSET, 4 },					// \010 (8)	= Change to Symbols
	{ "aA", 0, GVKEY_LOCKSET, 0 },					// \011 (9)	= Change to Lower Alpha
};

//Generic QWERTZ
static const char QwertzKS10[] = "QWERTZUIOP";
static const char QwertzKS11[] = "ASDFGHJKL";
static const char QwertzKS12[] = "YXCVBNM";

static const char *QwertzKS0[] = { "qwertyuiop",	"asdfghjkl",	"yxcvbnm",	"\001 \005",	0 };
static const char *QwertzKS1[] = { QwertzKS10,		QwertzKS11,		QwertzKS12,	"\002 \005",	0 };
static const char *QwertzKS2[] = { QwertzKS10,		QwertzKS11,		QwertzKS12,	"\003 \005",	0 };

static const GVKeySet QwertzKS[] = { QwertzKS0, QwertzKS1, QwertzKS2, 0 };
const GVKeyTable QwertzKeyboard = { KeyboardSpecialKeys, QwertzKS };

//NumPad for times (with :)
static const char *NumpadTKS1[] = { "123", "456", "789", ":0\005", 0 };
static const GVKeySet NumpadTKS[] = { NumpadTKS1, 0 };
const GVKeyTable NumpadTimeKeyboard = { KeyboardSpecialKeys, NumpadTKS };

//NumPad for dates (with .)
static const char *NumpadDKS1[] = { "123", "456", "789", ".0\005", 0 };
static const GVKeySet NumpadDKS[] = { NumpadDKS1, 0 };
const GVKeyTable NumpadDateKeyboard = { KeyboardSpecialKeys, NumpadDKS };

//NumPad for relations (with /)
static const char *NumpadRKS1[] = { "123", "456", "789", "/0\005", 0 };
static const GVKeySet NumpadRKS[] = { NumpadRKS1, 0 };
const GVKeyTable NumpadRelationKeyboard = { KeyboardSpecialKeys, NumpadRKS };

//NumPad with digits only (with neither)
static const char *NumpadKS1[] = { "123", "456", "789", "0\005", 0 };
static const GVKeySet NumpadKS[] = { NumpadKS1, 0 };
const GVKeyTable NumpadKeyboard = { KeyboardSpecialKeys, NumpadKS };

//Destroy and null the current page
void destroyPage(void) {
	clockDrawable = 0;
	if(ghContainer != NULL) {
		gwinDestroy(ghContainer);
		ghContainer = NULL;
	}
}

//If a functionless button was pressed, do nothing
void nothing(void) {
	return;
}

//Define the dimensions used for each button (x coordinate, y coordinate, width, height) (by index)
double buttonDimensions[6][4] = {
	{0,						0,					GDISP_SCREEN_WIDTH / 3,	GDISP_SCREEN_HEIGHT / 2},
	{GDISP_SCREEN_WIDTH / 3,		0,					GDISP_SCREEN_WIDTH / 3,	GDISP_SCREEN_HEIGHT / 2},
	{GDISP_SCREEN_WIDTH / 3 * 2,	0,					GDISP_SCREEN_WIDTH / 3,	GDISP_SCREEN_HEIGHT / 2},
	{0,						GDISP_SCREEN_HEIGHT / 2,	GDISP_SCREEN_WIDTH / 3,	GDISP_SCREEN_HEIGHT / 2},
	{GDISP_SCREEN_WIDTH / 3,		GDISP_SCREEN_HEIGHT / 2,	GDISP_SCREEN_WIDTH / 3,	GDISP_SCREEN_HEIGHT / 2},
	{GDISP_SCREEN_WIDTH / 3 * 2,	GDISP_SCREEN_HEIGHT / 2,	GDISP_SCREEN_WIDTH / 3,	GDISP_SCREEN_HEIGHT / 2}
};

//Define the labels used for each button (by index)
char *buttonLabels[6];

//Define the images used for each button (by index)
gdispImage *buttonImages[6];

//Define the functions called for each button (by index)
void (*buttonFunctions[6])(void);

//Create a nice button with custom dimensions, label, image and tag
void createButton(GWidgetInit wi, int index) {
	wi.g.show = TRUE;
	wi.g.x = ceil(buttonDimensions[index][0]);
	wi.g.y = ceil(buttonDimensions[index][1]);
	wi.g.width = ceil(buttonDimensions[index][2]);
	wi.g.height = ceil(buttonDimensions[index][3]);
	wi.g.parent = ghContainer;
	wi.text = buttonLabels[index];
	wi.customDraw = gwinButtonDraw_Image;
	wi.customParam = buttonImages[index];
	wi.customStyle = 0;
	wi.tag = index;
	gwinButtonCreate(0, &wi);
}

//Create a generic container for each page
GHandle createContainer(GWidgetInit wi) {
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = GDISP_SCREEN_WIDTH;
	wi.g.height = GDISP_SCREEN_HEIGHT;
	wi.g.parent = 0;
	wi.text = "";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	return gwinContainerCreate(0, &wi, 0);
}

//Create generic "yes or no" page
GWidgetInit createYesNo(void (*yesFunction)(void), void (*noFunction)(void)) {
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	// Create container widget
	ghContainer = createContainer(wi);
	
	//page configuration (3x2, indexes from left to right, top to bottom):
	
	//labels
	buttonLabels[0] = "";
	buttonLabels[1] = "";
	buttonLabels[2] = "";
	buttonLabels[3] = "";
	buttonLabels[4] = "";
	buttonLabels[5] = "";
	
	//images (must be defined under Images above and declared through guiCreate below)
	buttonImages[0] = &_obenlinks;
	buttonImages[1] = &_obenmitte;
	buttonImages[2] = &bestaetigen;
	buttonImages[3] = &_untenlinks;
	buttonImages[4] = &_untenmitte;
	buttonImages[5] = &zurueck;
	
	//functions (must be declared above)
	buttonFunctions[0] = nothing;
	buttonFunctions[1] = nothing;
	buttonFunctions[2] = yesFunction;
	buttonFunctions[3] = nothing;
	buttonFunctions[4] = nothing;
	buttonFunctions[5] = noFunction;
	
	//create the buttons using the information above
	createButton(wi, 2);
	createButton(wi, 5);
	
	//show the page
	gwinShow(ghContainer);
	
	return wi;
}

//Create generic "are you sure?" page
void createQuestion(void (*yesFunction)(void), void (*noFunction)(void), char* fieldText) {
	GWidgetInit wi = createYesNo(yesFunction, noFunction);
	
	// Create label widget: ghLabel
	wi.g.show = TRUE;
	wi.g.x = buttonDimensions[0][0];
	wi.g.y = buttonDimensions[0][1];
	wi.g.width = buttonDimensions[0][2] * 2;
	wi.g.height = buttonDimensions[0][3] * 2;
	wi.g.parent = ghContainer;
	wi.text = fieldText;
	wi.customDraw = gwinLabelDrawJustifiedCenter;
	ghLabel = gwinLabelCreate(0, &wi);
}

//Create generic keyboard + textedit page
void createAnswer(void (*yesFunction)(void), void (*noFunction)(void), const GVKeyTable* keyboardtype, char* fieldText) {
	GWidgetInit wi = createYesNo(yesFunction, noFunction);
	
	// Create keyboard widget: ghKeyboard
	wi.g.show = TRUE;
	wi.g.x = buttonDimensions[3][0];
	wi.g.y = buttonDimensions[3][1];
	wi.g.width = buttonDimensions[3][2] * 2;
	wi.g.height = buttonDimensions[3][3];
	wi.g.parent = ghContainer;
	ghKeyboard = gwinKeyboardCreate(0, &wi);
	
	//change keyboard layout to show a numpad
	gwinKeyboardSetLayout(ghKeyboard, keyboardtype);
	
	wi.g.show = TRUE;
	wi.g.x = buttonDimensions[0][0];
	wi.g.y = buttonDimensions[0][1];
	wi.g.width = buttonDimensions[0][2] * 2;
	wi.g.height = buttonDimensions[0][3];
	wi.g.parent = ghContainer;
	wi.text = fieldText;
	ghTextedit = gwinTexteditCreate(0, &wi, 100);
	//gwinSetText(ghTextedit, fieldText, 0);
	gwinSetFocus(ghTextedit);
}

void guiShowPage(unsigned pageIndex) {
	// Hide all pages
	gwinHide(ghContainer);

	// Show page selected page
	switch (pageIndex) {
	case 0:
		gwinShow(ghContainer);
		break;

	default:
		break;
	}
}

void guiCreate(void) {
	GWidgetInit wi;

	// Prepare fonts
	dejavu_sans_16 = gdispOpenFont("DejaVuSans16");

	// Prepare images
	gdispImageOpenFile(&intewa, "rsc/intewa.gif");
	gdispImageOpenFile(&_leer, "rsc/_leer.gif");
	gdispImageOpenFile(&_obenlinks, "rsc/_obenlinks.gif");
	gdispImageOpenFile(&_obenmitte, "rsc/_obenmitte.gif");
	gdispImageOpenFile(&_obenrechts, "rsc/_obenrechts.gif");
	gdispImageOpenFile(&_untenlinks, "rsc/_untenlinks.gif");
	gdispImageOpenFile(&_untenmitte, "rsc/_untenmitte.gif");
	gdispImageOpenFile(&_untenrechts, "rsc/_untenrechts.gif");
	gdispImageOpenFile(&einstellungen, "rsc/einstellungen.gif");
	gdispImageOpenFile(&zurueck, "rsc/zurueck.gif");
	gdispImageOpenFile(&weiter, "rsc/weiter.gif");
	gdispImageOpenFile(&bestaetigen, "rsc/bestaetigen.gif");

	// GWIN settings
	gwinWidgetClearInit(&wi);
	gwinSetDefaultFont(dejavu_sans_16);
	gwinSetDefaultStyle(&white, FALSE);
	gwinSetDefaultColor(black_studio);
	gwinSetDefaultBgColor(white_studio);

	//Create and show the overview page, including the clock
	showOverview();
}

void guiEventLoop(void) {
	GEvent* pe;

	while (1) {
		if(clockDrawable == 1) {
			//refresh the clock
			drawClock();
			
			// Get an event
			pe = geventEventWait(&glistener, TIME_IMMEDIATE);
			if(pe == NULL) {
				continue;
			}
		} else {
			pe = geventEventWait(&glistener, TIME_INFINITE);
		}
		
		//if the event was a button press, executve the relative command stored in the above array
		switch (pe->type) {
			case GEVENT_GWIN_BUTTON:
				(*buttonFunctions[((GEventGWinButton*) pe)->tag])();
				break;
			default:
				break;
		}
	}
}
