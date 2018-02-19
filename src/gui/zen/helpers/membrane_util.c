#include <stdio.h>
#include <string.h>

#include "date_and_time_util.h"
#include "gfx.h"
#include "gui.h"

static uint8_t membrane_count = 1;

uint8_t get_membrane_count(void) {
	return membrane_count;
}

void set_membrane_count(uint8_t new_membrane_count) {
	membrane_count = new_membrane_count;
}