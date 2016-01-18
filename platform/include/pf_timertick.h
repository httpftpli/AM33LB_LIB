/**
 * \file   systick.h
 *
 * \brief  This file contains the prototypes of the functions present in
 *         utils/src/systick.c
 */




#ifndef _PF_SYSTICK_H_
#define _PF_SYSTICK_H_


#include "list.h"
#include "type.h"

typedef struct {
    void (*fun)(void);
    unsigned long long ___delay;
    bool runOnce;
    uint32 timespan;
    struct list_head list;
}TASKLET;



#ifdef __cplusplus
extern "C"
{
#endif

    extern void TimerTickConfigure();
    extern void TimerTickPeriodSet(unsigned int moduleId, unsigned int microsecond);
    extern void TimerTickRegistHandler(void (*pfnHandler)(unsigned long long tick));
    extern void TimerTickStart(void);
    extern void TimerTickStop(void);
    extern unsigned int TimerTickGet(void);
    extern unsigned long long TimerTickGet64(void);
    extern unsigned int TimerTickTimeGet(void);
    extern unsigned long long TimerTickTimeGetUs(void);
    extern void Sysdelay(unsigned int mSec);
    extern int StartTimer(unsigned int mSec);
    extern unsigned int IsTimerElapsed(unsigned int timerindex);
    extern void  StopTimer(unsigned int timerindex);
#if USE_TASK_DELAYDO == 1
    extern TASKLET* taskdelaydo(unsigned int delay, void (*fun)(void),bool runOnce,uint32 timespan);
    extern void taskdelaydoDel(TASKLET *task);
#endif


#define withintimedo(time) \
            for(unsigned long long DECLARE_VAR_NAME_UNIQUE(TIMENAME)=TimerTickGet64();\
                TimerTickGet64()<=(time+DECLARE_VAR_NAME_UNIQUE(TIMENAME));\
                )
#define everytimedo(time)\
                static unsigned long long DECLARE_VAR_NAME_UNIQUE(TIMENAME)=0;\
                for(;DECLARE_VAR_NAME_UNIQUE(TIMENAME)<=TimerTickGet64();\
                  DECLARE_VAR_NAME_UNIQUE(TIMENAME)=TimerTickGet64()+time )

#define delaytimedo_once(TIMENAME,time)\
                static unsigned long long DECLARE_VAR_NAME_UNIQUE(TIMENAME)=TimerTickGet64();\
                for(;DECLARE_VAR_NAME_UNIQUE(TIMENAME)+(time) <=TimerTickGet64()\
                  ;DECLARE_VAR_NAME_UNIQUE(TIMENAME)=-1ULL)


//#define DEFINE_TASKLET(NAME) \
//             TASKLET NAME = {NULL,0,{&NAME.list,&NAME.list}}

#ifdef __cplusplus
#endif

#endif





