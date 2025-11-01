#include "gfx_led.h"
#include "src/gwin/gwin_class.h"

#define LED_THICKNESS		2
#define LED_SCALE_OFFSET	20

#define gh2obj ((LedObject*)gh)
#define gw2obj ((LedObject*)gw)
#define imgFaceWidth gw2obj->imgFace->width
#define imgFaceHeight gw2obj->imgFace->height

// The statusbar VMT
static const gwidgetVMT ledVMT = {
	{
		"Led",						// The classname
		sizeof(LedObject),			// The object size
		_gwidgetDestroy,			// The destroy routine
		_gwidgetRedraw,				// The redraw routine
		0,							// The after-clear routine
	},
	ledDraw_Default,				// The default drawing routine
	#if GINPUT_NEED_MOUSE
		{
			0,						// Process mouse down events
			0,						// Process mouse up events
			0,						// Process mouse move events
		},
	#endif
	#if GINPUT_NEED_KEYBOARD || GWIN_NEED_KEYBOARD
		{
			0						// Process keyboard events
		},
	#endif
	#if GINPUT_NEED_TOGGLE
		{
			0,						// Toggle role
			0,						// Assign Toggles
			0,						// Get Toggles
			0,						// Process toggle off events
			0,						// Process toggle on events
		},
	#endif
	#if GINPUT_NEED_DIAL
		{
			0,						// No dial roles
			0,						// Assign Dials
			0,						// Get Dials
			0,						// Process dial move events
		},
	#endif
};

GHandle ledGCreate(GDisplay* g, LedObject* so, GWidgetInit* pInit, gdispImage* imgFace, gColor color)
{
	// Create the base class (the actual widget)
	if (!(so = (LedObject*)_gwidgetCreate(g, &so->w, pInit, &ledVMT))) {
		return 0;
	}

	// Initialize the object struct
	so->imgFace = imgFace;
    so->color = color;

	// Set the initial visibility
	gwinSetVisible((GHandle)so, pInit->g.show);

	return (GHandle)so;
}

void ledSetState(GHandle gh, uint8_t state)
{
	// Make sure that this is a dial
	if (gh->vmt != (gwinVMT*)&ledVMT)
		return;

	gh2obj->state = state;

	_gwinUpdate(gh);
}

void ledSetColor(GHandle gh, gColor color)
{
	// Make sure that this is a dial
	if (gh->vmt != (gwinVMT*)&ledVMT)
		return;

	gh2obj->color = color;

	_gwinUpdate(gh);
}











void ledSetAngle(GHandle gh, uint16_t angle)
{
	// Make sure that this is a dial
	if (gh->vmt != (gwinVMT*)&ledVMT)
		return;

	gh2obj->angle = angle;

	_gwinUpdate(gh);
}

uint16_t ledGetAngle(GHandle gh)
{
	// Make sure that this is a dial
	if (gh->vmt != (gwinVMT*)&ledVMT)
		return ((uint16_t)-1);

	return gh2obj->angle;
}

void ledDraw_Default(GWidgetObject* gw, void* param)
{
	//MatrixFloat2D m1, m2;
	const GColorSet* colors;
	(void)param;

	// Make sure that this is a dial
	if (gw->g.vmt != (gwinVMT*)&ledVMT)
		return;

	// Get the current color set
	if (!(gw->g.flags & GWIN_FLG_SYSENABLED))
		colors = &gw->pstyle->disabled;
	else
		colors = &gw->pstyle->enabled;

	// We don't need to clear the widget area because the dial face image covers the entire widget area
	//gdispGFillArea(gw->g.display, gw->g.x, gw->g.y, gw->g.width, gw->g.height, colors->fill);
	(void)colors;

	// Draw the face
	//gdispGImageDraw(gw->g.display, gw2obj->imgFace, gw->g.x, gw->g.y, imgFaceWidth, imgFaceHeight, 0, 0);

    gdispGDrawCircle(gw->g.display, gw->g.x + (gw->g.width / 2) , gw->g.y + (gw->g.height / 2), (gw->g.width / 2) - 2, GFX_SILVER);

    if (gw2obj->state) {
        gdispGFillCircle(gw->g.display, gw->g.x + (gw->g.width / 2) , gw->g.y + (gw->g.height / 2), (gw->g.width / 2) - 3, gw2obj->color);
    } else {
        gdispGFillCircle(gw->g.display, gw->g.x + (gw->g.width / 2) , gw->g.y + (gw->g.height / 2), (gw->g.width / 2) - 3, GFX_BLACK);
    }

}

// Limit the scope of the macros. Required for single-file compilation.
#undef gh2obj
#undef gw2obj
#undef imgFaceWidth
#undef imgFaceHeight
