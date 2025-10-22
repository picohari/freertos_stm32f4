# uGFX library path
GFXLIB = ./lib/ugfx

# uGFX main makefile skript
include $(GFXLIB)/gfx.mk

# uGFX driver and boad skript
include $(GFXLIB)/boards/base/STM32F407i-EVK/board.mk

# uGFX source and ..
CSRC   += $(GFXSRC) \

# include directories
INCDIR += $(GFXINC) \
