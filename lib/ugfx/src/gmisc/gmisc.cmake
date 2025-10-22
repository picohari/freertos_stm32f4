set(ROOT_PATH ${UGFX_ROOT}/src/gmisc)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gmisc.c
    ${ROOT_PATH}/gmisc_arrayops.c
    ${ROOT_PATH}/gmisc_matrix2d.c
    ${ROOT_PATH}/gmisc_trig.c
    ${ROOT_PATH}/gmisc_hittest.c
)
