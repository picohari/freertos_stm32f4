GFXINC  += $(GFXLIB)/boards/base/STM32F407i-XCore
GFXSRC  +=
GFXDEFS += 

#include $(GFXLIB)/drivers/gdisp/QImage/driver.mk
include $(GFXLIB)/drivers/gdisp/ILI93xx/driver.mk
include $(GFXLIB)/drivers/ginput/touch/ADS7843/driver.mk
include $(GFXLIB)/drivers/ginput/toggle/hal/driver.mk


#include $(GFXLIB)/drivers/gdisp/ILI9325/driver.mk
