/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_NETWORK_H_
#define _ZEN_NETWORK_H_

#include "gfx.h"
#include "gui.h"
#include "gui_router.h"

/* LABELS */
#define MENU_TITLE_IPV4					"IPv4 ADDRESS"
#define MENU_TITLE_GATEWAY				"GATEWAY"
#define MENU_TITLE_SUBNET_MASK			"SUBNET  MASK"
#define MENU_TITLE_DHCP					"DHCP"
#define MENU_TITLE_BACK					"BACK"

/* IMAGES */

/* BUTTONS */
extern GHandle ghBtn_IPv4;
extern GHandle ghBtn_Gateway;
extern GHandle ghBtn_Network_Mask;
extern GHandle ghBtn_DHCP;
extern GHandle ghBtn_BackNetwork;

/* FUNCTIONS */
void create_PageNetwork(void);
extern GHandle ghContainer_PageNetwork;
extern GUIWindow winNetworkMenu;


#endif /* _ZEN_NETWORK_H_ */