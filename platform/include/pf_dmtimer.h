#ifndef __PF_DMTIMER__H__
#define __PF_DMTIMER__H__

#include "dmtimer.h"

#define DMTIMER_FLAG_LOADMODE_ONESHORT            1

#define DMTIMER_FLAG_INTENABLE_OVERFLOW           2<<4
#define DMTIMER_FLAG_INTENABLE_MATCH              1<<4
#define DMTIMER_FLAG_INTENABLE_CAPTURE            4<<4



#define DMTIMER_INT_FLAG_CAP     DMTIMER_INT_TCAR_IT_FLAG
#define DMTIMER_INT_FLAG_OVF     DMTIMER_INT_OVF_IT_FLAG
#define DMTIMER_INT_FLAG_MATCH   DMTIMER_INT_MAT_IT_FLAG

#define DMTIMER_FLAG_OUTPUTTRIG_NO                      0<<8
#define DMTIMER_FLAG_OUTPUTTRIG_OVERFLOW                1<<8
#define DMTIMER_FLAG_OUTPUTTRIG_OVERFLOW_AND_MATCH      2<<8

#define DMTIMER_FLAG_OUTPUTPHASE_POSITIVEPULSE     0
#define DMTIMER_FLAG_OUTPUTPHASE_NEGATIVEPULSE     0
#define DMTIMER_FLAG_OUTPUTPHASE_TOGGLE            0


typedef void (*DMTIMERHANDLER)(unsigned int tc,unsigned int intflag);

extern void dmtimerInitForMatch(unsigned int moduleId, unsigned int TCval, unsigned int matchVal, unsigned int flag);
extern void dmtimerInitForOverFlow(unsigned int moduleId, unsigned int TCval , unsigned int flag);
extern void dmtimerRegistHandler(unsigned int moduleId, DMTIMERHANDLER handler);
  
#endif
