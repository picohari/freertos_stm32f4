# FreeRTOS library path
LIB_FREERTOS = ./lib/FreeRTOS-9.0.0/Source

# Portable settings
FREE_RTOS_PORT      = GCC/ARM_CM4F
FREE_RTOS_HEAP      = heap_4

# FreeRTOS files
CSRC += $(LIB_FREERTOS)/croutine.c \
		$(LIB_FREERTOS)/event_groups.c \
		$(LIB_FREERTOS)/list.c  \
		$(LIB_FREERTOS)/queue.c  \
		$(LIB_FREERTOS)/tasks.c  \
		$(LIB_FREERTOS)/timers.c  \

# FreeRTOS CPU architecture
CSRC += $(LIB_FREERTOS)/portable/$(FREE_RTOS_PORT)/port.c \

# FreeRTOS HEAP management
CSRC += $(LIB_FREERTOS)/portable/MemMang/$(FREE_RTOS_HEAP).c \

# FreeRTOS CMSIS
CSRC += $(LIB_FREERTOS)/CMSIS_RTOS/cmsis_os.c \

# FreeRTOS Includes
INCDIR += $(LIB_FREERTOS)/include \
		  $(LIB_FREERTOS)/portable/$(FREE_RTOS_PORT) \
		  $(LIB_FREERTOS)/CMSIS_RTOS \
