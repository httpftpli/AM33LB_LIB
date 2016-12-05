#ifndef __PF__TSC2007__H__
#define __PF__TSC2007__H__



#include <stdbool.h>

extern bool tsc2007Init(unsigned int i2c, unsigned char addrA0A1);
extern void tsc2007TouchProcess(void);


#endif
