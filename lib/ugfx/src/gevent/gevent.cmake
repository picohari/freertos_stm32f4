set(ROOT_PATH ${UGFX_ROOT}/src/gevent)

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gevent.c
)
