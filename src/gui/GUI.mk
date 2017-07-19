
GUI = ./src/gui

CSRC += $(GUI)/vt100/vt100.c \
        $(GUI)/zen/gui.c \
        $(GUI)/zen/pages/zen_splash.c \
        #$(GUI)/cli/cli_commands.c \


INCDIR += $(GUI)/vt100 \
		  $(GUI)/zen \
		  $(GUI)/zen/pages \
		  #$(GUI)/cli \




