/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_MENU_H_
#define _ZEN_MENU_H_

#include "gfx.h"


/* Alle MENU-Pages werden hier inkludiert, sodaß man nur einmal die zen_menu.h angeben muss ... */
#include "pages/zen_main_home.h"
#include "pages/zen_main_one.h"
#include "pages/zen_config.h"
#include "pages/zen_config_date.h"
#include "pages/zen_config_time.h"
#include "pages/zen_clean.h"
#include "pages/zen_timers.h"
#include "pages/zen_network.h"
#include "pages/zen_network_ipv4.h"
#include "pages/zen_network_gateway.h"
#include "pages/zen_network_subnet_mask.h"
#include "pages/zen_test_mode.h"
#include "pages/zen_status.h"
#include "pages/zen_help.h"

#include "pages/zen_main_two.h"
#include "pages/zen_membrane.h"
#include "pages/zen_t1_t2.h"
#include "pages/zen_t1_t2_config_t1.h"
#include "pages/zen_t1_t2_config_t2.h"
#include "pages/zen_cycle_mode.h"
#include "pages/zen_cycle_time.h"
#include "pages/zen_aeration.h"
#include "pages/zen_aeration_air_on.h"
#include "pages/zen_aeration_air_off.h"
#include "pages/zen_aeration_a1.h"
#include "pages/zen_aeration_a2.h"
#include "pages/zen_overflow.h"
#include "pages/zen_sludge.h"

#include "helpers/sevensegment.h"


extern void zen_splash(void);

/* SHARED IMAGES */
extern gdispImage ic_back;
extern gdispImage ic_cancel;
extern gdispImage ic_done;


#endif /* _ZEN_MENU_H_ */