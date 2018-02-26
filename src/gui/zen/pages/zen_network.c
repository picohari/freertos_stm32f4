#include <stdio.h>

#include "gfx.h"
#include "gui.h"

#ifdef UGFXSIMULATOR

	#include <stdlib.h>
	#include <unistd.h>
	#include <fcntl.h>
	#include <errno.h>
	//#include <linux/input.h>

#else

	#include "stm32f4xx_hal.h"
	#include "cmsis_os.h"
	#include "XCore407I.h"

#endif

#include "zen_menu.h"
#include "skin/zenstyle.h"
#include "gui_router.h"

#include "pages/zen_network.h"
#include "pages/zen_network_ipv4.h"
#include "pages/zen_network_gateway.h"
#include "pages/zen_network_subnet_mask.h"
#include "pages/zen_main_one.h"


/* PAGE CONTAINER */
GHandle ghContainer_PageNetwork;

/* BUTTONS */
GHandle ghBtn_IPv4;
GHandle ghBtn_Gateway;
GHandle ghBtn_Subnet_Mask;
GHandle ghBtn_DHCP;
GHandle ghBtn_BackNetwork;

/* IMAGES */


void create_PageNetwork(void) {

	GWidgetInit wi;

	gwinWidgetClearInit(&wi);

	// create container widget: ghContainer_PageNetwork
	wi.g.show = FALSE;
	wi.g.x = 0;
	wi.g.y = 24;
	wi.g.width = 320;
	wi.g.height = 192;
	wi.g.parent = 0;
	wi.text = "Container";
	wi.customDraw = 0;
	wi.customParam = 0;
	wi.customStyle = 0;
	ghContainer_PageNetwork = gwinContainerCreate(0, &wi, 0);


	// create button widget: ghBtn_BackNetwork
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageNetwork;
	wi.text = MENU_TITLE_IPV4;
	//wi.customDraw = gwinButtonDraw_Image_Icon;
	//wi.customParam = &ic_back;
	wi.customStyle = &color_one;
	ghBtn_IPv4 = gwinButtonCreate(0, &wi);

	wi.g.show = TRUE;
	wi.g.x = 80;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageNetwork;
	wi.text = MENU_TITLE_GATEWAY;
	//wi.customDraw = gwinButtonDraw_Image_Icon;
	//wi.customParam = &ic_back;
	wi.customStyle = &color_two;
	ghBtn_Gateway = gwinButtonCreate(0, &wi);

	wi.g.show = TRUE;
	wi.g.x = 160;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageNetwork;
	wi.text = MENU_TITLE_SUBNET_MASK;
	//wi.customDraw = gwinButtonDraw_Image_Icon;
	//wi.customParam = &ic_back;
	wi.customStyle = &color_five;
	ghBtn_Subnet_Mask = gwinButtonCreate(0, &wi);


	wi.g.show = TRUE;
	wi.g.x = 240;
	wi.g.y = 0;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageNetwork;
	wi.text = MENU_TITLE_DHCP;
	//wi.customDraw = gwinButtonDraw_Image_Icon;
	//wi.customParam = &ic_back;
	wi.customStyle = &color_four;
	ghBtn_DHCP = gwinButtonCreate(0, &wi);


	// create button widget: ghBtn_BackNetwork
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 96;
	wi.g.width = 80;
	wi.g.height = 96;
	wi.g.parent = ghContainer_PageNetwork;
	wi.text = MENU_TITLE_BACK;
	wi.customDraw = gwinButtonDraw_Image_Icon;
	wi.customParam = &ic_back;
	wi.customStyle = &color_three;
	ghBtn_BackNetwork = gwinButtonCreate(0, &wi);
	
}


static void guiNetworkMenu_onShow(GUIWindow *win) {

	gui_set_title(win);

}

static void guiNetworkMenu_onClose(GUIWindow *win) {

	(void) win;

}

static int guiNetworkMenu_handleEvent(GUIWindow *win, GEvent *pe) {
    
    (void) win;

    switch (pe->type) {

        case GEVENT_GWIN_BUTTON: {

        	GEventGWinButton *peb = (GEventGWinButton *)pe;

            if(peb->gwin == ghBtn_BackNetwork) {
            	guiWindow_Show(&winMainMenuOne);

            } else if(peb->gwin == ghBtn_IPv4) {
            	guiWindow_Show(&winNetworkIpv4Menu);

          	} else if(peb->gwin == ghBtn_Gateway) {
          		guiWindow_Show(&winNetworkGatewayMenu);

          	} else if(peb->gwin == ghBtn_Subnet_Mask) {
          		guiWindow_Show(&winNetworkSubnetMaskMenu);	

            } else {
            	return 0;
            }

            return 1;
        }

        break;
    }
	
    return 0;
	
}



GUIWindow winNetworkMenu = {

/* Title   */	 "Network Settings Menu",
/* onInit  */    guiWindow_onInit,
/* onShow  */    guiNetworkMenu_onShow,
/* onClose */    guiNetworkMenu_onClose,
/* onEvent */    guiNetworkMenu_handleEvent,
/* handle  */    0

};