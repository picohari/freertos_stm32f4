/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_MENU_H_
#define _ZEN_MENU_H_

#include "gfx.h"


/* Alle MENU-Pages werden hier inkludiert, sodaß man nur einmal die pages.h angeben muss ... */
#include "pages/zen_aeration.h"
#include "pages/zen_clean.h"
#include "pages/zen_config.h"
#include "pages/zen_config_date.h"
#include "pages/zen_config_time.h"
#include "pages/zen_cycle_mode.h"
#include "pages/zen_cycle_time.h"
#include "pages/zen_help.h"

#include "pages/zen_main_home.h"
#include "pages/zen_main_one.h"
#include "pages/zen_main_two.h"

#include "pages/zen_membrane.h"
#include "pages/zen_network.h"
#include "pages/zen_network_gateway.h"
#include "pages/zen_network_ipv4.h"
#include "pages/zen_network_subnet_mask.h"
#include "pages/zen_overflow.h"
#include "pages/zen_sludge.h"
#include "pages/zen_status.h"
#include "pages/zen_t1_t2.h"
#include "pages/zen_test_mode.h"
#include "pages/zen_timers.h"


#include "helpers/sevensegment.h"


extern void zen_splash(void);




#endif /* _ZEN_MENU_H_ */