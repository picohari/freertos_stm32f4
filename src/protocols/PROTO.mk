PROTO_SRC = ./src/protocols

CSRC += $(PROTO_SRC)/dhcp/dhcp_eth.c \
        #$(PROTO_SRC)/httplog/httplog.c \

INCDIR += $(PROTO_SRC)/dhcp \
          #$(PROTO_SRC)/httplog \

