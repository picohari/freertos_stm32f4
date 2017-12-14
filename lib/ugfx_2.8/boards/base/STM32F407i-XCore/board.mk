GFXINC  += $(GFXLIB)/boards/base/STM32F407i-XCore
GFXSRC  +=
GFXDEFS += 

#include $(GFXLIB)/drivers/gdisp/QImage/driver.mk
include $(GFXLIB)/drivers/gdisp/ILI93xx/driver.mk
include $(GFXLIB)/drivers/ginput/touch/ADS7843/driver.mk
include $(GFXLIB)/drivers/ginput/toggle/hal/driver.mk
include $(GFXLIB)/drivers/ginput/dial/hal/driver.mk




# ChibiOS has own ADC driver, this was a test to see how to add own one :(
#include $(GFXLIB)/drivers/gadc/STM32F4/driver.mk
#include $(GFXLIB)/drivers/ginput/dial/GADC/driver.mk


#include $(GFXLIB)/drivers/gdisp/ILI9325/driver.mk
