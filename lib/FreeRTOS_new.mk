# FreeRTOS library path
LIB_FREERTOS = ./lib/FreeRTOS-10.3.1/Source

# FreeRTOS CMSIS
LIB_CMSIS_RTOS = ./drv/CMSIS_RTOS_V2

# Portable settings
FREE_RTOS_PORT      = GCC/ARM_CM4F
FREE_RTOS_HEAP      = heap_4

# FreeRTOS files
CSRC += $(LIB_FREERTOS)/croutine.c \
		$(LIB_FREERTOS)/event_groups.c \
		$(LIB_FREERTOS)/list.c  \
		$(LIB_FREERTOS)/queue.c  \
		$(LIB_FREERTOS)/stream_buffer.c  \
		$(LIB_FREERTOS)/tasks.c  \
		$(LIB_FREERTOS)/timers.c  \

# FreeRTOS CPU architecture
CSRC += $(LIB_FREERTOS)/portable/$(FREE_RTOS_PORT)/port.c \

# FreeRTOS HEAP management
CSRC += $(LIB_FREERTOS)/portable/MemMang/$(FREE_RTOS_HEAP).c \

# FreeRTOS Includes
INCDIR += $(LIB_FREERTOS)/include \
		  $(LIB_FREERTOS)/portable/$(FREE_RTOS_PORT) \


# FreeRTOS CMSIS
CSRC    += $(LIB_CMSIS_RTOS)/cmsis_os2.c \

INCDIR  += $(LIB_CMSIS_RTOS)/ \
