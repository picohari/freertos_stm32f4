
LIB_CMSIS = ./drv/CMSIS

MCU_FAMILY_LOWERCASE = $(shell echo $(MCU_FAMILY) | tr '[:upper:]' '[:lower:]')

MCU_MODEL_FAMILY_LOWERCASE  = $(shell echo $(MCU_MODEL_FAMILY) | tr '[:upper:]' '[:lower:]')



CSRC   += $(LIB_CMSIS)/$(MCU_FAMILY)/src/system_$(MCU_FAMILY_LOWERCASE).c

ASMSRC += $(LIB_CMSIS)/$(MCU_FAMILY)/src/startup_$(MCU_MODEL_FAMILY_LOWERCASE).s

INCDIR += $(LIB_CMSIS)/$(MCU_FAMILY)/inc \
          $(LIB_CMSIS)/inc \
