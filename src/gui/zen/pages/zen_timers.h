/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_TIMERS_H_
#define _ZEN_TIMERS_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */
extern gdispImage ic_back;

/* BUTTONS */
extern GHandle ghBtn_BackTimers;

/* FUNCTIONS */
void create_PageTimers(void);
extern GHandle ghContainer_PageTimers;
extern GUIWindow winTimersMenu;


#endif /* _ZEN_TIMERS_H_ */