/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_T1_T2_H_
#define _ZEN_T1_T2_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_BACK					"BACK"
#define MENU_TITLE_T1					"T1"
#define MENU_TITLE_T2					"T2"

/* IMAGES */

/* BUTTONS */
extern GHandle ghBtn_T1Config;
extern GHandle ghBtn_T2Config;
extern GHandle ghBtn_BackT1T2;

/* FUNCTIONS */
void create_PageT1T2(void);
extern GHandle ghContainer_PageT1T2;
extern GUIWindow winT1T2Menu;


#endif /* _ZEN_T1_T2_H_ */