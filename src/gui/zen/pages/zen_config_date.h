/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_DATE_CONFIG_H_
#define _ZEN_DATE_CONFIG_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelDateConfig;
extern GHandle ghBtn_SetDate;

/* IMAGES */

void create_PageDateConfig(void);
extern GHandle ghContainer_PageDateConfig;
extern GUIWindow winDateConfigMenu;


#endif /* _ZEN_DATE_CONFIG_H_ */