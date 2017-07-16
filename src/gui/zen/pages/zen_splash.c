
#include "pages/zen_menu.h"


void zen_splash(void) {

	static gdispImage myImage;

	gdispImageOpenFile(&myImage, "rsc/logo-white.gif");

	gdispClear(White);

	gdispImageDraw(&myImage, 21, 50, 278, 100, 0, 0);

	gdispImageClose(&myImage);

	gfxSleepMilliseconds(2000);

	gdispClear(Black);

}


