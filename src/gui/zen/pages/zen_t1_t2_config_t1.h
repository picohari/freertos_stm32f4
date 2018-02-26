#ifndef _ZEN_T1_T2_CONFIG_T1_H_
#define _ZEN_T1_T2_CONFIG_T1_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelT1;
extern GHandle ghBtn_SetT1;

/* IMAGES */

void create_PageT1T2ConfigT1(void);
extern GHandle ghContainer_PageT1T2ConfigT1;
extern GUIWindow winT1T2ConfigT1Menu;


#endif /* _ZEN_T1_T2_CONFIG_T1_H_ */