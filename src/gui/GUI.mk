
GUI_SRC = ./src/gui

CSRC += $(GUI_SRC)/vt100/vt100.c \
        $(GUI_SRC)/zen/gui.c \
        $(GUI_SRC)/zen/gui_router.c \
        $(GUI_SRC)/zen/skin/zenstyle.c \
		$(GUI_SRC)/zen/helpers/date_and_time_util.c \
		$(GUI_SRC)/zen/helpers/network_util.c \
		$(GUI_SRC)/zen/helpers/sevensegment.c \
		$(GUI_SRC)/zen/helpers/zen_util.c \
		$(GUI_SRC)/zen/pages/zen_aeration.c \
		$(GUI_SRC)/zen/pages/zen_aeration_a1.c \
		$(GUI_SRC)/zen/pages/zen_aeration_a2.c \
		$(GUI_SRC)/zen/pages/zen_aeration_air_off.c \
		$(GUI_SRC)/zen/pages/zen_aeration_air_on.c \
		$(GUI_SRC)/zen/pages/zen_clean.c \
		$(GUI_SRC)/zen/pages/zen_config.c \
		$(GUI_SRC)/zen/pages/zen_config_date.c \
		$(GUI_SRC)/zen/pages/zen_config_time.c \
		$(GUI_SRC)/zen/pages/zen_cycle_mode.c \
		$(GUI_SRC)/zen/pages/zen_cycle_time.c \
		$(GUI_SRC)/zen/pages/zen_help.c \
		$(GUI_SRC)/zen/pages/zen_main_home.c \
		$(GUI_SRC)/zen/pages/zen_main_one.c \
		$(GUI_SRC)/zen/pages/zen_main_two.c \
		$(GUI_SRC)/zen/pages/zen_membrane.c \
		$(GUI_SRC)/zen/pages/zen_network.c \
		$(GUI_SRC)/zen/pages/zen_network_gateway.c \
		$(GUI_SRC)/zen/pages/zen_network_ipv4.c \
		$(GUI_SRC)/zen/pages/zen_network_subnet_mask.c \
		$(GUI_SRC)/zen/pages/zen_overflow.c \
		$(GUI_SRC)/zen/pages/zen_sludge.c \
		$(GUI_SRC)/zen/pages/zen_splash.c \
		$(GUI_SRC)/zen/pages/zen_status.c \
		$(GUI_SRC)/zen/pages/zen_t1_t2.c \
		$(GUI_SRC)/zen/pages/zen_t1_t2_config_t1.c \
		$(GUI_SRC)/zen/pages/zen_t1_t2_config_t2.c \
		$(GUI_SRC)/zen/pages/zen_test_mode.c \
		$(GUI_SRC)/zen/pages/zen_timers.c \


CSRC += ./src/cli/cli_commands.c \

INCDIR += $(GUI_SRC)/vt100 \
		  $(GUI_SRC)/zen \
		  $(GUI_SRC)/zen/helpers \
		  $(GUI_SRC)/zen/pages \
		  $(GUI_SRC)/zen/skin \
		  #$(GUI_SRC)/cli \




