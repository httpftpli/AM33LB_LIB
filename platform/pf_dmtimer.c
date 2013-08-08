/**
 *  \file   pf_dmtimer.c
 *
 *  \brief
 *  \author  lfl 
 *  \addtogroup TIMER
 *  \# include "pf_dmtimer.h"
 *  @{
 *   
 */



#include "soc_AM335x.h"
#include "hw_types.h"
#include "dmtimer.h"
#include "pf_dmtimer.h"
#include "module.h"
#include "mmath.h"



static DMTIMERHANDLER dmtimerhandler[7];


void isr_dmtimer(unsigned int num){ 
   unsigned int addr = modulelist[num].baseAddr;
   unsigned int index = modulelist[num].index;
   unsigned int val = DMTimerIntStatusGet(addr);
   DMTimerIntStatusClear(addr,val );
   unsigned int tc = DMTimerCounterGet(addr);
   if (dmtimerhandler[index]) {
      dmtimerhandler[index](tc,val);
   }
}


/**
 * @briefb 
 *         注册dmtimer中断回调函数，类型是DMTIMERHANDLER
 *  
 * DMTIMERHANDLER的第一个参数是当前定时器的值，第二个参数是中断标记如下值的位组合:
 * \n\r
 * - DMTIMER_INT_FLAG_CAP -- 捕获中断
 * - DMTIMER_INT_FLAG_OVF -- 溢出中断
 * - DMTIMER_INT_FLAG_MATCH -- 匹配中断
 * @param [in] moduleId  
 * @param [in] handler 
 * @return   NONE        
 * @date    2013/8/1
 * @note 
 * @pre 
 * @see DMTIMERHANDLER
 */
void dmtimerRegistHandler(unsigned int moduleId, DMTIMERHANDLER handler){
   unsigned int index = modulelist[moduleId].index;
   dmtimerhandler[index] = handler; 
}


/**
 * @brief 初始化定时器作为匹配使用
 * @param [in] moduleId 定时器模块ID 
 * @param [in] TCval 
 *        定时器TC初始值，当定时器为设置为reload模式时，reload
 *        后的值也是该值
 * @param [in] matchVal  定时器配置值
 * @param [in] flag
 * - DMTIMER_FLAG_LOADMODE_ONESHORT  -- 
 *   ONESHORT模式，定时器溢出后自动停止,如果没有设定此标记就是AUTORELOAD模式,AUTORELOAD模式，当定时器溢出后自动载入初始值继续开始
 *   \n\r
 * - DMTIMER_FLAG_INTENABLE_OVERFLOW -- 使能溢出中断      
 * - DMTIMER_FLAG_INTENABLE_MATCH  -- 使能匹配中断 
 * - DMTIMER_FLAG_OUTPUTTRIG_NO -- 引脚不输出
 * - DMTIMER_FLAG_OUTPUTTRIG_OVERFLOW -- 
 *   当溢出时引脚输出
 * - DMTIMER_FLAG_OUTPUTTRIG_OVERFLOW_AND_MATCH  -- 
 *   当溢出或者匹配时是引脚输出 \n\r
 * - DMTIMER_FLAG_OUTPUTPHASE_POSITIVEPULSE --  
 *   引脚输出正脉冲 \n\r
 * - DMTIMER_FLAG_OUTPUTPHASE_NEGATIVEPULSE -- 
 *   引脚输出负脉冲 \n\r
 * - DMTIMER_FLAG_OUTPUTPHASE_TOGGLE -- 引脚高低切换 \n\r
 * @return   none        
 * @date    2013/7/28
 * @note
 * @code
 * @endcode
 * @pre 
 * 如果定时器要硬件产生引脚输出，必须先配置IO 
 * MUX 
 * @see dmtimerInitForOverFlow
 *  
 */
void dmtimerInitForMatch(unsigned int moduleId, unsigned int TCval ,unsigned int matchVal, unsigned int flag) {
   moduleEnable(moduleId);
   unsigned int baseaddr = modulelist[moduleId].baseAddr;
   DMTimerReset(baseaddr);
   DMTimerModeConfigure(baseaddr, DMTIMER_AUTORLD_CMP_ENABLE);

   /* Clear the AR and CE field of TCLR */
   unsigned int val = HWREG(baseaddr + DMTIMER_TCLR);
   if(!(flag & DMTIMER_FLAG_LOADMODE_ONESHORT)){
      bitSet(val,1);
   }
   bitSet(val,6);
   //if (flag & 0x DMTIMER_FLAG_OUTPUTTRIG_OVERFLOW ) {
   //}
   //todo trig

   HWREG(baseaddr + DMTIMER_TCLR) = val;

   DMTimerIntEnable(baseaddr, (flag&0x00000070)>>4);
   DMTimerCompareSet(baseaddr, matchVal);
   DMTimerIntStatusClear(baseaddr, DMTIMER_INT_TCAR_IT_FLAG | DMTIMER_INT_OVF_IT_FLAG | DMTIMER_INT_MAT_IT_FLAG);
   DMTimerReloadSet(baseaddr, TCval);
   DMTimerCounterSet(baseaddr, TCval);
   moduleIntConfigure(moduleId);
   DMTimerEnable(baseaddr);
}


/**
 * @brief 初始化定时器作为溢出使用
 * @param [in] moduleId 定时器模块ID 
 * @param [in] TCval 
 *        定时器TC初始值，当定时器为设置为reload模式时，reload
 *        后的值也是该值
 * @param [in] flag
 * - DMTIMER_FLAG_LOADMODE_ONESHORT  -- 
 *   ONESHORT模式，定时器溢出后自动停止,如果没有设定此标记就是AUTORELOAD模式,AUTORELOAD模式，当定时器溢出后自动载入初始值继续开始
 *   \n\r
 * - DMTIMER_FLAG_INTENABLE_OVERFLOW -- 使能溢出中断 
 * - DMTIMER_FLAG_OUTPUTTRIG_NO -- 引脚不输出
 * - DMTIMER_FLAG_OUTPUTTRIG_OVERFLOW -- 
 *   当溢出时引脚输出
 * - DMTIMER_FLAG_OUTPUTPHASE_POSITIVEPULSE --  
 *   引脚输出正脉冲 \n\r
 * - DMTIMER_FLAG_OUTPUTPHASE_NEGATIVEPULSE -- 
 *   引脚输出负脉冲 \n\r
 * - DMTIMER_FLAG_OUTPUTPHASE_TOGGLE -- 引脚高低切换 \n\r
 * @return   none        
 * @date    2013/7/28
 * @note
 * @code
 * @endcode
 * @pre
 * @see  dmtimerInitForMatch
 */
void dmtimerInitForOverFlow(unsigned int moduleId, unsigned int TCval ,unsigned int flag) {
   dmtimerInitForMatch(moduleId, TCval, 0 ,flag &(~(DMTIMER_FLAG_INTENABLE_MATCH&DMTIMER_FLAG_OUTPUTTRIG_OVERFLOW_AND_MATCH)));
}



//! @}
