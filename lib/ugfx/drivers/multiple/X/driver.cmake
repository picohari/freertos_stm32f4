set(ROOT_PATH ${UGFX_ROOT}/drivers/multiple/X)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gdisp_lld_X.c
)

list(APPEND ugfx_LIBS
	X11
)

