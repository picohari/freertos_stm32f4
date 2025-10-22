set(ROOT_PATH ${UGFX_ROOT}/src/gfile)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gfile.c
    ${ROOT_PATH}/gfile_fs_native.c
    ${ROOT_PATH}/gfile_fs_ram.c
    ${ROOT_PATH}/gfile_fs_rom.c
    ${ROOT_PATH}/gfile_fs_fatfs.c
    ${ROOT_PATH}/gfile_fs_petitfs.c
    ${ROOT_PATH}/gfile_fs_mem.c
    ${ROOT_PATH}/gfile_fs_chibios.c
    ${ROOT_PATH}/gfile_fs_strings.c
    ${ROOT_PATH}/gfile_printg.c
    ${ROOT_PATH}/gfile_scang.c
    ${ROOT_PATH}/gfile_stdio.c
    ${ROOT_PATH}/gfile_fatfs_wrapper.c
    ${ROOT_PATH}/gfile_fatfs_diskio_chibios.c
    ${ROOT_PATH}/gfile_petitfs_wrapper.c
    ${ROOT_PATH}/gfile_petitfs_diskio_chibios.c
)
