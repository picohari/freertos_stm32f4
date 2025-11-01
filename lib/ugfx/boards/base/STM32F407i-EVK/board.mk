# Driver setup for WaveShare STM32F407i EVK Development Board https://www.waveshare.com/wiki/EVK407I
# The XCore MCU is a STM32F407IGT6
# TFT driver chip shipped on the 3.2 inch display is a ILI9325 (as per register read out)
# Must use the ILI93xx driver - all others didn't work. Maybe some china crap bug..
# Touchscreen driver is TSC2046 (which works with ADS7846 driver) attached to SPI2

GFXINC	+= $(GFXLIB)/boards/base/STM32F407i-EVK
GFXSRC	+=
#GFXDEFS += -GFX_USE_OS_FREERTOS=GFXON

# Basic driver for 320x240 TFT LCD controller ILI9325 in 16bit parallel mode
include $(GFXLIB)/drivers/gdisp/ILI93xx/driver.mk

# Driver for touchscreen controller: Board has TSC2046 (successor of ADS7846) via SPI
include $(GFXLIB)/drivers/ginput/touch/ADS7843/driver.mk

# Driver for joystick
include $(GFXLIB)/drivers/ginput/toggle/Hal/driver.mk

# Driver for analog input
include $(GFXLIB)/drivers/ginput/dial/GADC/driver.mk
