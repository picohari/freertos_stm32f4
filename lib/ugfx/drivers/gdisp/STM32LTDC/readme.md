# Usage
To use this driver:

1. Add in your `gfxconf.h`:
	a) `#define GFX_USE_GDISP		GFXON`

2. To your makefile add the following lines:
```
	include $(GFXLIB)/gfx.mk
	include $(GFXLIB)/drivers/gdisp/STM32LTDC/driver.mk
```

3. Add a `board_STM32LTDC.h` to you project directory (or board directory)
	based on one of the templates.


# Configuration
Configuration options available in `gfxconf.h`:

| Option                          | Default  | Description                                                                                                            |
|---------------------------------|----------|------------------------------------------------------------------------------------------------------------------------|
| `STM32LTDC_DMA_CACHE_FLUSH`     | `GFXOFF` | Whether to flush the DMA cache on DMA2D operations. This will be turned on automatically on certian platforms/systems. |
| `STM32LTDC_USE_DMA2D`           | `GFXON`  | Whether to use the DMA2D peripheral for hardware acceleration.                                                         |
| `STM32LTDC_USE_LAYER2`          | `GFXOFF` | Whether to use the 2nd LTDC layer. See the corresponding section below.                                                |
| `STM32LTDC_USE_DOUBLEBUFFERING` | `GFXOFF` | Whether to use double buffering. See the corresponding section below.                                                  |
| `STM32LTDC_USE_RGB565`          | `GFXOFF` | Whether to use RGB565 instead of RGB888.                                                                               |


# 2nd layer
To use the 2nd LTDC layer:
  - set `STM32LTDC_USE_LAYER2` to `GFXON` in `gfxconf.h`.
  - set `GDISP_TOTAL_DISPLAYS` to `2` in `gfxconf.h`.

The 2nd layer is exposed as a separate display. Use `gdispGetDisplay()` to retrieve the individual layers.

For more information, see:
  - https://wiki.ugfx.io/index.php/Multiple_displays#Example_-_Same_controller
  - https://wiki.ugfx.io/index.php/Multiple_displays#Access_the_displays


# Double buffering
To use double buffering:
  - set `STM32LTDC_USE_DOUBLEBUFFERING` to `GFXON` in `gfxconf.h`.
  - set `GDISP_TOTAL_DISPLAYS` to `2` in `gfxconf.h`.

When double buffering is enabled, the 2nd LTDC layer cannot be used. While this limitation is easy to remove from a software perspective, there is little benefit in doing so. Double buffering requires, as the name implies, twice the memory. If the 2nd layer would be used together with double buffering strategy, a total of four full framebuffers would be required. Years of real-world experience shows that there's practically never enough memory bandwidth available to do this.

To use double buffering in an application, retrieve the two buffers via `gdispGetDisplay()`.
Whenever a buffer swap is necessary, use `gdispGControl(g, STM32LTDC_CONTROL_SHOW_BUFFER, NULL);` where `g` is the buffer to be shown.

Simple example:
```c
#include "gfx.h"

static GDisplay* _display1;
static GDisplay* _display2;

// Requests a buffer swap on the driver level
static void _setActiveBuffer(GDisplay* g)
{
	gdispGControl(g, STM32LTDC_CONTROL_SHOW_BUFFER, NULL);
}

int main(void)
{
	// Initialize uGFX library
	gfxInit();
	
	// Get the two buffers
	_display1 = gdispGetDisplay(0);
	if (!_display1)
		gfxHalt("could not get display 1");
	_display2 = gdispGetDisplay(1);
	if (!_display2)
		gfxHalt("could not get display 2");

	// Render to each buffer
	gdispGClear(_display1, GFX_BLUE);
	gdispGClear(_display2, GFX_RED);

	// Switch between buffers
	while (gTrue) {
		gfxSleepMilliseconds(800);
		_setActiveBuffer(_display1);
		gfxSleepMilliseconds(800);
		_setActiveBuffer(_display2);
	}

	return 0;
}
```
