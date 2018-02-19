/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_AERATION_H_
#define _ZEN_AERATION_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */
extern gdispImage ic_back;

/* BUTTONS */
extern GHandle ghBtn_BackAeration;

/* FUNCTIONS */
void create_PageAeration(void);
extern GHandle ghContainer_PageAeration;
extern GUIWindow winAerationMenu;


#endif /* _ZEN_AERATION_H_ */