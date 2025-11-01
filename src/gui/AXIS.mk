
GUI_SRC = ./src/gui

CSRC += $(GUI_SRC)/axis/gui.c \
		$(GUI_SRC)/axis/gui_menu.c \
		$(GUI_SRC)/axis/modules/fast_text.c \
		$(GUI_SRC)/axis/modules/gfx_led.c \
		$(GUI_SRC)/axis/skin/style.c \
		$(GUI_SRC)/axis/pages/axis_helpers.c \
		$(GUI_SRC)/axis/pages/main_home.c \
		$(GUI_SRC)/axis/pages/menu_setup.c \
		$(GUI_SRC)/axis/pages/setup_remoteip.c \
		$(GUI_SRC)/axis/pages/setup_localip.c \
		$(GUI_SRC)/axis/pages/setup_netmask.c \
		$(GUI_SRC)/axis/pages/setup_gateway.c \
		$(GUI_SRC)/axis/pages/view_iostate.c \
		#$(GUI_SRC)/axis/tests/streaming_test.c \
        #$(GUI_SRC)/axis/display_update.c \
		#$(GUI_SRC)/axis/tests/fast_numbers.c \
        #$(GUI_SRC)/axis/modules/gwin_fastlabel.c \

INCDIR += $(GUI_SRC)/axis \
	      $(GUI_SRC)/axis/pages \
	      $(GUI_SRC)/axis/skin \
	      $(GUI_SRC)/axis/rsc \
	      $(GUI_SRC)/axis/modules \
	      #$(GUI_SRC)/axis/tests \







