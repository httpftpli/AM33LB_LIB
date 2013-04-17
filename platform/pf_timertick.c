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
#include "pf_platform_cfg.h"
#include "interrupt.h"

#define INTNUMBER   SYS_INT_TINT2

#ifndef TIMER_TIMERTICK 
#define TIMER_TIMERTICK  SOC_DMTIMER_2_REGS
#endif

static unsigned int tick = 0;
static void (*timertickhandle)(unsigned int tick) = NULL;


typedef struct __softtimer{
   unsigned int tick;
   unsigned int timeElapsed;
   unsigned int enable;
} SOFTTIMER;

volatile  SOFTTIMER softtimer[16];


unsigned int softtimerenable;


static unsigned int timerFindFree(){
   for (int i=0;i<i<sizeof(softtimer)/sizeof(softtimer[0]);i++) {
      if (softtimer[i].enable==0) {
         return i;
      }
   }
   return -1;
}




void isr_DTimer2(unsigned int intnum){
   UNUSED(intnum);
   DMTimerIntStatusClear(TIMER_TIMERTICK,DMTIMER_INT_OVF_IT_FLAG );
   tick++;
   for (int i=0;i<sizeof(softtimer)/sizeof(softtimer[0]);i++) {
      if (softtimer[i].enable) {
         softtimer[i].tick--;
         if(softtimer[i].tick == 0)
            softtimer[i].timeElapsed = 1;   
      }
   }
   if (NULL!=timertickhandle) {
      timertickhandle(tick);
   }
}

int StartTimer(unsigned int mSec){
   int timeindex = timerFindFree();
   if (-1 == timeindex) {
      return -1;
   }else{
      IntSystemDisable(INTNUMBER);      
      softtimer[timeindex].enable = 1;
      softtimer[timeindex].tick = mSec+1;
      softtimer[timeindex].timeElapsed = 0;
      IntSystemEnable(INTNUMBER); 
   }
   return timeindex;
}



void  StopTimer(unsigned int timerindex){
    IntSystemDisable(INTNUMBER); 
    softtimer[timerindex].enable = 0;
    softtimer[timerindex].timeElapsed = 0;
    IntSystemEnable(INTNUMBER);
}
   
      

unsigned int IsTimerElapsed(unsigned int timerindex){
   if (softtimer[timerindex].timeElapsed == 0) {
      return FALSE;
   }else{
      IntSystemDisable(INTNUMBER);
      softtimer[timerindex].enable = 0;
      IntSystemEnable(INTNUMBER);   
   }
   return TRUE;  
}



void TimerTickConfigure(){
   DMTimerReset(TIMER_TIMERTICK);
   DMTimerModeConfigure(TIMER_TIMERTICK, DMTIMER_AUTORLD_NOCMP_ENABLE);
   DMTimerIntEnable(TIMER_TIMERTICK,  DMTIMER_INT_OVF_EN_FLAG);
   DMTimerPreScalerClkEnable(TIMER_TIMERTICK, 2); //CLK_M_OSC =24M 
                                           //PERCALE : 2^(2+1)=8
                                           //clkin = 24/8 = 3
   unsigned int cnt = 0xfffffffe - 1000*3;
   DMTimerReloadSet(TIMER_TIMERTICK, cnt);
   DMTimerTriggerSet(TIMER_TIMERTICK); 

   for (int i=0;i<sizeof(softtimer)/sizeof(softtimer[0]);i++) {
      softtimer[i].enable = 0;
   }
}

void TimerTickRegistHandler(void (*pfnHandler)(unsigned int tick))
{    
    timertickhandle = pfnHandler;
}

void TimerTickPeriodSet(unsigned int microsecond)
{
   unsigned int cnt = 0xffffffe - microsecond*3;
   DMTimerReloadSet(TIMER_TIMERTICK, cnt);
   DMTimerTriggerSet(TIMER_TIMERTICK);
}

void TimerTickStart(void)
{	
   DMTimerEnable(TIMER_TIMERTICK);
}

void TimerTickStop(void)
{
   DMTimerDisable(TIMER_TIMERTICK);
}

unsigned int TimerTickGet(void){
   return tick;
}

unsigned int TimerTickTimeGet(void){
   return DMTimerCounterGet(TIMER_TIMERTICK);
}


void Sysdelay(unsigned int mSec)
{
   unsigned int conter = TimerTickGet();
   if (mSec==0) {
      return;
   }
   while (1) {
      if (TimerTickGet() == (conter+mSec+1)) {
         break;
      }
   }
   return;     
}

void delay(unsigned int milliSec)
{
    Sysdelay(milliSec);
}


