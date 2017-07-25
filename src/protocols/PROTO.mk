PROTO_SRC = ./src/protocols

CSRC += $(PROTO_SRC)/dhcp/proto_dhcp.c \
        $(PROTO_SRC)/httplog/httplog.c \

INCDIR += $(PROTO_SRC)/dhcp \
          $(PROTO_SRC)/httplog \

