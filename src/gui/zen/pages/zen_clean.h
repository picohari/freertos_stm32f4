/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_CLEAN_H_
#define _ZEN_CLEAN_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */
extern gdispImage ic_back;

/* BUTTONS */
extern GHandle ghBtn_BackClean;

/* FUNCTIONS */
void create_PageClean(void);
extern GHandle ghContainer_PageClean;
extern GUIWindow winCleanMenu;


#endif /* _ZEN_CLEAN_H_ */