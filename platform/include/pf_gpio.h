#ifndef __PF__GPIO__H__
#define __PF__GPIO__H__

#include "gpio_v2.h"

extern void GPIOInit(unsigned int moduleId, int debounceTimer,unsigned int debounceEnableBitmap) ;
extern void GPIOClearHandler(unsigned char modulOfGpio, unsigned bitOfGpio);
extern void GPIORegistHandler(unsigned char modulOfGpio,unsigned bitOfGpio,
                       void (*gpiohandler)());
extern void GPIODirSet(unsigned int moduleId,unsigned int pinNumber,unsigned int pinDirection);
extern void GPIODirGet(unsigned  int moduleId,unsigned int pinNumber);
extern void GPIOPinWr(unsigned int moduleId, unsigned int pinNumber, unsigned int value);
extern unsigned int GPIOPinRd(unsigned int moduleId,unsigned int pinNumber);
extern void GPIOPinTogle(unsigned int moduleId, unsigned int pinNumber);
extern unsigned int GPIOMultiplePinsRd(unsigned int moduleId,unsigned int mask);
extern void GPIOMultiplePinsWr(unsigned int moduleId,unsigned int set,unsigned int clr);


#endif
