

/*

	sevensegment_update_coord(10, 80);
	sevensement_draw_0(  10, 80);

*/

#include <stdio.h>

#include "gfx.h"


#include "helpers/sevensegment.h"

	
//GDisplay* pixmap;
//pixel_t* surface;

#ifdef LCD_SMALLFONT

const point a[6] = { { 1,  1}, { 2,  0}, { 8,  0}, { 9,  1}, { 8,  2}, { 2,  2} };
const point b[6] = { { 1,  9}, { 0,  8}, { 0,  2}, { 1,  1}, { 2,  2}, { 2,  8} };
const point c[6] = { { 9,  1}, {10,  2}, {10,  8}, { 9,  9}, { 8,  8}, { 8,  2} };
const point d[6] = { { 1,  9}, { 2,  8}, { 8,  8}, { 9,  9}, { 8, 10}, { 2, 10} };
const point e[6] = { { 1, 17}, { 0, 16}, { 0, 10}, { 1,  9}, { 2, 10}, { 2, 16} };
const point f[6] = { { 9,  9}, {10, 10}, {10, 16}, { 9, 17}, { 8, 16}, { 8, 10} };
const point g[6] = { { 9, 17}, { 8, 18}, { 2, 18}, { 1, 17}, { 2, 16}, { 8, 16} };

#else

const point a[6] = { { 6,  5}, { 3,  2}, { 5,  0}, {17,  0}, {19,  2}, {16,  5} };
const point b[6] = { { 2, 20}, { 0, 18}, { 0,  5}, { 2,  3}, { 4,  5}, { 4, 18} };
const point c[6] = { {20, 20}, {18, 18}, {18,  5}, {20,  3}, {22,  5}, {22, 18} };
const point d[6] = { { 6, 23}, { 3, 20}, { 5, 18}, {17, 18}, {19, 20}, {16, 23} };
const point e[6] = { { 2, 38}, { 0, 36}, { 0, 23}, { 2, 21}, { 4, 23}, { 4, 36} };
const point f[6] = { {20, 38}, {18, 36}, {18, 23}, {20, 21}, {22, 23}, {22, 36} };
const point g[6] = { { 6, 41}, { 3, 38}, { 5, 36}, {17, 36}, {19, 38}, {16, 41} };

#endif

static point av[6];
static point bv[6];
static point cv[6];
static point dv[6];
static point ev[6];
static point fv[6];
static point gv[6];


void sevensegment_update_coord(uint16_t xpos, uint16_t ypos) {

	uint8_t i;

	for (i = 0; i < 6; i++) {
		av[i].x = a[i].x + xpos; av[i].y = a[i].y + ypos;
		bv[i].x = b[i].x + xpos; bv[i].y = b[i].y + ypos;
		cv[i].x = c[i].x + xpos; cv[i].y = c[i].y + ypos;
		dv[i].x = d[i].x + xpos; dv[i].y = d[i].y + ypos;
		ev[i].x = e[i].x + xpos; ev[i].y = e[i].y + ypos;
		fv[i].x = f[i].x + xpos; fv[i].y = f[i].y + ypos;
		gv[i].x = g[i].x + xpos; gv[i].y = g[i].y + ypos;
	}

}


void sevensegment_draw_0(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Inactive);
}

void sevensegment_draw_1(uint16_t xpos, uint16_t ypos) {
	
	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Inactive);

}

void sevensegment_draw_2(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Inactive);

}

void sevensegment_draw_3(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Inactive);

}

void sevensegment_draw_4(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Inactive);

}

void sevensegment_draw_5(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Inactive);

}

void sevensegment_draw_6(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Inactive);

}

void sevensegment_draw_7(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Inactive);

}

void sevensegment_draw_8(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Active);
}

void sevensegment_draw_9(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Active);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Active);

	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Inactive);

}

void sevensegment_draw_B(uint16_t xpos, uint16_t ypos) {

	sevensegment_update_coord(xpos, ypos);

	gdispGFillConvexPoly(GDISP, 2, 2, av, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, bv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, cv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, dv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, ev, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, fv, 6, Inactive);
	gdispGFillConvexPoly(GDISP, 2, 2, gv, 6, Inactive);

}


#ifdef LCD_SMALLFONT

#define LCD_CHAR_WIDTH		16
#define LCD_CHAR_HEIGHT		20
#define LCD_CHAR_SPACE		2

#else

#define LCD_CHAR_WIDTH		26
#define LCD_CHAR_HEIGHT		45
#define LCD_CHAR_SPACE		4

#endif

#define LCD_BUFFER			5

void gui_create_lcd(uint16_t value) {


	//uint16_t value = 4;
	char numbers[LCD_BUFFER] = "1234";

	sprintf(numbers, "%4d", value);

	uint16_t xpos, ypos;

	xpos = 160;
	ypos = 28;

	uint8_t i = 0;

	for (i = 0; i < LCD_BUFFER-1; i++) {

		switch(numbers[i]) {

			case 48: sevensegment_draw_0(xpos, ypos); break;
			case 49: sevensegment_draw_1(xpos, ypos); break;
			case 50: sevensegment_draw_2(xpos, ypos); break;
			case 51: sevensegment_draw_3(xpos, ypos); break;
			case 52: sevensegment_draw_4(xpos, ypos); break;
			case 53: sevensegment_draw_5(xpos, ypos); break;
			case 54: sevensegment_draw_6(xpos, ypos); break;
			case 55: sevensegment_draw_7(xpos, ypos); break;
			case 56: sevensegment_draw_8(xpos, ypos); break;
			case 57: sevensegment_draw_9(xpos, ypos); break;
			
			default: sevensegment_draw_B(xpos, ypos); break;
		}

		xpos = xpos + LCD_CHAR_WIDTH + LCD_CHAR_SPACE;

	}


}

