#include "test.h"
#include "gfx.h"

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	gfxInit();

	gdispSetBacklight(100);
	gdispSetContrast(100);

	guiCreate();

	while (1) {
		guiEventLoop();
	}

	return 0;
}
