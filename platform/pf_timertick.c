/**
 *  \file   pf_timertick.c
 *
 *  \brief
 *  \author  李飞亮  
 *  \addtogroup TIMETICK
 *  @brief   时钟节拍
 *  
 *  每1ms产生一个时钟节拍,用于定时执行某个任务，延时，软时钟等
 *  \n
 *  \#include "pf_timertick.h"
 *  @{
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
   if (NULL!=timertickhandle) {
      timertickhandle(tick);
   }
}


/**
 * @brief 读取定时器滴答值 
 * @return  NONE         
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
unsigned int TimerTickGet(void){
   return tick;
}


/**
 * @brief 启动一个延时时钟
 * @param [in] mSec 延时值，单位ms 
 * @return 
 * - -1 失败 
 * - >=0 延时时钟的索引 
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  
 */
int StartTimer(unsigned int mSec){
   int timeindex = timerFindFree();
   if (-1 == timeindex) {
      return -1;
   }else{          
      softtimer[timeindex].enable = 1;
      softtimer[timeindex].tick = TimerTickGet() + mSec+1; 
   }
   return timeindex;
}



/**
 * @brief 停止延时时钟 
 *  
 * 延时时钟如果被停止，该索引号不能继续使用，不能在调 
 * 用 IsTimerElapsed() 查询时间到
 * @param [timerindex] 时钟索引，由  StartTimer() 返回 
 * @return   NONE        
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre 
 *  StartTimer()
 * @see 
 */
void  StopTimer(unsigned int timerindex){   
    softtimer[timerindex].enable = 0;
}
   
      

/**
 * @brief 查询延时定时器是否到 
 *  
 * 如果该函数返回 \b TRUE ,由 \b timerindex 
 * 索引的时钟停止，该 timerindex不能继续使用
 *  
 * @param [in] timerindex 时钟索引，由  StartTimer() 
 *        返回
 * @return 
 * - FALSE 时间没到 
 * - TRUE 时间到 
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre 
 * StartTimer() 
 * @see 
 */
unsigned int IsTimerElapsed(unsigned int timerindex){
   if (softtimer[timerindex].tick > TimerTickGet()) {
      return FALSE;
   }else{      
      softtimer[timerindex].enable = 0;
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
      if (TimerTickGet() >= (conter+mSec+1)) {
         break;
      }
   }
   return;     
}

void delay(unsigned int milliSec)
{
    Sysdelay(milliSec);
}

//! @}
