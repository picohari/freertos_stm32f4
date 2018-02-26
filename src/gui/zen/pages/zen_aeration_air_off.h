#ifndef _ZEN_AERATION_AIR_OFF_H_
#define _ZEN_AERATION_AIR_OFF_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelAerationAirOff;
extern GHandle ghBtn_SetAerationAirOff;

/* IMAGES */

void create_PageAerationAirOff(void);
extern GHandle ghContainer_PageAerationAirOff;
extern GUIWindow winAerationAirOffMenu;


#endif /* _ZEN_AERATION_AIR_OFF_H_ */