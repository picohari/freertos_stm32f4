/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_MAIN_ONE_H_
#define _ZEN_MAIN_ONE_H_

#include "gfx.h"

#include "gui.h"
#include "gui_router.h"


/* LABELS */
#define MENU_TITLE_CONFIGURATION		"Config"
#define MENU_TITLE_CLEANING				"Clean"
#define MENU_TITLE_TIMERS				"Timers"
#define MENU_TITLE_NETWORK				"Network"
#define MENU_TITLE_TESTMODE				"Test\nMode"
#define MENU_TITLE_STATUS				"Status"
#define MENU_TITLE_HELP					"Help"
#define MENU_TITLE_NEXT					"NEXT"


/* IMAGES */
extern gdispImage ic_home;
extern gdispImage ic_settings;
extern gdispImage ic_local_drink;
extern gdispImage ic_alarm;
extern gdispImage ic_public;
extern gdispImage ic_heart_pulse;
extern gdispImage ic_search;
extern gdispImage ic_live_help;
extern gdispImage ic_forward;

//extern gdispImage ic_thumbs_up_down;


/* BUTTONS */
extern GHandle ghBtn_Config;
extern GHandle ghBtn_Clean;
extern GHandle ghBtn_Timers;
extern GHandle ghBtn_Network;
extern GHandle ghBtn_Testmode;
extern GHandle ghBtn_Status;
extern GHandle ghBtn_Help;
extern GHandle ghBtn_PageTwo;


/* FUNCTIONS */
void create_PageOne(void);
extern GHandle ghContainer_PageOne;
extern GUIWindow winMainMenuOne;



#endif /* _ZEN_MAIN_ONE_H_ */
