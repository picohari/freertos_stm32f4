/**
 * This file has a different license to the rest of the uGFX system.
 * You can copy, modify and distribute this file as you see fit.
 * You do not need to publish your source modifications to this file.
 * The only thing you are not permitted to do is to relicense it
 * under a different license.
 */

/**
 * Copy this file into your project directory and rename it as gfxconf.h
 * Edit your copy to turn on the uGFX features you want to use.
 * The values below are the defaults. You should delete anything
 * you are leaving as default.
 *
 * Please use spaces instead of tabs in this file.
 */

#ifndef _GFXCONF_H
#define _GFXCONF_H

/* The operating system to use. One of these must be defined - preferably in your Makefile */
#ifndef __arm__
#define UGFXSIMULATOR                       /* TRUE on LINUX, used for simulation (we want it on our ARM CPU to be UNDEFINED/FALSE) */     
#endif

#define GFX_COMPILER                        GFX_COMPILER_GCC

#ifdef UGFXSIMULATOR
/* We can use the µGFX API on our development machine in a simulator for X Window */
    #ifdef __linux__
    #define GFX_USE_OS_LINUX                TRUE
    #else
    #define GFX_USE_OS_WIN32                TRUE
    #endif
    #define GFX_CPU                         GFX_CPU_X64

#else
/* We the real thing ... */

    #define GFX_USE_OS_FREERTOS             TRUE
    #define GFX_OS_NO_INIT                  TRUE
    #define GFX_CPU                         GFX_CPU_CORTEX_M4_FP
    #define GFX_OS_INIT_NO_WARNING          TRUE

#endif

#define GDISP_SCREEN_WIDTH                  320
#define GDISP_SCREEN_HEIGHT                 240

///////////////////////////////////////////////////////////////////////////
// GDISP                                                                 //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GDISP                              TRUE

#define GDISP_NEED_VALIDATION                      TRUE
#define GDISP_NEED_CLIP                            TRUE
#define GDISP_NEED_CIRCLE                          TRUE
#define GDISP_NEED_CONVEX_POLYGON                  TRUE
//#define GDISP_NEED_SCROLL                            TRUE
#define GDISP_NEED_CONTROL                         TRUE
#define GDISP_NEED_TEXT                            TRUE
    #define GDISP_INCLUDE_FONT_UI2                 TRUE
    #define GDISP_INCLUDE_USER_FONTS               TRUE
#define GDISP_NEED_STREAMING	                   FALSE

#define GDISP_NEED_IMAGE                           TRUE
    #define GDISP_NEED_IMAGE_BMP                   TRUE
    #define GDISP_NEED_IMAGE_GIF                   TRUE
    #define GDISP_NEED_IMAGE_ACCOUNTING            TRUE

#define GDISP_NEED_PIXMAP                          TRUE
 
#ifdef UGFXSIMULATOR
#define GDISP_DEFAULT_ORIENTATION                  GDISP_ROTATE_LANDSCAPE
#else
#define GDISP_DEFAULT_ORIENTATION                  GDISP_ROTATE_270
#endif
 
#define GDISP_NEED_MULTITHREAD                     TRUE       /* needed by GWIN_NEED_WIDGET */
#define GDISP_NEED_STARTUP_LOGO                    FALSE

///////////////////////////////////////////////////////////////////////////
// GWIN                                                                  //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GWIN                               TRUE

#define GWIN_NEED_WINDOWMANAGER                    TRUE
	#define GWIN_NEED_FLASHING					   TRUE

#define GWIN_NEED_CONSOLE                          TRUE
    #define GWIN_CONSOLE_USE_HISTORY               TRUE
        #define GWIN_CONSOLE_HISTORY_AVERAGING     TRUE
        #define GWIN_CONSOLE_HISTORY_ATCREATE      TRUE
#define GWIN_NEED_GRAPH                            TRUE

#define GWIN_NEED_WIDGET                           TRUE
    #define GWIN_NEED_LABEL                        TRUE
        #define GWIN_LABEL_ATTRIBUTE               TRUE
    #define GWIN_NEED_BUTTON                       TRUE
//        #define GWIN_BUTTON_LAZY_RELEASE             TRUE
    #define GWIN_NEED_SLIDER                       TRUE
    #define GWIN_NEED_CHECKBOX                     TRUE
    #define GWIN_NEED_IMAGE                        TRUE
    #define GWIN_NEED_RADIO                        TRUE
    #define GWIN_NEED_LIST                         TRUE
        #define GWIN_NEED_LIST_IMAGES              TRUE
    #define GWIN_NEED_PROGRESSBAR                  TRUE
        #define GWIN_PROGRESSBAR_AUTO              TRUE

#define GWIN_NEED_CONTAINERS                       TRUE
    #define GWIN_NEED_CONTAINER                    TRUE

#define GWIN_NEED_TABSET                           TRUE

///////////////////////////////////////////////////////////////////////////
// GEVENT                                                                //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GEVENT                             TRUE

///////////////////////////////////////////////////////////////////////////
// GTIMER                                                                //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GTIMER                             TRUE

#define GTIMER_THREAD_PRIORITY                     NORMAL_PRIORITY
#define GTIMER_THREAD_WORKAREA_SIZE                2048

///////////////////////////////////////////////////////////////////////////
// GQUEUE                                                                //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GQUEUE                             TRUE

#define GQUEUE_NEED_ASYNC                          TRUE

///////////////////////////////////////////////////////////////////////////
// GINPUT                                                                //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GINPUT                             TRUE

#define GINPUT_NEED_MOUSE                          TRUE

#ifndef UGFXSIMULATOR
#define GINPUT_NEED_TOGGLE                         TRUE
//#define GINPUT_NEED_DIAL                           TRUE
#endif

#define GINPUT_TOUCH_USER_CALIBRATION_LOAD         TRUE

///////////////////////////////////////////////////////////////////////////
// GFILE                                                                 //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GFILE                              TRUE

#define GFILE_NEED_PRINTG                          TRUE
#define GFILE_NEED_STRINGS                         TRUE

#define GFILE_NEED_ROMFS                           TRUE
//#define GFILE_NEED_NATIVEFS                          TRUE

#define GFILE_MAX_GFILES                           5

///////////////////////////////////////////////////////////////////////////
// GMISC                                                                 //
///////////////////////////////////////////////////////////////////////////
#define GFX_USE_GMISC				               TRUE
#define GMISC_NEED_INVSQRT			               TRUE


#endif /* _GFXCONF_H */
