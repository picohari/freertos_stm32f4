set(ROOT_PATH ${UGFX_ROOT}/src/gdisp)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
    ${ROOT_PATH}/mcufont
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gdisp.c
    ${ROOT_PATH}/gdisp_fonts.c
    ${ROOT_PATH}/gdisp_pixmap.c
    ${ROOT_PATH}/gdisp_image.c
    ${ROOT_PATH}/gdisp_image_native.c
    ${ROOT_PATH}/gdisp_image_gif.c
    ${ROOT_PATH}/gdisp_image_bmp.c
    ${ROOT_PATH}/gdisp_image_jpg.c
    ${ROOT_PATH}/gdisp_image_png.c

    ${ROOT_PATH}/mcufont/mf_encoding.c
    ${ROOT_PATH}/mcufont/mf_font.c
    ${ROOT_PATH}/mcufont/mf_justify.c
    ${ROOT_PATH}/mcufont/mf_kerning.c
    ${ROOT_PATH}/mcufont/mf_rlefont.c
    ${ROOT_PATH}/mcufont/mf_bwfont.c
    ${ROOT_PATH}/mcufont/mf_scaledfont.c
    ${ROOT_PATH}/mcufont/mf_wordwrap.c
)
