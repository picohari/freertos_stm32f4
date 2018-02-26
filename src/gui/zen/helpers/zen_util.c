#include <stdio.h>
#include <string.h>

#include "date_and_time_util.h"
#include "gfx.h"
#include "gui.h"

static uint8_t hours_t1 = 12;
static uint8_t minutes_t1 = 0;
static uint8_t seconds_t1 = 0;

static uint8_t hours_t2 = 12;
static uint8_t minutes_t2 = 0;
static uint8_t seconds_t2 = 0;

static uint8_t membrane_count = 1;

static uint8_t suction_overflow = 0;

static uint8_t sludge = 0;

static uint8_t aeration_air_on;
static uint8_t aeration_air_off;
static uint8_t aeration_a1;
static uint8_t aeration_a2;

uint8_t get_membrane_count(void) {
	return membrane_count;
}

void set_membrane_count(uint8_t new_membrane_count) {
	membrane_count = new_membrane_count;
}

uint8_t get_suction_overflow(void) {
	return suction_overflow;
}

void set_suction_overflow(uint8_t new_suction_overflow) {
	suction_overflow = new_suction_overflow;
}

uint8_t get_sludge(void) {
	return sludge;
}

void set_sludge(uint8_t new_sludge) {
	sludge = new_sludge;
}

uint8_t get_seconds_t1(void) {
	return seconds_t1;
}

void set_seconds_t1(uint8_t new_seconds_t1) {
	seconds_t1 = new_seconds_t1;
}

uint8_t get_minutes_t1(void) {
	return minutes_t1;
}

void set_minutes_t1(uint8_t new_minutes_t1) {
	minutes_t1 = new_minutes_t1;
}

uint8_t get_hours_t1(void) {
	return hours_t1;
}

void set_hours_t1(uint8_t new_hours_t1) {
	hours_t1 = new_hours_t1;
}

uint8_t get_seconds_t2(void) {
	return seconds_t2;
}

void set_seconds_t2(uint8_t new_seconds_t2) {
	seconds_t2 = new_seconds_t2;
}

uint8_t get_minutes_t2(void) {
	return minutes_t2;
}

void set_minutes_t2(uint8_t new_minutes_t2) {
	minutes_t2 = new_minutes_t2;
}

uint8_t get_hours_t2(void) {
	return hours_t2;
}

void set_hours_t2(uint8_t new_hours_t2) {
	hours_t2 = new_hours_t2;
}

uint8_t get_aeration_air_on(void) {
	return aeration_air_on;
}

void set_aeration_air_on(uint8_t new_aeration_air_on) {
	aeration_air_on = new_aeration_air_on;
}

uint8_t get_aeration_air_off(void) {
	return aeration_air_off;
}

void set_aeration_air_off(uint8_t new_aeration_air_off) {
	aeration_air_off = new_aeration_air_off;
}

uint8_t get_aeration_a1(void) {
	return aeration_a1;
}

void set_aeration_a1(uint8_t new_aeration_a1) {
	aeration_a1 = new_aeration_a1;
}

uint8_t get_aeration_a2(void) {
	return aeration_a2;
}

void set_aeration_a2(uint8_t new_aeration_a2) {
	aeration_a2 = new_aeration_a2;
}