#include <stdio.h>
#include <string.h>

#include "date_and_time_util.h"
#include "gfx.h"
#include "gui.h"

static uint8_t days_in_month[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

static uint8_t hours = 12;
static uint8_t minutes = 0;
static uint8_t seconds = 0;
static uint16_t milliseconds = 0;

static uint8_t date = 31;
static uint8_t month = 1;
static uint16_t year = 2018;

void update_time(void) {

	// Updates the time every 100 milliseconds

	milliseconds += 100;

	uint8_t oldMinutes = oldMinutes;

	if(milliseconds == 1000) {
		milliseconds = 0;
		seconds++;
	}

	if(seconds == 60) {
		seconds = 0;
		minutes++;
	}

	if(minutes == 60) {
		minutes = 0;
		hours++;
	}

	if(hours == 24) {
		hours = 0;
		// update date
		update_date();
	}


	// update date and time label every minute
	if(oldMinutes != minutes) {
		update_date_and_time_label();
	}
	
}


void update_date(void) {

	// Handle different number of days for different months
	date++;

	if(date > days_in_month[month - 1]) {
		date = 1;
		month++;
	}

	if(month > 12) {
		month = 1;
		year++;
		compute_days_in_month();
	}
}

void compute_days_in_month(void) {

	// Only change the days in a month for February if the current year is a Leap year
	if((year % 400 == 0) || (year % 4 == 0 && year % 100 != 0)) {
		days_in_month[1] = 29;
	} else {
		days_in_month[1] = 28;
	}
}

void update_date_and_time_label(void) {

	// current date
	// 2 chars for date, 1 char for dot, 2 chars for month, 1 char for dot, 4 chars for year, 1 char for whitespace
	// 2 chars for hour, 1 char for colon, 2 chars for minutes, 1 char for colon, 2 chars for seconds
	// total 11 + 8 = 19
	//char new_date_and_time[20];
	char new_date_and_time[17];

	// Copy everything to a String/char array
	// snprintf(new_date_and_time, sizeof(new_date_and_time), "%02d.%02d.%d %02d:%02d:%02d", date, month, year, hours, minutes, seconds);
	snprintf(new_date_and_time, sizeof(new_date_and_time), "%02d.%02d.%d %02d:%02d", date, month, year, hours, minutes);

	gwinSetText(ghLabelClockTime, new_date_and_time, TRUE);
}


uint8_t get_seconds(void) {
	return seconds;
}

void set_seconds(uint8_t new_seconds) {
	seconds = new_seconds;
}

uint8_t get_minutes(void) {
	return minutes;
}

void set_minutes(uint8_t new_minutes) {
	minutes = new_minutes;
}

uint8_t get_hours(void) {
	return hours;
}

void set_hours(uint8_t new_hours) {
	hours = new_hours;
}

uint8_t get_date(void) {
	return date;
}

void set_date(uint8_t new_date) {
	date = new_date;
}

uint8_t get_month(void) {
	return month;
}

void set_month(uint8_t new_month) {
	month = new_month;
}

uint16_t get_year(void) {
	return year;
}

void set_year(uint16_t new_year) {
	year = new_year;
	compute_days_in_month();
}

uint8_t get_days_in_current_month(void) {
	return days_in_month[month - 1];
}