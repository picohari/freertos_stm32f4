set(ROOT_PATH ${UGFX_ROOT}/src/ginput)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/ginput.c
    ${ROOT_PATH}/ginput_mouse.c
    ${ROOT_PATH}/ginput_keyboard.c
    ${ROOT_PATH}/ginput_keyboard_microcode.c
    ${ROOT_PATH}/ginput_toggle.c
    ${ROOT_PATH}/ginput_dial.c
)
