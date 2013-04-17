/**
 * \file   systick.h
 *
 * \brief  This file contains the prototypes of the functions present in
 *         utils/src/systick.c
 */




#ifndef _PF_SYSTICK_H_
#define _PF_SYSTICK_H_

#ifdef __cplusplus
extern "C" {
#endif

extern void TimerTickConfigure( );
extern void TimerTickPeriodSet(unsigned int microsecond);
extern void TimerTickRegistHandler(void (*pfnHandler)(unsigned int tick));
extern void TimerTickStart(void);
extern void TimerTickStop(void);
extern unsigned int TimerTickGet(void);
extern unsigned int TimerTickTimeGet(void);
extern void Sysdelay(unsigned int mSec);
extern void delay(unsigned int milliSec);
extern int StartTimer(unsigned int mSec);
extern unsigned int IsTimerElapsed(unsigned int timerindex);
extern void  StopTimer(unsigned int timerindex);





#ifdef __cplusplus
#endif

#endif

