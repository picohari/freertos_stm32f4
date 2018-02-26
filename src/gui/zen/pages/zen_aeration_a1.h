#ifndef _ZEN_AERATION_A1_H_
#define _ZEN_AERATION_A1_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelAerationA1;
extern GHandle ghBtn_SetAerationA1;

/* IMAGES */

void create_PageAerationA1(void);
extern GHandle ghContainer_PageAerationA1;
extern GUIWindow winAerationA1Menu;


#endif /* _ZEN_AERATION_A1_H_ */