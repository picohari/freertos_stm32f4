# Define UGFX_ROOT
set(UGFX_ROOT ${CMAKE_CURRENT_LIST_DIR}/..)

# Ensure that UGFX_ROOT is set
if(NOT UGFX_ROOT)
    message(FATAL_ERROR "No UGFX_ROOT specified")
endif()

# Assemble list of components
list(APPEND ugfx_COMPONENTS_BUILTIN
    gadc
    gaudio
    gdisp
    gdriver
    gevent
    gfile
    ginput
    gmisc
    gos
    gqueue
    gtimer
    gtrans
    gwin
)

# Core sources
list(APPEND ugfx_SOURCES
    ${UGFX_ROOT}/src/gfx.c
)

# Core include directories
list(APPEND ugfx_INCLUDE_DIRS
	${UGFX_ROOT}
    ${UGFX_ROOT}/src
)

# Include each built-in component
foreach(component ${ugfx_COMPONENTS_BUILTIN})
    include(${UGFX_ROOT}/src/${component}/${component}.cmake)
endforeach()

# Include each component
foreach(component ${ugfx_FIND_COMPONENTS})
	include(${UGFX_ROOT}/${component}/driver.cmake)
endforeach()

# Remove duplicates from non-cached variables
list(REMOVE_DUPLICATES ugfx_SOURCES)
list(REMOVE_DUPLICATES ugfx_INCLUDE_DIRS)

# Outsource heavy-lifting to cmake
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ugfx DEFAULT_MSG UGFX_ROOT ugfx_SOURCES ugfx_INCLUDE_DIRS)

# Create the target
if(NOT TARGET ugfx)
	add_library(ugfx INTERFACE IMPORTED)
endif()
target_include_directories(
	ugfx
    INTERFACE
        ${ugfx_INCLUDE_DIRS}
)
target_sources(
	ugfx
    INTERFACE
        ${ugfx_SOURCES}
)
target_compile_definitions(
	ugfx
    INTERFACE
        ${ugfx_DEFS}
)
target_link_libraries(
	ugfx
    INTERFACE
        ${ugfx_LIBS}
)
