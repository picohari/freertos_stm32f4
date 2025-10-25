/* ============================================================================
 * File: gui_menu.h
 * Description: Menu handling for µGFX
 *              Core page manager concept
 * ============================================================================ */

#ifndef _GUI_MENU_H_
#define _GUI_MENU_H_

#include <stdbool.h>

/* Pages and Menus - keep overview with intended structure :) */
typedef enum {
    PAGE_MAIN = 0,      /* Start counting with 0 */
    MENU_SETUP,
    PAGE_COUNT          /* MUST (!) be last - automatic count of page numering */
} MenuPage_t;


typedef struct MenuPageDef_t MenuPageDef_t;   // forward declaration for struct

typedef bool (*MenuEventFn)(MenuPageDef_t *page, GEvent *pe);

struct MenuPageDef_t {
    MenuPage_t  id;             /* Counts the page ID - done in MenuPage_t */
    GHandle     container;      /* The container handle of the whole page */

    const char *title;          /* Human-readable title */
    void (*onShow)(void);       /* Function pointers to life-cycle of a page */
    void (*onClose)(void);

    MenuEventFn onEvent;        /* Shortcut for 'bool (*onEvent)(MenuPageDef_t *page, GEvent *pe)' */
};

extern MenuPageDef_t menuPages[PAGE_COUNT];
extern MenuPage_t activePage;
extern MenuPage_t lastActive;


void Menu_ShowPage(MenuPage_t newPage);
void Menu_Init(MenuPage_t startPage);
MenuPage_t Menu_GetActive(void);
MenuPage_t Menu_FindVisible(void);
const char *Menu_GetActiveTitle(void);


#endif /* _GUI_MENU_H */