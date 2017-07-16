
PROJECT = stm32f4-firmware

##############################################################################
# CPU definitions and hardware settings
#
MCU_FAMILY        = STM32F4xx
MCU_MODEL_FAMILY  = STM32F407xx
MCU_MODEL         = STM32F407IGHx
MCU_CPU           = cortex-m4



# Hardware drivers for XCore STM32F407
include ./drv/CMSIS.mk
include ./drv/HAL.mk
include ./drv/BSP.mk

# Software libraries and features
include ./lib/FreeRTOS.mk
include ./lib/LwIP.mk
include ./lib/UGFX.mk
#include ./lib/UTIL.mk

# BIOS hardware and peripherals
include ./src/hw/HW.mk


# Application libraries and external modules
include ./src/gui/GUI.mk


CSRC += ./src/main.c \
		./src/stm32f4xx_it.c \
		./src/stm32f4xx_hal_msp.c \
#		./src/os/syscalls.c \
#		./src/syscalls.c \
#		./src/stm32f4xx_hal_msp.c \



INCDIR += ./src/config \
		  ./src \
		  ./src/os \
		  ./src/hw/uart \




GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)




##############################################################################
# Compiler settings
#

# Compiler options
USE_OPT = -Os -D$(MCU_MODEL_FAMILY) -DUSE_HAL_DRIVER -DUSE_STM324xG_EVAL -DVERSION=\"$(GIT_VERSION)\"

# -ggdb -fomit-frame-pointer -falign-functions=16 

# C specific options (added to USE_OPT).
USE_COPT = -fstack-usage

# C++ specific options (added to USE_OPT).
USE_CPPOPT = -fno-rtti

# Compile the application in THUMB mode.
USE_THUMB = yes

# ARM-specific options
AOPT =

# THUMB-specific options
TOPT = -mthumb -DTHUMB

TRGT = arm-none-eabi-

CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
LD   = $(TRGT)gcc
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
SZ   = $(TRGT)size
RELF = $(TRGT)readelf -e

HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# Define C warning options here
CWARN = -Wall -Wextra -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall -Wextra

# Verbose log while compiling
USE_VERBOSE_COMPILE = yes

##############################################################################
# Linker settings
#

# Linker garbage collection: ask linker to remove unused code and data
USE_LINK_GC = yes

# Linker extra options
USE_LDOPT = 

# Link time optimizations (LTO)
USE_LTO = no

# Linker script
LDSCRIPT= ./ld/$(MCU_MODEL)_FLASH.ld
#LDSCRIPT= ./ld/STM32F407.ld

##############################################################################
# OpenOCD section
#

# Location of OpenOCD Board .cfg files
OPENOCD_BOARD_DIR = /home/dk6yf/stm32/openocd/scripts/board

# OpenOCD board config file
OPENOCD_BOARD_CFG = stm32f7discovery.cfg


# ARM Cortex-Mx common makefile scripts and rules.






##############################################################################
# Processing options coming from the upper Makefile.
#

# Compiler options
OPT = $(USE_OPT)
COPT = $(USE_COPT)
CPPOPT = $(USE_CPPOPT)

# Garbage collection
ifeq ($(USE_LINK_GC),yes)
  OPT += -ffunction-sections -fdata-sections -fno-common
  LDOPT := ,--gc-sections
else
  LDOPT :=
endif

# Linker extra options
ifneq ($(USE_LDOPT),)
  LDOPT := $(LDOPT),$(USE_LDOPT)
endif

# Link time optimizations
ifeq ($(USE_LTO),yes)
  OPT += -flto
endif

# FPU-related options
ifeq ($(USE_FPU),)
  USE_FPU = yes
endif
ifneq ($(USE_FPU),no)
  OPT += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant
  DDEFS += -DCORTEX_USE_FPU=TRUE
  DADEFS += -DCORTEX_USE_FPU=TRUE
else
  DDEFS += -DCORTEX_USE_FPU=FALSE
  DADEFS += -DCORTEX_USE_FPU=FALSE
endif

# Output directory and files
ifeq ($(BUILDDIR),)
  BUILDDIR = build
endif
ifeq ($(BUILDDIR),.)
  BUILDDIR = build
