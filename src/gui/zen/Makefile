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
	GFXLIB				= ../../../lib/ugfx_2.8
	GFXBOARD			= Linux-X
	GFXDEMO				=

# Linux settings
	# See $(GFXLIB)/tools/gmake_scripts/os_linux.mk for the list of variables

SIMDIR = .


##############################################################################################
# Set these for your project
#

ARCH     =
SRCFLAGS = -Os
CFLAGS   = -fstack-usage
CXXFLAGS =
ASFLAGS  =
LDFLAGS  = -ffunction-sections -fdata-sections -fno-common

SRC      =
OBJS     =
DEFS     =
LIBS     =
INCPATH  =
LIBPATH  =

##############################################################################################
# These should be at the end
#

GFXINC +=  $(SIMDIR) \
		   pages/ \
		   pixmaps/ \

GFXSRC +=  skin/zenstyle.c \
		   gui.c \
		   gui_router.c \
	   	   helpers/sevensegment.c \
	   	   helpers/date_and_time_util.c \
	   	   helpers/network_util.c \
	   	   helpers/membrane_util.c \
	   	   pages/zen_splash.c \
	   	   pages/zen_main_home.c \
	   	   pages/zen_main_one.c \
	   	   pages/zen_main_two.c \
	   	   pages/zen_config.c \
	   	   pages/zen_date_config.c \
	   	   pages/zen_time_config.c \
	   	   pages/zen_clean.c \
	   	   pages/zen_timers.c \
	   	   pages/zen_network.c \
	   	   pages/zen_network_ipv4.c \
	   	   pages/zen_network_gateway.c \
	   	   pages/zen_network_subnet_mask.c \
	   	   pages/zen_test_mode.c \
	   	   pages/zen_status.c \
	   	   pages/zen_help.c \
	   	   pages/zen_membrane.c \
	   	   pages/zen_t1_t2.c \
	   	   pages/zen_cycle_mode.c \
	   	   pages/zen_cycle_time.c \
	   	   pages/zen_aeration.c \
	   	   pages/zen_overflow.c \
	   	   pages/zen_sludge.c \

include $(GFXLIB)/tools/gmake_scripts/library_ugfx.mk
include $(GFXLIB)/tools/gmake_scripts/os_$(OPT_OS).mk
include $(GFXLIB)/tools/gmake_scripts/compiler_gcc.mk
# *** EOF ***
