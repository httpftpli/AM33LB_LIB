#ifndef __DELAY_H__
#define __DELAY_H__

extern void delay(unsigned int milliSec);
extern void delayus(unsigned int uSec);
extern int StartTimer(unsigned int mSec);
extern unsigned int IsTimerElapsed(unsigned int timerindex);
extern void  StopTimer(unsigned int timerindex);

#endif
