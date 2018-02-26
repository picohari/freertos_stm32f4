#ifndef _ZEN_AERATION_A2_H_
#define _ZEN_AERATION_A2_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelAerationA2;
extern GHandle ghBtn_SetAerationA2;

/* IMAGES */

void create_PageAerationA2(void);
extern GHandle ghContainer_PageAerationA2;
extern GUIWindow winAerationA2Menu;


#endif /* _ZEN_AERATION_A2_H_ */