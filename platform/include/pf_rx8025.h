
#ifndef  _PF_RX8025_H_ 
#define _PF_RX8025_H_

#include "type.h"

extern BOOL Rx8025Init();
extern BOOL Rx8025Set24Hour(BOOL hour24);
extern BOOL Rx8025SetTime(unsigned char hour,
                          unsigned char minute,unsigned char second);
extern BOOL Rx8025GetTime(unsigned char *hour,
                          unsigned char *minute, unsigned char *second);

extern BOOL Rx8025SetCalendar(unsigned char year,
                          unsigned char month,unsigned char day);
extern BOOL Rx8025GetCalendar(unsigned char *year,
                          unsigned char *month,unsigned char *day);


#endif /*_PF_RX8025_H_  */ 


