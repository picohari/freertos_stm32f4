#ifndef _NETWORK_UTIL_
#define _NETWORK_UTIL_

#include "gfx.h"
#include "gui.h"

#endif /* _NETWORK_UTIL_ */

/* GETTERS AND SETTERS */
uint32_t get_ipv4_address(void);

void set_ipv4_address(uint32_t new_ipv4_address);

uint32_t get_gateway(void);

void set_gateway(uint32_t new_gateway);

uint32_t get_subnet_mask(void);

void set_subnet_mask(uint32_t new_subnet_mask);