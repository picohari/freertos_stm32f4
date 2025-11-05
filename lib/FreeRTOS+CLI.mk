# FreeRTOS CLI library path
CLI_SRC = ./lib/FreeRTOS-Plus-CLI

# This one has only one file
CSRC += $(CLI_SRC)/FreeRTOS_CLI.c \

# Include self
INCDIR += $(CLI_SRC) \

