/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _AXIS_HOME_H_
#define _AXIS_HOME_H_

#include "gfx.h"

#include "gui.h"
#include "gui_router.h"


/* VARIABLES */
extern GHandle ghContainer_PageHome;
extern GUIWindow winMainHome;


/* LABELS */
extern GHandle ghLabelG53_X;    // G53: current machine coordinates
extern GHandle ghLabelG53_Y;
extern GHandle ghLabelG53_Z;

/* Handles */
extern GHandle ghFastLabelX;
extern GHandle ghFastLabelY;
extern GHandle ghFastLabelZ;

extern GHandle ghLabelRotary;

/* IMAGES */


/* BUTTONS */
extern GHandle ghBtn_MENU;


/* FUNCTIONS */
void create_PageHome(void);


#endif /* _AXIS_HOME_H_ */
