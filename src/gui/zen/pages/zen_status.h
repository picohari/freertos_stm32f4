/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_STATUS_H_
#define _ZEN_STATUS_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */
extern gdispImage ic_back;

/* BUTTONS */
extern GHandle ghBtn_BackStatus;

/* FUNCTIONS */
void create_PageStatus(void);
extern GHandle ghContainer_PageStatus;
extern GUIWindow winStatusMenu;


#endif /* _ZEN_STATUS_H_ */