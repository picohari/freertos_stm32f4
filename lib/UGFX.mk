GFXLIB = ./lib/ugfx

include $(GFXLIB)/gfx.mk

include $(GFXLIB)/boards/base/STM32F407i-XCore/board.mk


CSRC   += $(GFXSRC) \

INCDIR += $(GFXINC) \


OPT_OS				= freertos
