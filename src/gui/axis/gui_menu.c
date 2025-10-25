/* ============================================================================
 * File: gui_menu.c
 * Description: Menu handling for µGFX
 *              Core page manager concept
 * ============================================================================ */

#include <stdio.h>
#include "gfx.h"
#include "gui.h"

#include "gui_menu.h"
#include "gui_logger.h"


/* Holds main menu structure of all pages */
MenuPageDef_t menuPages[PAGE_COUNT];

/* Enum value of current page */
MenuPage_t activePage = PAGE_MAIN;
MenuPage_t lastActive = PAGE_MAIN;  /* Keep track of page changes */


void Menu_ShowPage(MenuPage_t newPage) {
    /* Skip conditions */
    if (newPage >= PAGE_COUNT) return;
    if (newPage == activePage) return;

    LOG_MENU("Switch: %d -> %d", lastActive, newPage);

    /* onClose for the current page */
    if (menuPages[activePage].onClose) {
        menuPages[activePage].onClose();
    }

    /* Hide all pages first and .. */
    for (int i = 0; i < PAGE_COUNT; i++) {
        if (menuPages[i].container) {
            gwinHide(menuPages[i].container);
        }
    }

    /* Show only the requested one - if valid */
    if (menuPages[newPage].container) {
        gwinShow(menuPages[newPage].container);
        if (menuPages[newPage].onShow) {
            menuPages[newPage].onShow();
        }
        activePage = newPage;
    } else {
        LOG_MENU("Menu_ShowPage: page %d has no container", newPage);
    }

    /* Keep track of page change */
    lastActive = activePage;
}


void Menu_Init(MenuPage_t startPage) {

    /* First page to show - if valid */
    if (menuPages[startPage].container) {
        gwinShow(menuPages[startPage].container);
        if (menuPages[startPage].onShow) {
            menuPages[startPage].onShow();
        }
        activePage = startPage;
    } else {
        LOG_MENU("Menu_Init: page %d has no container", startPage);
    }
}

MenuPage_t Menu_GetActive(void) {
    return activePage;
}

MenuPage_t Menu_FindVisible(void) {
    for (int i = 0; i < PAGE_COUNT; i++) {
        if (gwinGetVisible(menuPages[i].container)) {
            return menuPages[i].id;
        }
    }
    return PAGE_COUNT;
}

const char *Menu_GetActiveTitle(void) {
    return menuPages[activePage].title;
}