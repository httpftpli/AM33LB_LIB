#ifndef __PF__RTC__H__
#define __PF__RTC__H__

#include "pf_rx8025.h"

extern BOOL RTCInit(void);
extern void RTCReadBcd(unsigned short *year, unsigned char *month,
             unsigned char  *day, unsigned char *hour,
             unsigned char *minute, unsigned char *second);
extern void RTCReadHex(unsigned short *year, unsigned char *month,
             unsigned char  *day, unsigned char *hour,
             unsigned char *minute, unsigned char *second);

extern BOOL RTCSetBcd(unsigned short year, unsigned char month,
             unsigned char  day, unsigned char hour,
             unsigned char minute, unsigned char second);
extern BOOL RTCSetHex(unsigned short year, unsigned char month,
             unsigned char  day, unsigned char hour,
             unsigned char minute, unsigned char second);

#endif
