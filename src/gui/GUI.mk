
GUI_SRC = ./src/gui

CSRC += $(GUI_SRC)/vt100/vt100.c \
        $(GUI_SRC)/zen/gui.c \
        $(GUI_SRC)/zen/gui_router.c \
        $(GUI_SRC)/zen/skin/zenstyle.c \
        $(GUI_SRC)/zen/helpers/sevensegment.c \
        $(GUI_SRC)/zen/pages/zen_splash.c \
        $(GUI_SRC)/zen/pages/zen_main_home.c \
        $(GUI_SRC)/zen/pages/zen_main_one.c \
        $(GUI_SRC)/zen/pages/zen_main_two.c \

CSRC += ./src/cli/cli_commands.c \

INCDIR += $(GUI_SRC)/vt100 \
		  $(GUI_SRC)/zen \
		  $(GUI_SRC)/zen/helpers \
		  $(GUI_SRC)/zen/pages \
		  $(GUI_SRC)/zen/skin \
		  #$(GUI_SRC)/cli \




