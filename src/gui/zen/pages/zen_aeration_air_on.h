#ifndef _ZEN_AERATION_AIR_ON_H_
#define _ZEN_AERATION_AIR_ON_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelAerationAirOn;
extern GHandle ghBtn_SetAerationAirOn;

/* IMAGES */

void create_PageAerationAirOn(void);
extern GHandle ghContainer_PageAerationAirOn;
extern GUIWindow winAerationAirOnMenu;


#endif /* _ZEN_AERATION_AIR_ON_H_ */