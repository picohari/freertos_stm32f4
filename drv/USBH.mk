
LIB_USBH = ./drv/STM32_USB_Host_Library

# USB Core library
CSRC += $(LIB_USBH)/Core/Src/usbh_core.c \
        $(LIB_USBH)/Core/Src/usbh_ctlreq.c \
        $(LIB_USBH)/Core/Src/usbh_ioreq.c \
        $(LIB_USBH)/Core/Src/usbh_pipes.c \


INCDIR += $(LIB_USBH)/Core/Inc \



# MSC Class library
CSRC += $(LIB_USBH)/Class/MSC/Src/usbh_msc.c \
        $(LIB_USBH)/Class/MSC/Src/usbh_msc_bot.c \
        $(LIB_USBH)/Class/MSC/Src/usbh_msc_scsi.c \


INCDIR += $(LIB_USBH)/Class/MSC/Inc \

