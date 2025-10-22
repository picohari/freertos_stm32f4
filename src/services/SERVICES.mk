SERVICES_SRC = ./src/services

CSRC += $(SERVICES_SRC)/httpd/httpd_server.c \
		$(SERVICES_SRC)/httpd/test_server.c \
		$(SERVICES_SRC)/jogctrl/udp_connect.c \

INCDIR += $(SERVICES_SRC)/httpd \
		  $(SERVICES_SRC)/jogctrl \

