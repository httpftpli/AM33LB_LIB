#ifndef __PF__GPIO__H__
#define __PF__GPIO__H__

#include "gpio_v2.h"

extern void GPIOInit(unsigned int moduleId, int debounceTimer,unsigned int debounceEnableBitmap) ;
extern void GPIOClearHandler(unsigned char modulOfGpio, unsigned bitOfGpio);
extern void GPIORegistHandler(unsigned char modulOfGpio,unsigned bitOfGpio,
                       void (*gpiohandler)());


#endif
