/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_SLUDGE_H_
#define _ZEN_SLUDGE_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */

/* IMAGES */

/* BUTTONS */
extern GHandle ghBtn_CancelSludge;
extern GHandle ghBtn_SetSludge;

/* FUNCTIONS */
void create_PageSludge(void);
extern GHandle ghContainer_PageSludge;
extern GUIWindow winSludgeMenu;


#endif /* _ZEN_SLUDGE_H_ */