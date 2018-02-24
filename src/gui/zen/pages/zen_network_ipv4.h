/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_NETWORK_IPV4_H_
#define _ZEN_NETWORK_IPV4_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_CANCEL				"CANCEL"
#define MENU_TITLE_OK					"OK"

/* BUTTONS */
extern GHandle ghBtn_CancelIpv4;
extern GHandle ghBtn_SetIpv4;

/* IMAGES */

/* FUNCTIONS */
void create_PageNetworkIpv4(void);
extern GHandle ghContainer_PageNetworkIpv4;
extern GUIWindow winNetworkIpv4Menu;


#endif /* _ZEN_NETWORK_IPV4_H_ */