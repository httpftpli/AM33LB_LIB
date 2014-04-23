#ifndef __PF__RTC__H__
#define __PF__RTC__H__

#include "pf_rx8025.h"

extern BOOL RTCInit(void);
extern void RTCReadBcd(unsigned char *year, unsigned char *month,
             unsigned char  *day, unsigned char *hour,
             unsigned char *minute, unsigned char *second);
extern void RTCReadHex(unsigned char *year, unsigned char *month,
             unsigned char  *day, unsigned char *hour,
             unsigned char *minute, unsigned char *second);

extern BOOL RTCSetBcd(unsigned char year, unsigned char month,
             unsigned char  day, unsigned char hour,
             unsigned char minute, unsigned char second);
extern BOOL RTCSetHex(unsigned char year, unsigned char month,
             unsigned char  day, unsigned char hour,
             unsigned char minute, unsigned char second);

#endif
