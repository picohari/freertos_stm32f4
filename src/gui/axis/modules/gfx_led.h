#ifndef _GFX_LED_H
#define _GFX_LED_H

#include "gfx.h"

/**
 * The Event Type for a led event
 */
#define GEVENT_LED		  (GEVENT_USER_FIRST + 0)

/* A led widget object.
 * Treat it as a black box. Never access it directly.
 */
typedef struct LedObject_t {
	GWidgetObject w;  // Base Class

	gdispImage* imgFace;
	uint8_t angle; // degrees
    uint8_t state; // LED state (TRUE/FALSE = On/Off)
    gColor  color; // LED on color 
} LedObject;

#ifdef __cplusplus
extern "C" {
#endif

/**
    * Create a led widget.
    */
GHandle ledGCreate(GDisplay* g, LedObject* so, GWidgetInit* pInit, gdispImage* imgFace, gColor color);
#define ledCreate(so, pI, imgFace, color) ledGCreate(GDISP, so, pI, imgFace, color)


void ledSetState(GHandle gh, uint8_t state);
void ledSetColor(GHandle gh, gColor color);


/**
    * Set the angle of the led needle in degrees.
    */
void ledSetAngle(GHandle gh, uint16_t angle);

/**
    * Get the current angle of the led needle in degrees.
    */
uint16_t ledGetAngle(GHandle gh);

/**
    * The default rendering routine
    */
void ledDraw_Default(GWidgetObject* gw, void* param);

#ifdef __cplusplus
}
#endif

#endif /* _GFX_LED_H */
