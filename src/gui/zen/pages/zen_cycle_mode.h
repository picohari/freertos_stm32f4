/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_CYCLE_MODE_H_
#define _ZEN_CYCLE_MODE_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */
extern gdispImage ic_back;

/* BUTTONS */
extern GHandle ghBtn_BackCycleMode;

/* FUNCTIONS */
void create_PageCycleMode(void);
extern GHandle ghContainer_PageCycleMode;
extern GUIWindow winCycleModeMenu;


#endif /* _ZEN_CYCLE_MODE_H_ */