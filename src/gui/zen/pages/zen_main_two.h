/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_MAIN_TWO_H_
#define _ZEN_MAIN_TWO_H_

#include "gfx.h"

#include "gui.h"
#include "gui_router.h"



/* LABELS */
#define MENU_TITLE_BACK					"BACK"
#define MENU_TITLE_T1_T2				"T1/T2"
#define MENU_TITLE_CYCLEMODE			"Cycle\nMode"
#define MENU_TITLE_CYCLEPARAM			"Cycle\nTime"
#define MENU_TITLE_MEMNUMBER			"MEM\nSetup"
#define MENU_TITLE_AERATION				"Aeration"
#define MENU_TITLE_OVERFLOW				"Overflow"
#define MENU_TITLE_SUCTIONSET			"Sludge"


/* IMAGES */
extern gdispImage ic_fan;
extern gdispImage ic_membrane;
extern gdispImage ic_unfill;
extern gdispImage ic_aligntop;
extern gdispImage ic_timelapse;










/* BUTTONS */
extern GHandle ghBtn_T1T2;
extern GHandle ghBtn_Cyclemode;
extern GHandle ghBtn_Cycleparam;
extern GHandle ghBtn_Memnumber;
extern GHandle ghBtn_Aeration;
extern GHandle ghBtn_Overflow;
extern GHandle ghBtn_Suctionset;
extern GHandle ghBtn_PageOne;


/* FUNCTIONS */
void create_PageTwo(void);
extern GHandle ghContainer_PageTwo;
extern GUIWindow winMainMenuTwo;



#endif /* _ZEN_MAIN_TWO_H_ */
