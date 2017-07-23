HW_SRC = ./src/hw

CSRC += $(HW_SRC)/uart/uart.c \
		$(HW_SRC)/ethernet/eth_if.c \
		$(HW_SRC)/rtc/rtc_clock.c \

INCDIR += $(HW_SRC)/uart \
		  $(HW_SRC)/ethernet \
		  $(HW_SRC)/rtc \

