#include <stdio.h>
#include <string.h>

#include "network_util.h"
#include "gfx.h"
#include "gui.h"

static char* ipv4_address;
static char* gateway;
static char* subnet_mask;

char *get_ipv4_address(void) {
	return ipv4_address;
}

void set_ipv4_address(char* new_ipv4_address) {
	ipv4_address = new_ipv4_address;
}

char *get_gateway(void) {
	return gateway;
}

void set_gateway(char* new_gateway) {
	gateway = new_gateway;
}

char *get_subnet_mask(void) {
	return subnet_mask;
}

void set_subnet_mask(char* new_subnet_mask) {
	subnet_mask = new_subnet_mask;
}