#ifndef _NETWORK_UTIL_
#define _NETWORK_UTIL_

#include "gfx.h"
#include "gui.h"

#endif /* _NETWORK_UTIL_ */

/* GETTERS AND SETTERS */
char *get_ipv4_address(void);

void set_ipv4_address(char* new_ipv4_address);

char *get_gateway(void);

void set_gateway(char* new_gateway);

char *get_subnet_mask(void);

void set_subnet_mask(char* new_subnet_mask);