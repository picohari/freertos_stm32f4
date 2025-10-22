
GUI_SRC = ./src/gui

CSRC += $(GUI_SRC)/axis/gui.c \
        $(GUI_SRC)/axis/gui_router.c \
        $(GUI_SRC)/axis/display_update.c \
        $(GUI_SRC)/axis/modules/gwin_fastlabel.c \
		$(GUI_SRC)/axis/skin/style.c \
		$(GUI_SRC)/axis/pages/main_home.c \
		$(GUI_SRC)/axis/tests/streaming_test.c \
		$(GUI_SRC)/axis/tests/fast_numbers.c \

INCDIR += $(GUI_SRC)/axis \
	      $(GUI_SRC)/axis/pages \
	      $(GUI_SRC)/axis/skin \
	      $(GUI_SRC)/axis/rsc \
	      $(GUI_SRC)/axis/modules \
	      $(GUI_SRC)/axis/tests \
	      ./lib/ugfx/src/gdisp/mcufont/ \






