set(ROOT_PATH ${UGFX_ROOT}/src/gos)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gos_chibios.c
    ${ROOT_PATH}/gos_freertos.c
    ${ROOT_PATH}/gos_win32.c
    ${ROOT_PATH}/gos_linux.c
    ${ROOT_PATH}/gos_osx.c
    ${ROOT_PATH}/gos_raw32.c
    ${ROOT_PATH}/gos_ecos.c
    ${ROOT_PATH}/gos_rawrtos.c
    ${ROOT_PATH}/gos_arduino.c
    ${ROOT_PATH}/gos_cmsis.c
    ${ROOT_PATH}/gos_nios.c
    ${ROOT_PATH}/gos_zephyr.c
    ${ROOT_PATH}/gos_x_threads.c
    ${ROOT_PATH}/gos_x_heap.c
)
