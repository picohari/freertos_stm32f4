




#if 1


#include <time.h>
#include <sys/time.h>

#include <locale.h>

#define TIMEBUF   64

static void RTC_thread (void const * arg)
{

  (void) arg;

  struct tm tm_set;

  /* Set time to 1.1.2017 00:00 */

  tm_set.tm_sec  = 1;         /* seconds,  range 0 to 59          */
  tm_set.tm_min  = 0;         /* minutes, range 0 to 59           */
  tm_set.tm_hour = 0;         /* hours, range 0 to 23             */

  tm_set.tm_mon  = 0;             /* month, range 0 to 11             */
  tm_set.tm_year = 2017 - 1900;   /* The number of years since 1900   */
  tm_set.tm_mday = 1;             /* day of the month, range 1 to 31  */
  tm_set.tm_isdst = -1;           /* daylight saving time             */

#if 0
  /*
  The mktime function ignores the specified contents of the tm_wday and tm_yday members of the broken-down
  time structure. It uses the values of the other components to compute the calendar time; it's permissible
  for these components to have unnormalized values outside of their normal ranges. The last thing that mktime 
  does is adjust the components of the brokentime structure (including the tm_wday and tm_yday).
  */ 
  tm_set.tm_yday;        /* day in the year, range 0 to 365  */
  tm_set.tm_wday;        /* day of the week, range 0 to 6    */

  //debug("Size of time_t: %d\n", sizeof(time_t)); /* Returns: 4 = 32bit => we have a year 2038 Problem... */
#endif

  /* Create time from settings provided in tm_set */
  time_t set_timer = mktime(&tm_set) ;

  //debug("%lu \n", set_timer );  // Print timestamp

  /* Set system time to milliseconds, reduce by one second */
  clock_timestamp = (uint64_t) set_timer * 1000U;

  //setlocale(LC_TIME, "de_DE.UTF-8");

  while (1) {

    static char buffer[TIMEBUF];
    
    static time_t timer;
    static struct tm* tm_info;

    //static char buffer_del[TIMEBUF];
    //static struct tm tm_del;

/*
       1  2  3  4  5  6  7
rtc    mo di mi do fr sa so

    0  1  2  3  4  5  6
lib so mo di mi do fr sa
*/

    /* Read time info from clock (UTC) via _gettimeofday() */
    time(&timer);

    /* Break up time info as local time */
    tm_info = localtime(&timer);
    
    //strftime (buffer, TIMEBUF, "%a, %d. %B %Y %H:%M:%S", tm_info);
    strftime (buffer, TIMEBUF, "%d.%m.%Y %H:%M:%S", tm_info);

    //gwinSetText(ghLabelClockTime, buffer, 1);


#if 0
    //taskENTER_CRITICAL();

    strftime (buffer, TIMEBUF, "%a, %d. %B %Y\n", tm_info); /* So, 31. Januar 1970 */
    gdispFillStringBox( 5, 60, 200, 20, buffer, dejavu_sans_10, White, Black, justifyLeft);
    memset(buffer, 0, TIMEBUF);
    //gfxSleepMilliseconds(50);

    strftime (buffer, TIMEBUF, "%H:%M:%S", tm_info);
    gdispFillStringBox( 5, 80, 200, 20, buffer, dejavu_sans_10, White, Black, justifyLeft);
    memset(buffer, 0, TIMEBUF);
    //gfxSleepMilliseconds(50);
#endif


#if 0
    // compare/store old value
    if (tm_del.tm_sec != tm_info->tm_sec) {

      do {
        /* Delete old value from screen */
        sprintf(buffer_del, "%s\n", asctime(&tm_del));
        gdispDrawString( 5, 40, buffer_del, dejavu_sans_16, Black);
        
        /* Draw new value */
        sprintf(buffer, "%s\n", asctime(tm_info));
        gdispDrawString( 5, 40, buffer, dejavu_sans_16, White);
        
        /* store new value as old one.. */
        tm_del = *tm_info;

      } while (0);

    }

    sprintf(buffer, "DST: %d DOW: %d DOY: %d \n", tm_info->tm_isdst, tm_info->tm_wday, tm_info->tm_yday);
    gdispFillStringBox( 5, 100, 200, 20, buffer, dejavu_sans_10, White, Black, justifyLeft);
    memset(buffer, 0, TIMEBUF);
    //gfxSleepMilliseconds(50);

#endif
    //taskEXIT_CRITICAL();

    /*
    vt100_puts("\e[H");
    vt100_puts(aShowTime); vt100_putc('\r');
    vt100_puts(aShowDate); vt100_putc('\r');
    */

    osDelay(1000);
  }

}



#endif


