# FreeRTOS + lwIP + µGFX for STM32F407-XCore

Currently a work-in-progress as a remote HandWheel controller for my LinuxCNC machine.

* Likely older HAL libraries from ST (will be upgraded soon) but they work.
* FreeRTOS 9.0 as from CubeMX Repository
* Current version of lwIP 2.2.2 with websocket support
* Current version of µGFX graphics library with driver for XCore board
* BSP, MQTT and USB-FAT has been played around, as said, wip!
* makefile based project
* Additional tools for easy debugging

Learned a lot by working on this project. Tested a lot of things, thus many left comments and
unused code - ways not ready for production, but a good starting point for many porjects
requiring a GUI, Network and a Taskmanager.

With this package (wish I had it before), all hardware for the XCore407i development board can
be accessed and testet. Not all hardware drivers are written out, as you can easily create them
using the CubeMX application. So I just included, the ones I'm currently interested in.

Give it a look!

## What we have

1. Low level drivers sourced from ST, Firmware Package 1.7.1 - I know, this is old, but in latest
   releases the rewrite of some hardware drivers messed all up - specially the ETHERNET drivers.
   Apparently latest known version should be 1.24 or 1.25, will test them and provide feedback.

2. FreeRTOS is time- and houskeeper: Fast and efficient task management. Go to config/FreeRTOSConfig.h
   to study the settings.

3. lwIP provides the TCP/IP stack and handles TCP and UDP requests. As an example of
   interactive web application, the websocket functionality has been added (from patch ...)

4. µGFX gives full power of the graphics LCD and touchscreen. Easy to use, easy to adapt.

5. UART debug to console. Interactive shell possible, tried, maybe later.

6. USB drivers and FatFS support, mount your own USB stick or make a cool USB gadget.

7. MQTT needs a rebase, experimental, compiler throws few warnings...

8. clean and easy build with makefile. Memory usage calculation.


## How to use

Clone repository with submodules

make

Program ST with openocd and gdb

Run UART debug terminal as console output

Tata! Go your steps now.


## External libraries

Uses a few external libraries with own licences, read them carefully before deploying project.
As long as they are submodules, own forks here on github.com are provided - as sometimes code needs
to be adapted. You still get the latest changes using git.


## Debugging

Most important lession: Try and error. Sorry, code is 'as-it' unfinished, sometimes not elegant at all
and maybe a couple of bugs too, but it gives the chance to start quickly with the XCore board.

As I do not like complex IDEs and overloaded build settings, a simple makefile does all the work.

Feel free to participate!


## Config

Not much - wait! Okay yes, very much indeed! Most is in config folder:

* FreeRTOS
* lwIP
* HAL
* Project config

Adapt it to your needs..


## Tools

For easier testing, a couple of python tools were used, eg. UDP telegram sender, receiver, Menu builder..
There are also tools for creating own websites in http-fs or font creation for LCD lib.

