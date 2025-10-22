set(ROOT_PATH ${UGFX_ROOT}/drivers/multiple/Win32)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gdisp_lld_Win32.c
)

list(APPEND ugfx_LIBS
	gdi32
)