endif
OUTFILES = $(BUILDDIR)/$(PROJECT).elf $(BUILDDIR)/$(PROJECT).hex \
           $(BUILDDIR)/$(PROJECT).bin $(BUILDDIR)/$(PROJECT).dmp

# Source files groups and paths
ifeq ($(USE_THUMB),yes)
  TCSRC += $(CSRC)
  TCPPSRC += $(CPPSRC)
else
  ACSRC += $(CSRC)
  ACPPSRC += $(CPPSRC)
endif
ASRC	  = $(ACSRC)$(ACPPSRC)
TSRC	  = $(TCSRC)$(TCPPSRC)
SRCPATHS  = $(sort $(dir $(ASMXSRC)) $(dir $(ASMSRC)) $(dir $(ASRC)) $(dir $(TSRC)))

# Various directories
OBJDIR    = $(BUILDDIR)/obj
LSTDIR    = $(BUILDDIR)/lst

# Object files groups
ACOBJS    = $(addprefix $(OBJDIR)/, $(notdir $(ACSRC:.c=.o)))
ACPPOBJS  = $(addprefix $(OBJDIR)/, $(notdir $(ACPPSRC:.cpp=.o)))
TCOBJS    = $(addprefix $(OBJDIR)/, $(notdir $(TCSRC:.c=.o)))
TCPPOBJS  = $(addprefix $(OBJDIR)/, $(notdir $(TCPPSRC:.cpp=.o)))
ASMOBJS   = $(addprefix $(OBJDIR)/, $(notdir $(ASMSRC:.s=.o)))
ASMXOBJS  = $(addprefix $(OBJDIR)/, $(notdir $(ASMXSRC:.S=.o)))
OBJS	  = $(ASMXOBJS) $(ASMOBJS) $(ACOBJS) $(TCOBJS) $(ACPPOBJS) $(TCPPOBJS)

# Paths
IINCDIR   = $(patsubst %,-I%,$(INCDIR) $(DINCDIR) $(UINCDIR))
LLIBDIR   = $(patsubst %,-L%,$(DLIBDIR) $(ULIBDIR))

# Macros
DEFS      = $(DDEFS) $(UDEFS)
ADEFS 	  = $(DADEFS) $(UADEFS)

# Libs
LIBS      = $(DLIBS) $(ULIBS)

# Various settings
MCFLAGS   = -mcpu=$(MCU_CPU)
ODFLAGS	  = -x --syms
ASFLAGS   = $(MCFLAGS) -Wa,-amhls=$(LSTDIR)/$(notdir $(<:.s=.lst)) $(ADEFS)
ASXFLAGS  = $(MCFLAGS) -Wa,-amhls=$(LSTDIR)/$(notdir $(<:.S=.lst)) $(ADEFS)
CFLAGS    = $(MCFLAGS) $(OPT) $(COPT) $(CWARN) -Wa,-alms=$(LSTDIR)/$(notdir $(<:.c=.lst)) $(DEFS)
CPPFLAGS  = $(MCFLAGS) $(OPT) $(CPPOPT) $(CPPWARN) -Wa,-alms=$(LSTDIR)/$(notdir $(<:.cpp=.lst)) $(DEFS)

#LDFLAGS   = $(MCFLAGS) $(OPT) -nostartfiles $(LLIBDIR) -Wl,-Map=$(BUILDDIR)/$(PROJECT).map,--cref,--no-warn-mismatch,--script=$(LDSCRIPT)$(LDOPT)
LDFLAGS   = $(MCFLAGS) $(OPT) $(LLIBDIR) -Wl,-Map=$(BUILDDIR)/$(PROJECT).map,--cref,--no-warn-mismatch,--script=$(LDSCRIPT)$(LDOPT)


