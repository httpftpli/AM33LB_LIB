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



#include "dmtimer.h"
#include "soc_AM335x.h"
#include "pf_platform_cfg.h"
#include "interrupt.h"
#include "module.h"
#include "pf_dmtimer.h"
#include "pf_beep.h"

#define INTNUMBER   SYS_INT_TINT2

#ifndef TIMER_TIMERTICK 
#define TIMER_TIMERTICK  MODULE_ID_TIMER2
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
   for (int i=0;i<sizeof(softtimer)/sizeof(softtimer[0]);i++) {
      if (softtimer[i].enable==0) {
         return i;
      }
   }
   return -1;
}

extern volatile unsigned int  beepdeuation;

static void dmtimertimetickhandler(unsigned int tc, unsigned int intFlag) {
   if (intFlag & DMTIMER_INT_FLAG_OVF) {
      tick++;
      //for beep
      if ((beepdeuation != 0) && (--beepdeuation == 0)) {
         BEEP_OFF();
      }
      if (NULL != timertickhandle) {
         timertickhandle(tick);
      }
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




/**
 * @brief 初始化周期滴答定时器
 * @param [in] moduleId 
 *        定时器模块号，必须是未使用的定时器
 * @return  none  
 * @date    2013/8/1
 * @note 
 * @pre
 * @see 
 */
void TimerTickConfigure(unsigned int moduleId){
   moduleEnable(moduleId);
   unsigned int baseaddr = modulelist[moduleId].baseAddr;
   DMTimerReset(baseaddr);
   DMTimerModeConfigure(baseaddr, DMTIMER_AUTORLD_NOCMP_ENABLE);
   DMTimerIntEnable(baseaddr,  DMTIMER_INT_OVF_EN_FLAG);
   //DMTimerPreScalerClkEnable(baseaddr, 2); //CLK_M_OSC =24M 
                                           //PERCALE : 2^(2+1)=8
                                           //clkin = 24/8 = 3
   unsigned int  inclk = modulelist[moduleId].moduleClk->fClk[0]->clockSpeedHz;
   unsigned int cnt = 0xfffffffe - inclk/1000;  //1ms
   DMTimerReloadSet(baseaddr, cnt);
   DMTimerCounterSet(baseaddr,cnt); 

   for (int i=0;i<sizeof(softtimer)/sizeof(softtimer[0]);i++) {
      softtimer[i].enable = 0;
   }
   dmtimerRegistHandler(moduleId,dmtimertimetickhandler);
   moduleIntConfigure(moduleId);
}



void TimerTickRegistHandler(void (*pfnHandler)(unsigned int tick))
{    
    timertickhandle = pfnHandler;
}


void TimerTickPeriodSet(unsigned int microsecond)
{
   unsigned int cnt = 0xffffffe - microsecond*3;
   unsigned int baseaddr = modulelist[TIMER_TIMERTICK].baseAddr; 
   DMTimerReloadSet(baseaddr, cnt);
   DMTimerTriggerSet(baseaddr);
}

void TimerTickStart(void)
{	
   unsigned int baseaddr = modulelist[TIMER_TIMERTICK].baseAddr;
   DMTimerEnable(baseaddr);
}

void TimerTickStop(void)
{
    unsigned int baseaddr = modulelist[TIMER_TIMERTICK].baseAddr; 
    DMTimerDisable(baseaddr);
}



unsigned int TimerTickTimeGet(void){
   unsigned int baseaddr = modulelist[TIMER_TIMERTICK].baseAddr; 
   return DMTimerCounterGet(baseaddr);
}


void Sysdelay(unsigned int mSec)
{
   unsigned int counter = TimerTickGet();
   if (mSec==0) {
      return;
   }
   while (1) {
      if (TimerTickGet() >= (counter+mSec+1)) {
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
