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

extern void TimerTickConfigure(unsigned int milliSec);
extern void TimerTickPeriodSet(unsigned int milliSec);
extern void TimerTickRegistHandler(void (*pfnHandler)(unsigned int tick));
extern void TimerTickStart(void);
extern void TimerTickStop(void);


#ifdef __cplusplus
}
#endif

#endif

