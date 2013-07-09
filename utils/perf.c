/**
 *  \file   perf.c
 *
 *  \brief  This file contains APIs for measuring performance using
 *          DMTimer.
*/




#include "perf.h"

/****************************************************************************
**                        FUNCTION DEFINITION
****************************************************************************/
/**
 * \brief   This function configures a DMTimer instance for performance
 *          measurement.
 *
 * \param   None
 *
 * \return  None.
 *
 */
void PerfTimerSetup(void)
{
    SysPerfTimerSetup();
}

/**
 * \brief   This function starts performance timer
 *
 * \param   None
 *
 * \return  None
 */
void PerfTimerStart(void)
{
    SysPerfTimerConfig(1);
}

/**
 * \brief   This function stops performance timer and returns the time
 *          elapsed in number of ticks
 *
 * \param   None
 *
 * \return  Time elapsed in ticks
 */
unsigned int PerfTimerStop(void)
{
    return (SysPerfTimerConfig(0));
}


