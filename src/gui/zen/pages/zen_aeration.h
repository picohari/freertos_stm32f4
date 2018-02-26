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
#define MENU_TITLE_AIR_ON					"AIR ON"
#define MENU_TITLE_AIR_OFF					"AIR OFF"
#define MENU_TITLE_A1						"A1"
#define MENU_TITLE_A2						"A2"
#define MENU_TITLE_BACK						"BACK"

/* IMAGES */

/* BUTTONS */
extern GHandle ghBtn_AerationAirOn;
extern GHandle ghBtn_AerationAirOff;
extern GHandle ghBtn_AerationA1;
extern GHandle ghBtn_AerationA2;
extern GHandle ghBtn_BackAeration;

/* FUNCTIONS */
void create_PageAeration(void);
extern GHandle ghContainer_PageAeration;
extern GUIWindow winAerationMenu;


#endif /* _ZEN_AERATION_H_ */