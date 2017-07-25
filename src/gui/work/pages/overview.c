//#define USE_PIXMAP

//boolean to determine if the clock should be drawn or not
int clockDrawable = 0;

//date values
int curYear = 2017;
char* dateMonths[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
int curMonth = 7;
int curDay = 24;

#ifdef USE_PIXMAP
	//pixmap for smooth refresh
	int pixmap_x;
	int pixmap_y;
	int pixmap_w;
	int pixmap_h;
	
	static GDisplay* pixmap;
	static const pixel_t* surface;
#endif

//position and radius of the clock
int cx;
int cy;
int cr;

//Show Overview page
void showOverview(void) {
	GWidgetInit wi;
	gwinWidgetClearInit(&wi);
	
	destroyPage();
	
	//draw INTEWA logo for 2 seconds
	gdispImageDraw(&intewa, 0, 0, GDISP_SCREEN_WIDTH, GDISP_SCREEN_HEIGHT, 0, 0);
	gfxSleepMilliseconds(2000);
	
	//empty the screen
	gdispClear(white_studio);
	
	// create container widget
	ghContainer = createContainer(wi);
	
	// Create label widget
	wi.g.show = TRUE;
	wi.g.x = buttonDimensions[0][0];
	wi.g.y = buttonDimensions[0][1];
	wi.g.width = buttonDimensions[0][2];
	wi.g.height = buttonDimensions[0][3];
	wi.g.parent = ghContainer;
	char str[80];
	sprintf(str, "%i %s %i", curDay, dateMonths[curMonth - 1], curYear);
	wi.text = str;
	wi.customDraw = gwinLabelDrawJustifiedCenter;
	gwinLabelCreate(0, &wi);
	
	// Create label widget
	wi.g.show = TRUE;
	wi.g.x = buttonDimensions[2][0];
	wi.g.y = buttonDimensions[2][1];
	wi.g.width = buttonDimensions[2][2];
	wi.g.height = buttonDimensions[2][3];
	wi.g.parent = ghContainer;
	wi.text = "Rainwater stuff here";
	wi.customDraw = gwinLabelDrawJustifiedCenter;
	gwinLabelCreate(0, &wi);
	
	// Create label widget
	wi.g.show = TRUE;
	wi.g.x = buttonDimensions[3][0];
	wi.g.y = buttonDimensions[3][1];
	wi.g.width = buttonDimensions[3][2];
	wi.g.height = buttonDimensions[3][3];
	wi.g.parent = ghContainer;
	wi.text = "Greywater  stuff here";
	wi.customDraw = gwinLabelDrawJustifiedCenter;
	gwinLabelCreate(0, &wi);
	
	// Create label widget
	wi.g.show = TRUE;
	wi.g.x = buttonDimensions[4][0];
	wi.g.y = buttonDimensions[4][1];
	wi.g.width = buttonDimensions[4][2];
	wi.g.height = buttonDimensions[4][3];
	wi.g.parent = ghContainer;
	wi.text = "Settings  stuff pt.1 here";
	wi.customDraw = gwinLabelDrawJustifiedCenter;
	gwinLabelCreate(0, &wi);
	
	// Create label widget
	wi.g.show = TRUE;
	wi.g.x = buttonDimensions[5][0];
	wi.g.y = buttonDimensions[5][1];
	wi.g.width = buttonDimensions[5][2];
	wi.g.height = buttonDimensions[5][3];
	wi.g.parent = ghContainer;
	wi.text = "Settings  stuff pt.2 here";
	wi.customDraw = gwinLabelDrawJustifiedCenter;
	gwinLabelCreate(0, &wi);
	
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
	
	#ifdef USE_PIXMAP
		//init pixmap
		pixmap_x = ceil(buttonDimensions[1][0]);
		pixmap_y = ceil(buttonDimensions[1][1]);
		pixmap_w = ceil(buttonDimensions[1][2]);
		pixmap_h = ceil(buttonDimensions[1][3]);
		
		pixmap = gdispPixmapCreate(pixmap_w, pixmap_h);
		surface = gdispPixmapGetBits(pixmap);
		gdispGSetOrientation(pixmap, GDISP_ROTATE_LANDSCAPE);
	
		cx = pixmap_w / 2;
		cy = pixmap_h / 2;
		cr = cx * 0.8;
	#else
		cx = GDISP_SCREEN_WIDTH/2;
		cy = GDISP_SCREEN_HEIGHT/2;
		cr = cx / 2;
	#endif
	
	//clock may be drawn
	gdispFillCircle(cx, cy, cr * 1.25, black_studio);
	clockDrawable = 1;
	drawClock();
}

//time difference indicators for the clock
double curTime = 61200;
int oldTime = 0;

//draw a clock hand with custom color and thickness
void drawClockHand(int cx, int cy, double angle, int length, color_t color, int thickness) {
    int x = (int) (cos(angle) * length + cx);
    int y = (int) (sin(angle) * length + cy);

	#ifdef USE_PIXMAP
		gdispGDrawThickLine(pixmap, cx, cy, x, y, color, thickness, 1);
	#else
		gdispDrawThickLine(cx, cy, x, y, color, thickness, 1);
	#endif
}

//draw the clock
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
	
	#ifdef USE_PIXMAP
		//draw clock face and outer frame
		gdispGFillCircle(pixmap, cx, cy, cr * 1.25, black_studio);
		gdispGFillCircle(pixmap, cx, cy, cr, white_studio);
	#else
		//draw clock face and outer frame
		gdispFillCircle(cx, cy, cr, white_studio);
	#endif
	
	//draw design on clock face
	int i;
	for(i = 0; i < 12; i++) {
		double a = M_PI * 2 * i / 12;
		int x1 = (int) (cos(a) * cr * 0.75 + cx);
		int y1 = (int) (sin(a) * cr * 0.75 + cy);
		int x2 = (int) (cos(a) * cr * 0.85 + cx);
		int y2 = (int) (sin(a) * cr * 0.85 + cy);
		
		#ifdef USE_PIXMAP
			gdispGDrawLine(pixmap, x1, y1, x2, y2, black_studio);
		#else
			gdispDrawLine(x1, y1, x2, y2, black_studio);
		#endif
	}
	
	//draw hour hand
	drawClockHand(cx, cy, M_PI * 2 * curTime / 60 / 60 / 12 - M_PI / 2, cr * 0.7, yellow_studio, 3);
	
	//draw minute hand
	drawClockHand(cx, cy, M_PI * 2 * curTime / 60 / 60 - M_PI / 2, cr * 0.85, black_studio, 2);
	
	//draw second hand
	drawClockHand(cx, cy, M_PI * 2 * curTime / 60 - M_PI / 2, cr * 0.95, red_studio, 1);
	
	#ifdef USE_PIXMAP
		//mirror pixmap to screen
		gdispBlitArea(pixmap_x, pixmap_y, pixmap_w, pixmap_h, surface);
	#endif
}
