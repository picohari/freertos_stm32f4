# Possible Targets:	all clean Debug cleanDebug Release cleanRelease

##############################################################################################
# Settings
#

# General settings
	# See $(GFXLIB)/tools/gmake_scripts/readme.txt for the list of variables
	OPT_OS				= linux
	OPT_LINK_OPTIMIZE	= yes
	# Change this next setting (or add the explicit compiler flags) if you are not compiling for x86 linux
	OPT_CPU				= x86

# uGFX settings
	# See $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk for the list of variables
	GFXLIB				= ../../../lib/ugfx
	GFXBOARD			= Linux-X
	GFXDEMO				=

# Linux settings
	# See $(GFXLIB)/tools/gmake_scripts/os_linux.mk for the list of variables

SIMDIR = .

GFXINC +=  $(SIMDIR) \
           $(SIMDIR)/pages \

GFXSRC +=  $(SIMDIR)/main.c \
           $(SIMDIR)/gui.c \
           $(SIMDIR)/pages/pages.c \

##############################################################################################
# Set these for your project
#

ARCH     =
SRCFLAGS = -Os
CFLAGS   = -fstack-usage
CXXFLAGS =
ASFLAGS  =
LDFLAGS  = -ffunction-sections -fdata-sections -fno-common -lm

SRC      =
OBJS     =
DEFS     =
LIBS     =
INCPATH  =
LIBPATH  =

##############################################################################################
# These should be at the end
#

include $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk
include $(GFXLIB)/tools/gmake_scripts/os_$(OPT_OS).mk
include $(GFXLIB)/tools/gmake_scripts/compiler_gcc.mk
# *** EOF ***
