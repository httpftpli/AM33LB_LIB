/**
 * \file   sysperf.c
 *
 * \brief  This file contains functions that configures a DMTimer instance
 *         for performance measurement.
 *
*/

#include "soc_AM335x.h"
#include "interrupt.h"
#include "dmtimer.h"
#include "perf.h"

/******************************************************************************
**                      INTERNAL MACRO DEFINITIONS
*******************************************************************************/
#define TIMER_INITIAL_COUNT             (0xFFFFA23Fu)
#define TIMER_PERF_BASE                 (SOC_DMTIMER_7_REGS)

/******************************************************************************
**                      INTERNAL VARIABLE DEFINITIONS
*******************************************************************************/
static volatile unsigned int flagIsr = 1;

/******************************************************************************
**                      FUNCTION DEFINITIONS
*******************************************************************************/
/*
** This function sets up timer for performance measurement
*/
void SysPerfTimerSetup(void)
{
    /* This function will enable clocks for the DMTimer7 instance */
    DMTimer7ModuleClkConfig();

    DMTimerCounterSet(TIMER_PERF_BASE, 0);

}

/*
** Configures the performance timer to start or stop timer
** @param  flag   '0', stop the timer and read the value
**                non-zero value to start timer
** /NOTE  This function shouldnot be called when SysStartTimer, SysStopTimer,
**        SysIsTimerElapsed or Sysdelay functionality is in use and vice Versa.
**             Maximim Duration is 171 Sec.
**
*/
unsigned int SysPerfTimerConfig(unsigned int flag)
{
    unsigned int timeInTicks = 0;

    if(flag)
    {
        DMTimerCounterSet(TIMER_PERF_BASE, 0);
        DMTimerEnable(TIMER_PERF_BASE);
    }
    else
    {
        DMTimerDisable(TIMER_PERF_BASE);
        timeInTicks = DMTimerCounterGet(TIMER_PERF_BASE);
    }

    return timeInTicks;
}


