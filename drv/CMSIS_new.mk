# CPU & STARTUP CMSIS
LIB_CMSIS = ./drv/CMSIS-5.0.4

MCU_FAMILY_LOWERCASE = $(shell echo $(MCU_FAMILY) | tr '[:upper:]' '[:lower:]')

MCU_MODEL_FAMILY_LOWERCASE  = $(shell echo $(MCU_MODEL_FAMILY) | tr '[:upper:]' '[:lower:]')

USE_OPT += -DCMSIS_device_header=\"$(MCU_FAMILY_LOWERCASE).h\"

CSRC   += $(LIB_CMSIS)/Device/ST/$(MCU_FAMILY)/Source/Templates/system_$(MCU_FAMILY_LOWERCASE).c

ASMSRC += $(LIB_CMSIS)/Device/ST/$(MCU_FAMILY)/Source/Templates/gcc/startup_$(MCU_MODEL_FAMILY_LOWERCASE).s

INCDIR += $(LIB_CMSIS)/Device/ST/$(MCU_FAMILY)/Include \
          $(LIB_CMSIS)/Include \
