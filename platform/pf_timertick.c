/**
 * \file   systick.c
 *
 * \brief  system timer tick routines
 *
*/

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


#include "dmtimer.h"
#include "soc_AM335x.h"

#define TIMERTICK   SOC_DMTIMER_2_REGS


static void (*timertickhandle)(unsigned int tick) = NULL;


void isr_DTimer2(unsigned int intnum){
   UNUSED(intnum);
   static unsigned int tick = 0;
   DMTimerIntStatusClear(SOC_DMTIMER_2_REGS,DMTIMER_INT_OVF_IT_FLAG );
   if (NULL!=timertickhandle) {
      timertickhandle(tick++);
   }
}



void TimerTickConfigure(unsigned milliSec){
   DMTimerReset(TIMERTICK);
   DMTimerModeConfigure(TIMERTICK, DMTIMER_AUTORLD_NOCMP_ENABLE);
   DMTimerIntEnable(TIMERTICK,  DMTIMER_INT_OVF_EN_FLAG);
   DMTimerPreScalerClkEnable(TIMERTICK, 2); //CLK_M_OSC =24M 
                                           //PERCALE : 2^(2+1)=8
                                           //clkin = 24/8 = 3
   unsigned int cnt = 0xfffffffe - milliSec*3;
   DMTimerReloadSet(TIMERTICK, cnt);
   DMTimerTriggerSet(TIMERTICK);
}

void TimerTickRegistHandler(void (*pfnHandler)(unsigned int tick))
{    
    timertickhandle = pfnHandler;
}

void TimerTickPeriodSet(unsigned int usTime)
{
   unsigned int cnt = 0xffffffe - usTime*3;
   DMTimerReloadSet(TIMERTICK, cnt);
   DMTimerTriggerSet(TIMERTICK);
}

void TimerTickStart(void)
{	
   DMTimerEnable(TIMERTICK);
}

void TimerTickStop(void)
{
   DMTimerDisable(TIMERTICK);
}



