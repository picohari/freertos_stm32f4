HW_SRC = ./src/hw

CSRC += $(HW_SRC)/uart/uart.c \
		$(HW_SRC)/ethernet/eth_if.c \
		$(HW_SRC)/rtc/rtc_clock.c \
		$(HW_SRC)/nand_flash/flashdrive.c \
		$(HW_SRC)/usbh/usbh_conf.c \
		$(HW_SRC)/adc/adc.c \
		$(HW_SRC)/onewire/onewire.c \
		$(HW_SRC)/onewire/ds18b20.c \

INCDIR += $(HW_SRC)/uart \
		  $(HW_SRC)/ethernet \
		  $(HW_SRC)/rtc \
		  $(HW_SRC)/nand_flash \
		  $(HW_SRC)/usbh \
		  $(HW_SRC)/adc \
		  $(HW_SRC)/onewire \

