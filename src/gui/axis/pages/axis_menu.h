/* ============================================================================
 * File: axis_menu.h
 * Description: Global declarations for all page creation functions of menu
 *              Also include global shared resources, like icons of images
 * ============================================================================ */

#ifndef _AXIS_MENU_H_
#define _AXIS_MENU_H_

#include "gfx.h"


/* SHARED SETTINGS */
extern uint32_t remote_ipv4_addr;
extern uint32_t local_ipv4_addr;


/* SHARED HANDLES */
extern GHandle ghLabelRotary;


/* SHARED IMAGES */



/* Some helper functions used in menu to access data across
 * diffenent locations in programm. Concentrate all includes
 * into one file.
 */
void axis_helper_setRemoteIP(uint32_t *new_address);
void axis_helper_getRemoteIP(uint32_t *out_addr);
void axis_helper_setLocalIP(uint32_t *new_address);
void axis_helper_getLocalIP(uint32_t *out_addr);

void axis_helper_setNetmask(uint32_t *new_address);
void axis_helper_getNetmask(uint32_t *out_addr);

void axis_helper_setGateway(uint32_t *new_address);
void axis_helper_getGateway(uint32_t *out_addr);

//uint32_t get_ipv4_address(void);

//void set_ipv4_address(uint32_t new_ipv4_address);



/* Here we declare all creation functions for the pages.
 * So we must include only this file for all menu pages.
 */
void create_PageHome(void);

void create_MenuSetup(void);
void create_SetupRemoteIP(void);
void create_SetupLocalIP(void);
void create_SetupNetmask(void);
void create_SetupGateway(void);



#endif /* _AXIS_MENU_H_ */