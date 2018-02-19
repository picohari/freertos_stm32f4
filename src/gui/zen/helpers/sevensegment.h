/******************************************************************************/
/* ZEN MENU for µGFX                                                          */
/*                                                                            */
/******************************************************************************/

#ifndef _SEVENSEGMENT_H_
#define _SEVENSEGMENT_H_


#include "gfx.h"

//extern GDisplay* pixmap;
//extern pixel_t* surface;

#define LCD_SMALLFONT

/* Bright Orange */
// #define Active			HTML2COLOR(0xFFB300)
// #define Inactive			HTML2COLOR(0x0D0900)

/* Bright Blue */
#define Active			HTML2COLOR(0x0055FF)
#define Inactive		HTML2COLOR(0x000714)


void sevensement_draw_0(uint16_t xpos, uint16_t ypos);
void sevensement_draw_1(uint16_t xpos, uint16_t ypos);
void sevensement_draw_2(uint16_t xpos, uint16_t ypos);
void sevensement_draw_3(uint16_t xpos, uint16_t ypos);
void sevensement_draw_4(uint16_t xpos, uint16_t ypos);
void sevensement_draw_5(uint16_t xpos, uint16_t ypos);
void sevensement_draw_6(uint16_t xpos, uint16_t ypos);
void sevensement_draw_7(uint16_t xpos, uint16_t ypos);
void sevensement_draw_8(uint16_t xpos, uint16_t ypos);
void sevensement_draw_9(uint16_t xpos, uint16_t ypos);


void gui_create_lcd(uint16_t value);



#endif /* _SEVENSEGMENT_H_ */
