/*
 * This file is subject to the terms of the GFX License. If a copy of
 * the license was not distributed with this file, you can obtain one at:
 *
 *              http://ugfx.io/license.html
 */

#ifndef _GDISP_LLD_CONFIG_H
#define _GDISP_LLD_CONFIG_H

#if GFX_USE_GDISP

/*===========================================================================*/
/* Driver hardware support.                                                  */
/*===========================================================================*/

#define GDISP_HARDWARE_DRAWPIXEL			GFXON
#define GDISP_HARDWARE_PIXELREAD			GFXON
#define GDISP_HARDWARE_CONTROL				GFXON

// Both these pixel formats are supported - pick one.
// RGB565 obviously is faster and uses less RAM but with lower color resolution than RGB888
#if defined(STM32LTDC_USE_RGB565) && STM32LTDC_USE_RGB565
	#define GDISP_LLD_PIXELFORMAT			GDISP_PIXELFORMAT_RGB565
	#if defined(STM32LTDC_USE_LAYER2) && STM32LTDC_USE_LAYER2
		#error "GDISP - STM32LTDC: You must use RGB888 pixel format with LTDC when using both layers as only RGB888 currently supports using alpha."
	#endif
#else
	#define GDISP_LLD_PIXELFORMAT			GDISP_PIXELFORMAT_RGB888
#endif

/*===========================================================================*/
/* Don't change stuff below this line. Please.                               */
/*===========================================================================*/

// LLD command to swap buffers if double buffering is enabled.
#if STM32LTDC_USE_DOUBLEBUFFERING
	#define STM32LTDC_CONTROL_SHOW_BUFFER	(GDISP_CONTROL_LLD+0)
#endif

// Adjust driver config if DMA2D is enabled.
#if STM32LTDC_USE_DMA2D
	// DMA2D supports accelerated fills
 	#define GDISP_HARDWARE_FILLS		GFXON

	// Accelerated bitfills are also possible but only for gOrientation0
	//	and if no color translation is required (for now)
	#if !GDISP_NEED_CONTROL && !defined(GDISP_PIXELFORMAT)
 		#define GDISP_HARDWARE_BITFILLS	GFXON
	#endif
#endif /* GDISP_USE_DMA2D */

#endif	/* GFX_USE_GDISP */

#endif	/* _GDISP_LLD_CONFIG_H */
