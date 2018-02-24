/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_MEMBRANE_H_
#define _ZEN_MEMBRANE_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelMembrane;
extern GHandle ghBtn_SetMembrane;

/* FUNCTIONS */
void create_PageMembrane(void);
extern GHandle ghContainer_PageMembrane;
extern GUIWindow winMembraneMenu;


#endif /* _ZEN_MEMBRANE_H_ */