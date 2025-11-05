# LwIP library path
LIB_LWIP = ./lib/lwip-2.2.2

# LwIP source folder
LWIP_SRC = $(LIB_LWIP)/src

# Mapping for lwIP make
LWIPDIR = $(LWIP_SRC)

LWIP_INCLUDE_DIRS = $(LWIP_SRC)/include \
					$(LIB_LWIP)/system \
					./src/config \

# Basic lwIP files inclusion
include $(LWIP_SRC)/Filelists.mk

CSRC += $(LWIPNOAPPSFILES) \
		$(LWIPAPPFILES) \
		$(LIB_LWIP)/system/OS/sys_arch.c \

INCDIR += $(LWIP_INCLUDE_DIRS) \


# Application specific source files
CSRC += $(LWIP_SRC)/apps/http/fs_custom.c \

INCDIR += $(LWIP_SRC)/apps/http \

CSRC += $(LWIP_SRC)/apps/httpd/base64.c \
        $(LWIP_SRC)/apps/httpd/sha1_ws.c \
