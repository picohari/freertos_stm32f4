
GUI_SRC = ./src/gui

CSRC += $(GUI_SRC)/vt100/vt100.c \
        $(GUI_SRC)/zen/gui.c \
        $(GUI_SRC)/zen/pages/zen_splash.c \

CSRC += ./src/cli/cli_commands.c \

INCDIR += $(GUI_SRC)/vt100 \
		  $(GUI_SRC)/zen \
		  $(GUI_SRC)/zen/pages \
		  #$(GUI_SRC)/cli \




