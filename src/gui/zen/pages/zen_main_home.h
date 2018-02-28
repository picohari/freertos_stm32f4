/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _ZEN_HOME_H_
#define _ZEN_HOME_H_

#include "gfx.h"

#include "gui.h"
#include "gui_router.h"



/* LABELS */


/* IMAGES */


/* BUTTONS */
extern GHandle ghBtn_MENU;

/* FUNCTIONS */
void create_PageHome(void);
extern GHandle ghContainer_PageHome;
extern GUIWindow winMainHome;


extern GHandle ghLabelADCvalue;
extern GHandle ghLabelADCindicOK;
extern GHandle ghLabelADCindicFAIL;

extern GHandle ghScaleADCvalue;

/* SLIDERS */
extern GHandle ghSliderADCvalue;

#endif /* _ZEN_HOME_H_ */
