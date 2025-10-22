set(ROOT_PATH ${UGFX_ROOT}/drivers/multiple/SDL)

# Defaults
if (NOT UGFX_DEPENDENCY_SDL2_DOWNLOAD)
	set(UGFX_DEPENDENCY_SDL2_DOWNLOAD OFF)
endif()

# Get or find SDL2
if (UGFX_DEPENDENCY_SDL2_DOWNLOAD)
	include(FetchContent)
		
	FetchContent_Declare(
		SDL2
		GIT_REPOSITORY https://github.com/libsdl-org/SDL
		GIT_TAG        release-2.26.5
	)
	FetchContent_MakeAvailable(SDL2)

	if (NOT UGFX_DEPENDENCY_SDL2_TARGET)
		set(UGFX_DEPENDENCY_SDL2_TARGET SDL2-static)
	endif()
else()
	find_package(
		SDL2
		REQUIRED
	)

	if (NOT UGFX_DEPENDENCY_SDL2_TARGET)
		set(UGFX_DEPENDENCY_SDL2_TARGET SDL2::SDL2)
	endif()
endif()

list(APPEND ugfx_INCLUDE_DIRS
    ${ROOT_PATH}
)

list(APPEND ugfx_SOURCES
    ${ROOT_PATH}/gdisp_lld_SDL.c
)

list(APPEND ugfx_DEFS
    GFX_OS_PRE_INIT_FUNCTION=sdl_driver_init
)

list(APPEND ugfx_LIBS
	${UGFX_DEPENDENCY_SDL2_TARGET}
)

