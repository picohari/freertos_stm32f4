
LIB_FATFS = ./lib/FatFs/src

FATFS_DRIVER      = usbh_diskio.c


# FatFS basis files
CSRC += $(LIB_FATFS)/diskio.c \
		$(LIB_FATFS)/ff.c \
		$(LIB_FATFS)/ff_gen_drv.c  \
		$(LIB_FATFS)/option/unicode.c  \
		$(LIB_FATFS)/option/syscall.c  \


# FatFS DRIVER type
CSRC += $(LIB_FATFS)/drivers/$(FATFS_DRIVER) \


# FatFS Includes
INCDIR += $(LIB_FATFS)/ \
		  $(LIB_FATFS)/drivers/ \


