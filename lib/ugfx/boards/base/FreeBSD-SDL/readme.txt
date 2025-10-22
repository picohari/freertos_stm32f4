This directory contains the interface for FreeBSD using SDL.

As of today, this is simply a wrapper around the Linux drivers/support.

On this board uGFX currently supports:
	- GDISP via the SDL driver
	- GINPUT-touch via the SDL driver
	- GINPUT-keyboard via the SDL driver


The folowing packages are required to run uGFX using this driver on
a 64-bit FreeBSD system using SDL:
	+ devel/gcc10
	+ devel/sdl2


The following should be added to the CFLAGS of the target makefile:
	CFLAGS = `sdl2-config --libs --cflags`


There is an example Makefile and project in the examples directory.

