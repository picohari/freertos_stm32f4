#ifndef _DATE_AND_TIME_UTIL_
#define _DATE_AND_TIME_UTIL_

#include "gfx.h"
#include "gui.h"

void update_time(void);

void update_date(void);

void update_date_and_time_label(void);

void compute_days_in_month(void);

/* GETTERS AND SETTERS */
uint8_t get_seconds(void);

void set_seconds(uint8_t new_seconds);

uint8_t get_minutes(void);

void set_minutes(uint8_t new_minutes);

uint8_t get_hours(void);

void set_hours(uint8_t new_hours);

uint8_t get_date(void);

void set_date(uint8_t new_date);

uint8_t get_month(void);

void set_month(uint8_t new_month);

uint16_t get_year(void);

void set_year(uint16_t new_year);

uint8_t get_days_in_current_month(void);

#endif /* _DATE_AND_TIME_UTIL_ */