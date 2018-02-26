#ifndef _MEMBRANE_UTIL_
#define _MEMBRANE_UTIL_

#include "gfx.h"
#include "gui.h"

/* GETTERS AND SETTERS */
uint8_t get_membrane_count(void);

void set_membrane_count(uint8_t new_membrane_count);

uint8_t get_suction_overflow(void);

void set_suction_overflow(uint8_t new_suction_overflow);

uint8_t get_sludge(void);

void set_sludge(uint8_t new_sludge);

uint8_t get_seconds_t1(void);

void set_seconds_t1(uint8_t new_seconds_t1);

uint8_t get_minutes_t1(void);

void set_minutes_t1(uint8_t new_minutes_t1);

uint8_t get_hours_t1(void);

void set_hours_t1(uint8_t new_hours_t1);

uint8_t get_seconds_t2(void);

void set_seconds_t2(uint8_t new_seconds_t2);

uint8_t get_minutes_t2(void);

void set_minutes_t2(uint8_t new_minutes_t2);

uint8_t get_hours_t2(void);

void set_hours_t2(uint8_t new_hours_t2);

uint8_t get_aeration_air_on(void);

void set_aeration_air_on(uint8_t new_aeration_air_on);

uint8_t get_aeration_air_off(void);

void set_aeration_air_off(uint8_t new_aeration_air_off);

uint8_t get_aeration_a1(void);

void set_aeration_a1(uint8_t new_aeration_a1);

uint8_t get_aeration_a2(void);

void set_aeration_a2(uint8_t new_aeration_a2);

#endif /* _ZEN_UTIL_ */