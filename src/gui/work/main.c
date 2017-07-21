#include "gui.h"
#include "gfx.h"

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	gfxInit();

	gdispSetBacklight(100);
	gdispSetContrast(100);

	geventListenerInit(&glistener);
	gwinAttachListener(&glistener);

	geventAttachSource(&glistener, ginputGetKeyboard(0), 0);

	guiCreate();

	while (1) {
		guiEventLoop();
	}

	return 0;
}
