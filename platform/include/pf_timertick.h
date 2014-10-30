/**
 * \file   systick.h
 *
 * \brief  This file contains the prototypes of the functions present in
 *         utils/src/systick.c
 */




#ifndef _PF_SYSTICK_H_
#define _PF_SYSTICK_H_


#include "list.h"

typedef struct {
    void (*fun)(void);
    unsigned int delay;
    struct list_head list;
}TASKLET;



#ifdef __cplusplus
extern "C"
{
#endif

    extern void TimerTickConfigure();
    extern void TimerTickPeriodSet(unsigned int moduleId, unsigned int microsecond);
    extern void TimerTickRegistHandler(void (*pfnHandler)(unsigned int tick));
    extern void TimerTickStart(void);
    extern void TimerTickStop(void);
    extern unsigned int TimerTickGet(void);
    extern unsigned int TimerTickTimeGet(void);
    extern void Sysdelay(unsigned int mSec);
    extern int StartTimer(unsigned int mSec);
    extern unsigned int IsTimerElapsed(unsigned int timerindex);
    extern void  StopTimer(unsigned int timerindex);
#if USE_TASK_DELAYDO == 1
    extern bool taskdelaydo(unsigned int delay, void (*fun)(void));
#endif


#define withintimedo(TIMENAME,time) \
            for(unsigned int TIMENAME=TimerTickGet();\
                TimerTickGet()<=(time+TIMENAME);\
                )
#define everytimedo(TIMENAME,time)\
                static unsigned int TIMENAME=0;\
                for(;TIMENAME<=TimerTickGet();TIMENAME=TimerTickGet()+time )


//#define DEFINE_TASKLET(NAME) \
//             TASKLET NAME = {NULL,0,{&NAME.list,&NAME.list}}

#ifdef __cplusplus
#endif

#endif





