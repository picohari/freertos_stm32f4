/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_TESTMODE_H_
#define _ZEN_TESTMODE_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */
extern gdispImage ic_back;

/* BUTTONS */
extern GHandle ghBtn_BackTestMode;

/* FUNCTIONS */
void create_PageTestMode(void);
extern GHandle ghContainer_PageTestMode;
extern GUIWindow winTestModeMenu;


#endif /* _ZEN_TESTMODE_H_ */