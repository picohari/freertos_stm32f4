/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_OVERFLOW_H_
#define _ZEN_OVERFLOW_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */
extern gdispImage ic_back;

/* BUTTONS */
extern GHandle ghBtn_BackOverflow;

/* FUNCTIONS */
void create_PageOverflow(void);
extern GHandle ghContainer_PageOverflow;
extern GUIWindow winOverflowMenu;


#endif /* _ZEN_OVERFLOW_H_ */