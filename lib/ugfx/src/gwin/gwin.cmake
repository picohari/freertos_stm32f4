set(ROOT_PATH ${UGFX_ROOT}/src/gwin)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
    ${UGFX_ROOT}/3rdparty/tinygl-0.4-ugfx/include
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gwin.c
    ${ROOT_PATH}/gwin_widget.c
    ${ROOT_PATH}/gwin_wm.c
    ${ROOT_PATH}/gwin_console.c
    ${ROOT_PATH}/gwin_graph.c
    ${ROOT_PATH}/gwin_button.c
    ${ROOT_PATH}/gwin_slider.c
    ${ROOT_PATH}/gwin_checkbox.c
    ${ROOT_PATH}/gwin_image.c
    ${ROOT_PATH}/gwin_label.c
    ${ROOT_PATH}/gwin_radio.c
    ${ROOT_PATH}/gwin_list.c
    ${ROOT_PATH}/gwin_progressbar.c
    ${ROOT_PATH}/gwin_progressbar.c
    ${ROOT_PATH}/gwin_container.c
    ${ROOT_PATH}/gwin_frame.c
    ${ROOT_PATH}/gwin_tabset.c
    ${ROOT_PATH}/gwin_gl3d.c
    ${ROOT_PATH}/gwin_keyboard.c
    ${ROOT_PATH}/gwin_keyboard_layout.c
    ${ROOT_PATH}/gwin_textedit.c
)
