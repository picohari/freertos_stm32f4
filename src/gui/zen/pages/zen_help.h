/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_HELP_H_
#define _ZEN_HELP_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */

/* BUTTONS */
extern GHandle ghBtn_BackHelp;

/* FUNCTIONS */
void create_PageHelp(void);
extern GHandle ghContainer_PageHelp;
extern GUIWindow winHelpMenu;


#endif /* _ZEN_HELP_H_ */