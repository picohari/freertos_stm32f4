#ifndef _ZEN_TIME_CONFIG_H_
#define _ZEN_TIME_CONFIG_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelTimeConfig;
extern GHandle ghBtn_SetTime;

/* IMAGES */

void create_PageTimeConfig(void);
extern GHandle ghContainer_PageTimeConfig;
extern GUIWindow winTimeConfigMenu;


#endif /* _ZEN_TIME_CONFIG_H_ */