# Thumb interwork enabled only if needed because it kills performance.
ifneq ($(TSRC),)
  CFLAGS   += -DTHUMB_PRESENT
  CPPFLAGS += -DTHUMB_PRESENT
  ASFLAGS  += -DTHUMB_PRESENT
  ifneq ($(ASRC),)
    # Mixed ARM and THUMB mode.
    CFLAGS   += -mthumb-interwork
    CPPFLAGS += -mthumb-interwork
    ASFLAGS  += -mthumb-interwork
    LDFLAGS  += -mthumb-interwork
  else
    # Pure THUMB mode, THUMB C code cannot be called by ARM asm code directly.
    CFLAGS   += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    CPPFLAGS += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING
    ASFLAGS  += -mno-thumb-interwork -DTHUMB_NO_INTERWORKING -mthumb
    LDFLAGS  += -mno-thumb-interwork -mthumb
  endif
else
  # Pure ARM mode
  CFLAGS   += -mno-thumb-interwork
  CPPFLAGS += -mno-thumb-interwork
  ASFLAGS  += -mno-thumb-interwork
  LDFLAGS  += -mno-thumb-interwork
endif

# Generate dependency information
ASFLAGS  += -MD -MP -MF .dep/$(@F).d
CFLAGS   += -MD -MP -MF .dep/$(@F).d
CPPFLAGS += -MD -MP -MF .dep/$(@F).d

# Paths where to search for sources
VPATH     = $(SRCPATHS)






#
# Makefile rules
#

all: $(OBJS) $(OUTFILES) MAKE_ALL_RULE_HOOK


MAKE_ALL_RULE_HOOK:


$(OBJS): | $(BUILDDIR)


$(BUILDDIR) $(OBJDIR) $(LSTDIR):
ifneq ($(USE_VERBOSE_COMPILE),yes)
	@echo Compiler Options
	@echo $(CC) -c $(CFLAGS) -I. $(IINCDIR) main.c -o main.o
	@echo
endif
	mkdir -p $(OBJDIR)
	mkdir -p $(LSTDIR)

$(ACPPOBJS) : $(OBJDIR)/%.o : %.cpp Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CPPC) -c $(CPPFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
endif

$(TCPPOBJS) : $(OBJDIR)/%.o : %.cpp Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CPPC) -c $(CPPFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CPPC) -c $(CPPFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
endif

$(ACOBJS) : $(OBJDIR)/%.o : %.c Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CC) -c $(CFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CC) -c $(CFLAGS) $(AOPT) -I. $(IINCDIR) $< -o $@
endif

$(TCOBJS) : $(OBJDIR)/%.o : %.c Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CC) -c $(CFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CC) -c $(CFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
endif

$(ASMOBJS) : $(OBJDIR)/%.o : %.s Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(AS) -c $(ASFLAGS) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(AS) -c $(ASFLAGS) -I. $(IINCDIR) $< -o $@
endif

$(ASMXOBJS) : $(OBJDIR)/%.o : %.S Makefile
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(CC) -c $(ASXFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
else
	@echo Compiling $(<F)
	@$(CC) -c $(ASXFLAGS) $(TOPT) -I. $(IINCDIR) $< -o $@
endif

%.elf: $(OBJS) $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo
	$(LD) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
else
	@echo Linking $@
	@$(LD) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
endif

%.hex: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(HEX) $< $@
else
	@echo Creating $@
	@$(HEX) $< $@
endif

%.bin: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	$(BIN) $< $@
else
	@echo Creating $@
	@$(BIN) $< $@
endif

%.dmp: %.elf $(LDSCRIPT)
ifeq ($(USE_VERBOSE_COMPILE),yes)
	@echo Creating $@
	$(OD) $(ODFLAGS) $< > $@
	@echo
	@$(SZ) $<
	@echo
	@$(RELF) $<
	@echo
	@echo Done
else
	@echo Creating $@
	@$(OD) $(ODFLAGS) $< > $@
	@echo
	@$(SZ) $<
	@echo
	@$(RELF) $<
	@echo
	@echo Done
endif

clean:
	@echo Cleaning
	-rm -fR .dep $(BUILDDIR)
	@echo
	@echo Done

#
# Include the dependency files, should be the last of the makefile
#
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)


# Flashes your board using OpenOCD
flash: all
	openocd -f $(OPENOCD_BOARD_DIR)/$(OPENOCD_BOARD_CFG) -f openocd/stm32f0-flash.cfg \
            -c "stm_flash `pwd`/$(BUILDDIR)/$(PROJECT).bin" -c shutdown

# *** EOF ***
