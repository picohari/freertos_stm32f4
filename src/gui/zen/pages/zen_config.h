/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_CONFIG_H_
#define _ZEN_CONFIG_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_DATE	 				"DATE"
#define MENU_TITLE_TIME					"TIME"
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */
extern gdispImage ic_date;
extern gdispImage ic_time;
extern gdispImage ic_back;

/* BUTTONS */
extern GHandle ghBtn_Date;
extern GHandle ghBtn_Time;
extern GHandle ghBtn_BackConfig;

/* FUNCTIONS */
void create_PageConfig(void);
extern GHandle ghContainer_PageConfig;
extern GUIWindow winConfigMenu;


#endif /* _ZEN_CONFIG_H_ */