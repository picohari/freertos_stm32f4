#include "colors.h"
#include "math.h"
#include "time.h"

//boolean to determine if the clock should be drawn or not
int clockDrawable = 0;

//Show Overview page
void showOverview(void) {
	//draw INTEWA logo for 2 seconds
	gdispImageDraw(&intewa, 0, 0, GDISP_SCREEN_WIDTH, GDISP_SCREEN_HEIGHT, 0, 0);
	gfxSleepMilliseconds(2000);
	
	//empty the screen
	gdispClear(white_studio);
	
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	// create container widget
	ghContainer = createContainer(wi);
	
	//functions (must be declared above)
	buttonFunctions[0] = showMainMenu;
	
	//create the buttons using the information above
	wi.g.show = TRUE;
	wi.g.x = 0;
	wi.g.y = 0;
	wi.g.width = GDISP_SCREEN_WIDTH;
	wi.g.height = GDISP_SCREEN_HEIGHT;
	wi.g.parent = ghContainer;
	wi.text = "";
	wi.customDraw = gwinButtonDraw_Image;
	wi.customParam = &_leer;
	wi.customStyle = 0;
	wi.tag = 0;
	gwinButtonCreate(0, &wi);
	
	//show the page
	gwinShow(ghContainer);
	
	//clock may be drawn
	clockDrawable = 1;
	drawClock();
}

//draw a clock hand with custom color and thickness
void drawClockHand(int cx, int cy, double angle, int length, color_t color, int thickness) {
    int x = (int) (cos(angle) * length + cx);
    int y = (int) (sin(angle) * length + cy);
    gdispDrawThickLine(cx, cy, x, y, color, thickness, 1);
}

//position and radius of the clock
int cx = GDISP_SCREEN_WIDTH / 2;
int cy = GDISP_SCREEN_HEIGHT / 2;
int cr = GDISP_SCREEN_WIDTH / 4;

//time difference indicators for the clock
double curTime = 60030;
int oldTime = 0;

//draw a clock	
void drawClock(void) {
	if(clockDrawable != 1) {
		return;
	}
	
	//minimal delay to prevent flickering
	gfxSleepMilliseconds(100);
	curTime += 0.1;
	
	//do not redraw if time has not changed
	if((int) curTime == oldTime) {
		return;
	}
	oldTime = curTime;
	
	//draw clock face and outer frame
	gdispFillCircle(cx, cy, cr * 1.25, black_studio);
	gdispFillCircle(cx, cy, cr, white_studio);
	
	//draw design on clock face
	int i;
	for(i = 0; i < 12; i++) {
		double a = M_PI * 2 * i / 12;
		int x1 = (int) (cos(a) * cr * 0.75 + cx);
		int y1 = (int) (sin(a) * cr * 0.75 + cy);
		int x2 = (int) (cos(a) * cr * 0.85 + cx);
		int y2 = (int) (sin(a) * cr * 0.85 + cy);
		gdispDrawLine(x1, y1, x2, y2, black_studio);
	}
	
	//draw hour hand
	drawClockHand(cx, cy, M_PI * 2 * curTime / 60 / 60 / 12 - M_PI / 2, cr * 0.7, yellow_studio, 3);
	
	//draw minute hand
	drawClockHand(cx, cy, M_PI * 2 * curTime / 60 / 60 - M_PI / 2, cr * 0.85, black_studio, 2);
	
	//draw second hand
	drawClockHand(cx, cy, M_PI * 2 * curTime / 60 - M_PI / 2, cr * 0.95, red_studio, 1);
}
