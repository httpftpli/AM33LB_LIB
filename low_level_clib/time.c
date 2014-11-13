/*******************
 *
 * Copyright 1998-2010 IAR Systems AB.
 *
 * This is the default implementation of the "time" function of the
 * standard library.  It can be replaced with a system-specific
 * implementation.
 *
 * The "time" function returns the current calendar time.  (time_t)-1
 * should be returned if the calendar time is not available.  The time
 * is measured in seconds since the first of January 1970.
 *
 ********************/

#include <time.h>
#include "pf_rtc.h"

#pragma module_name = "?time"

__time32_t (__time32)(__time32_t *t)
{
  struct tm tm_time;
  unsigned char month,day,hour,minute,second;
  unsigned short year;
  RTCReadHex(&year, &month,  &day, &hour, &minute,&second);
  tm_time.tm_sec = second;
  tm_time.tm_min = minute;
  tm_time.tm_hour = hour;
  tm_time.tm_mday = day;
  tm_time.tm_mon = month-1;
  tm_time.tm_year = year-1900;
  tm_time.tm_isdst = 0;
  __time32_t time = mktime(&tm_time);
  if (t)
  {
    *t =  time;
  }
  return  time;
}
