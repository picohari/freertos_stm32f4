/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_NETWORK_GATEWAY_H_
#define _ZEN_NETWORK_GATEWAY_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelGateway;
extern GHandle ghBtn_SetGateway;

/* IMAGES */

/* FUNCTIONS */
void create_PageNetworkGateway(void);
extern GHandle ghContainer_PageNetworkGateway;
extern GUIWindow winNetworkGatewayMenu;


#endif /* _ZEN_NETWORK_GATEWAY_H_ */