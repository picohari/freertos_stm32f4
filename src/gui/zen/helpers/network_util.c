#include <stdio.h>
#include <string.h>

#include "network_util.h"
#include "gfx.h"
#include "gui.h"

static uint32_t ipv4_address;
static uint32_t gateway;
static uint32_t subnet_mask;

uint32_t get_ipv4_address(void) {
	return ipv4_address;
}

void set_ipv4_address(uint32_t new_ipv4_address) {
	ipv4_address = new_ipv4_address;
}

uint32_t get_gateway(void) {
	return gateway;
}

void set_gateway(uint32_t new_gateway) {
	gateway = new_gateway;
}

uint32_t get_subnet_mask(void) {
	return subnet_mask;
}

void set_subnet_mask(uint32_t new_subnet_mask) {
	subnet_mask = new_subnet_mask;
}