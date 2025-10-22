GFXINC  += $(GFXLIB)/boards/base/STM32F407i-XCore
GFXSRC  +=
GFXDEFS += 

# Basic driver for 320x240 TFT LCD controller ILI9325 in 16bit parallel mode
include $(GFXLIB)/drivers/gdisp/ILI93xx/driver.mk

# Driver for touchscreen controller: Board has TSC2046 (successor of ADS7846) via SPI
include $(GFXLIB)/drivers/ginput/touch/ADS7843/driver.mk

# Driver for joystick
include $(GFXLIB)/drivers/ginput/toggle/hal/driver.mk

# Driver for analog input
include $(GFXLIB)/drivers/ginput/dial/hal/driver.mk


# Testmode for QT Application 
#include $(GFXLIB)/drivers/gdisp/QImage/driver.mk

# ChibiOS has own ADC driver, this was a test to see how to add own one :(
#include $(GFXLIB)/drivers/gadc/STM32F4/driver.mk
#include $(GFXLIB)/drivers/ginput/dial/GADC/driver.mk
#include $(GFXLIB)/drivers/gdisp/ILI9325/driver.mk
