#ifndef _GFXCONF_H
#define _GFXCONF_H

#ifndef __arm__
	#define UGFXSIMULATOR
#endif

#define GFX_COMPILER GFX_COMPILER_GCC

#ifdef UGFXSIMULATOR
	#define GFX_USE_OS_WIN32 TRUE
	#define GDISP_SCREEN_WIDTH 320
	#define GDISP_SCREEN_HEIGHT 240
	#define GFX_CPU GFX_CPU_X64
#else
	#define GFX_USE_OS_FREERTOS TRUE
	#define GFX_OS_NO_INIT TRUE
	#define GFX_CPU GFX_CPU_CORTEX_M4_FP
	#define GFX_OS_INIT_NO_WARNING TRUE
#endif


/********************************************************/
/* GDISP stuff                                          */
/********************************************************/
#define GFX_USE_GDISP TRUE

#define GDISP_NEED_CONTROL TRUE
#define GDISP_NEED_VALIDATION TRUE
#define GDISP_NEED_CLIP TRUE
#define GDISP_NEED_ARC FALSE
#define GDISP_NEED_CONVEX_POLYGON TRUE
#define GDISP_NEED_IMAGE TRUE
	#define GDISP_NEED_IMAGE_BMP FALSE
	#define GDISP_NEED_IMAGE_GIF TRUE
#define GDISP_NEED_STARTUP_LOGO FALSE
#define GDISP_NEED_CIRCLE TRUE
#define GDISP_NEED_MULTITHREAD TRUE
#define GDISP_NEED_TEXT_WORDWRAP TRUE
#ifdef UGFXSIMULATOR
	#define GDISP_DEFAULT_ORIENTATION GDISP_ROTATE_LANDSCAPE
#else
	#define GDISP_DEFAULT_ORIENTATION GDISP_ROTATE_270
#endif
#define GDISP_STARTUP_COLOR HTML2COLOR(0xFFFFFF)


/********************************************************/
/* Font stuff                                           */
/********************************************************/
#define GDISP_NEED_TEXT TRUE
#define GDISP_NEED_ANTIALIAS FALSE
#define GDISP_NEED_TEXT_KERNING FALSE
#define GDISP_NEED_UTF8 FALSE
#define GDISP_INCLUDE_USER_FONTS TRUE


/********************************************************/
/* GWIN stuff                                           */
/********************************************************/
#define GFX_USE_GWIN TRUE

#define GWIN_NEED_WINDOWMANAGER TRUE
    #define GWIN_REDRAW_IMMEDIATE TRUE
    #define GWIN_REDRAW_SINGLEOP TRUE

#define GWIN_NEED_WIDGET TRUE
    #define GWIN_NEED_LABEL TRUE
    #define GWIN_NEED_BUTTON TRUE
        #define GWIN_BUTTON_LAZY_RELEASE FALSE
    #define GWIN_FLAT_STYLING FALSE

    #define GWIN_NEED_KEYBOARD TRUE
    #define GWIN_NEED_TEXTEDIT TRUE
	#define GWIN_WIDGET_TAGS TRUE
#define GWIN_NEED_CONTAINERS TRUE
    #define GWIN_NEED_CONTAINER TRUE
    #define GWIN_NEED_FRAME FALSE
    #define GWIN_NEED_TABSET FALSE


/********************************************************/
/* GTIMER stuff                                         */
/********************************************************/
#define GFX_USE_GTIMER TRUE

#define GTIMER_THREAD_PRIORITY NORMAL_PRIORITY
#define GTIMER_THREAD_WORKAREA_SIZE 2048


/********************************************************/
/* GINPUT stuff                                         */
/********************************************************/
#define GFX_USE_GINPUT TRUE

#define GINPUT_NEED_MOUSE TRUE
#ifdef UGFXSIMULATOR
	#define GINPUT_TOUCH_USER_CALIBRATION_LOAD FALSE
#else
	#define GINPUT_TOUCH_USER_CALIBRATION_LOAD TRUE
#endif
#define GINPUT_NEED_KEYBOARD FALSE


/********************************************************/
/* GEVENT stuff                                         */
/********************************************************/
#define GFX_USE_GEVENT TRUE

#define GEVENT_ASSERT_NO_RESOURCE FALSE
#define GEVENT_MAXIMUM_SIZE 32
#define GEVENT_MAX_SOURCE_LISTENERS 32


/********************************************************/
/* GEVENT stuff                                         */
/********************************************************/
#define GFX_USE_GQUEUE TRUE

#define GQUEUE_NEED_ASYNC TRUE


/********************************************************/
/* GFILE stuff                                          */
/********************************************************/
#define GFX_USE_GFILE TRUE

#define GFILE_NEED_NATIVEFS FALSE
#define GFILE_NEED_ROMFS TRUE
#define GFILE_MAX_GFILES 12



#endif /* _GFXCONF_H */
