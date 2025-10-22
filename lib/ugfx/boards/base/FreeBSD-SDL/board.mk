GFXINC  +=
GFXSRC  +=
GFXDEFS += -DGFX_USE_OS_FREEBSD=GFXON
GFXLIBS += rt

include $(GFXLIB)/drivers/multiple/SDL/driver.mk
