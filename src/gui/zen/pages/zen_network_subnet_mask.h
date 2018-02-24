/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_NETWORK_SUBNET_MASK_H_
#define _ZEN_NETWORK_SUBNET_MASK_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelSubnetMask;
extern GHandle ghBtn_SetSubnetMask;

/* IMAGES */

/* FUNCTIONS */
void create_PageNetworkSubnetMask(void);
extern GHandle ghContainer_PageNetworkSubnetMask;
extern GUIWindow winNetworkSubnetMaskMenu;


#endif /* _ZEN_NETWORK_SUBNET_MASK_H_ */