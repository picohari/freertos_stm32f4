#ifndef _GUI_LOGGER_H
#define _GUI_LOGGER_H

#include "gfxconf.h"

#ifdef UGFXSIMULATOR

/* Logging on SIM */
#if LOG_GUI_MENU
#include <stdio.h>  /* Logging to console (linux) */
#define LOG_MENU(fmt, ...) \
    fprintf(stderr, "[MENU] " fmt "\n", ##__VA_ARGS__)
#else
#define LOG_MENU(fmt, ...)  ((void)0)
#endif /* LOG_GUI_MENU */

#else

/* Logging on MCU*/
#if LOG_GUI_MENU
#include "log.h"    /* Logging through uart */
#define LOG_MENU(fmt, ...) \
    LOG_DEBUG("[MENU] " fmt, ##__VA_ARGS__)
#else
#define LOG_MENU(fmt, ...)  ((void)0)
#endif /* LOG_GUI_MENU */

#endif /* UGFXSIMULATOR */

#endif /* _GUI_LOGGER_H */