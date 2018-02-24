/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_CYCLE_TIME_H_
#define _ZEN_CYCLE_TIME_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */

/* BUTTONS */
extern GHandle ghBtn_BackCycleTime;

/* FUNCTIONS */
void create_PageCycleTime(void);
extern GHandle ghContainer_PageCycleTime;
extern GUIWindow winCycleTimeMenu;


#endif /* _ZEN_CYCLE_TIME_H_ */