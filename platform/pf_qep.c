/**
 *  \file   pf_qep.c
 *
 *  \brief
 *  \author  lfl
 *  \addtogroup QEP
 *  \# include "pf_qep.h"
 *  @{
 *
 */


#include  "pf_qep.h"
#include "interrupt.h"
#include "hw_eqep.h"
#include "soc_AM335x.h"
#include "module.h"
#include "hw_types.h"
#include "debug.h"


const unsigned int POSORIGIN = 0;
//unittime us;unitposition um;positionFactor 0.1um,capTimeFactor ns;
unsigned int unitPosition,positionFactor = 1,capTimeFactor;

static unsigned int oldpos[3];

static unsigned int unitTime;


static volatile signed int velocity[3];

static void (*qephandler[3])(unsigned int flag);



/**
 * @brief 设置位置和编码值的比例因子
 * @param [in] factor
 *        比如位置编码器值加1，对应的实际位置为10us，factor可设置为10
 * @return
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void QEPSetPosFactor(unsigned int factor) {
   positionFactor = factor;
   ASSERT(positionFactor != 0);
}


static void QEPSetInputMode(unsigned int baseAddr, unsigned int mode) {
   unsigned int val = HWREGH(baseAddr + EQEP_QDECCTL);
   val &= ~(3 << 14);
   val |= mode << 14;
   HWREGH(baseAddr + EQEP_QDECCTL) = val;
}



/**
 * @brief
 * 交换正交信号线
 * @param [in] moduleId  \b MODULE_ID_eQEPX
 * @return
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void QEPSwapQuadInput(unsigned int moduleId) {
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   HWREGH(baseAddr + EQEP_QDECCTL) |= 1 << 10;
}




/**
 * @brief qep 初始化
 * @param [in] moduleId  控制器模块号 \b MODULE_ID_eQEPX
 * @param [in] inputmode
 * - QEP_MODE_QUAD
 *   正交模式，如果用于正交编码器，选此模式
 * - QEP_MODE_DERECTION 直接计数模式
 * - QEP_MODE_UPCOUNT 向上计数测频模式
 * - QEP_MODE_DOWNCOUNT 向下计数测频模式
 * @return    NONE
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 *  
 * @see
 */



void QEPInit(unsigned int moduleId, unsigned int inputmode,unsigned int intFlag) {
   unsigned int infreq = modulelist[moduleId].moduleClk->iClk[0]->clockSpeedHz;
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   //soft reset Quadrature position counter
   HWREGH(baseAddr + EQEP_QEPCTL) &= ~(1 << 3);
   QEPSetInputMode(baseAddr, inputmode);
   //disable position compare syn output
   HWREGH(baseAddr + EQEP_QDECCTL) |= 0 << 13;
   unitPosition = positionFactor * 2;
   capTimeFactor = 1000 * 1000 * 8 / infreq;
   //Capture Control:enable; clock prescaler:CAPCLK = SYSCLKOUT/128 ;Unit position:QCLK/2
   HWREGH(baseAddr + EQEP_QCAPCTL) = 1 << 15 | 7 << 4 | 1 << 0;
   //position counter reset on maximum position;
   HWREGHBITARRAY(baseAddr + EQEP_QEPCTL, 12, 2, 1);
   //maximum position :0xffffffff
   HWREG(baseAddr + EQEP_QPOSMAX) = 0xffffffff;
   //campare position :0xffffffff
   HWREG(baseAddr + EQEP_QPOSCMP) = 0xffffffff;
   //int enable  UTO,PCM,PCO,PCU,PHE,PCE,QDC
   HWREGH(baseAddr + EQEP_QEINT) = intFlag ;
   // position compare: enable;  shadow disable;
   HWREGH(baseAddr + EQEP_QPOSCTL) = 1 << 12;
   QEPSetPos(moduleId, 0, QEP_SETPOS_IMMED);
   //enable Quadrature position counter
   HWREGH(baseAddr + EQEP_QEPCTL) |= 1 << 3;
   moduleIntConfigure(moduleId);
}



void  QEPSetStrobe(unsigned int moduleId,bool strobePluseRevers,bool interrupt){
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   HWREGH(baseAddr + EQEP_QDECCTL) |= !!strobePluseRevers << 5;
   HWREGH(baseAddr + EQEP_QPOSCTL) |= 1 << 6;
   if (interrupt) {
       HWREGH(baseAddr + EQEP_QCLR)  |= 1 << 9;
       HWREGH(baseAddr + EQEP_QEINT) |= 1 << 9;
   }else{
       HWREGH(baseAddr + EQEP_QEINT) &= ~(1 << 9);
   }
}



/**
 * @brief 设置当前位置的编码值
 * @param [in] moduleId qep控制器模块ID \b MODULE_ID_eQEPX
 * @param [in] pos   编码值
 * @param [in] setEvent  编码值生效时机
 *
 * - QEP_SETPOS_IMMED --立即生效
 * - QEP_SETPOS_INDEX_RISEEDGE --index信号上升沿生效
 * - QEP_SETPOS_INDEX_FALLEDGE --index信号下降沿生效
 * - QEP_SETPOS_STROBE_RISEEDGE --strobe信号上升沿生效
 * - QEP_SETPOS_STROBE_WISE
 *   --正转时strobe信号上升沿生效，反转时strobe信号下升沿生效
 * @return   NONE
 * @date    2013/7/9
 * @note
 *
 * @code
 * @endcode
 * @pre
 * @see
 */
void QEPSetPos(unsigned int moduleID, unsigned int pos, unsigned int setEvent) {
   unsigned int baseAddr = modulelist[moduleID].baseAddr;
   ASSERT(positionFactor != 0);
   HWREG(baseAddr + EQEP_QPOSINIT) = POSORIGIN + pos / positionFactor;
   unsigned short ctrl = HWREGH(baseAddr + EQEP_QEPCTL);
   if (setEvent & QEP_SETPOS_IMMED) {
      ctrl |= 1 << 7;
   }
   unsigned short val =  (setEvent >> 1) & 0x03; // INDEX
   if (val > 1) {
      ctrl &= ~(0x03 << 8);
      ctrl |= (val << 8);
   }
   val = (setEvent >> 3) & 0x03; // STROBE
   if (val > 1) {
      ctrl &= ~(0x03 << 10);
      ctrl |= (val << 10);
   }
   HWREGH(baseAddr + EQEP_QEPCTL) = ctrl;
}


/**
 * @brief 读当前编码器值
 * @param [in] 模块ID \b MODULE_ID_eQEPX
 * @return  编码器值
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
unsigned int QEPReadPos(unsigned int moduleId) {
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   return (HWREG(baseAddr + EQEP_QPOSCNT) - POSORIGIN) * positionFactor;
}


unsigned int QEPReadDir(unsigned int moduleId) {
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   return ((HWREG(baseAddr + EQEP_QEPSTS)>>5)&0x1);
}



static int QEPCalcuLatchVelocity(unsigned int moduleId) {
   unsigned int index = modulelist[moduleId].index;
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   unsigned int newpos = (HWREG(baseAddr + EQEP_QPOSLAT) - POSORIGIN);
   signed long long  dis = ((signed long long)newpos - (signed long long)oldpos[index])
      * (signed int)positionFactor;
   oldpos[index] = newpos;
   return dis * 1000L * 1000L / (signed int)unitTime; // pulse*factor/s
}



/**
 * @brief
 *        设置编码器比较值，当编码器和比较值相同的时候产生中断
 * @param [in] moduleId \b MODULE_ID_eQEPX
 * @param [in]  compare
 *        比较值，比较值已经乘上了比例因子，所以compare为实际值，不是编码器值
 * @return
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void QEPSetPosCompare(unsigned int moduleId, int compare) {
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   ASSERT(positionFactor != 0);
   HWREG(baseAddr + EQEP_QPOSCMP) = compare / positionFactor + POSORIGIN;
}



/**
 * @brief
 *        设置当前位置为比较值，当到达当前位置的时候产生中断
 * @param [in] moduleId \b MODULE_ID_eQEPX
 * @return
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void QEPSetPosCompareCurrent(unsigned int moduleId) {
   unsigned int val = QEPReadPos(moduleId);
   QEPSetPos(moduleId, val, QEP_SETPOS_IMMED);
}



/**
 * @brief 注册中断回调函数
 * @param [in] moduleId \b MODULE_ID_eQEPX
 * @param [in]  handler 回调函数 \n
 *   handler 的参数 flag:
*   - QEP_HANDER_FlAG_COMPARE_MATCH --比较中断 见
*     QEPSetPosCompareCurrent() , QEPSetPosCompare()
*   - QEP_HANDER_FlAG_PHASE_ERROR
*     --错误中断，比如正交信号错误等
*   - QEP_HANDER_FlAG_POSCNT_ERROR --计数器错误
 *  - QEP_HANDER_FlAG_POSCNT_OVERFLOW  --计数器上溢出
 *  - QEP_HANDER_FlAG_POSCNT_UNDERFLOW --计数器下溢出
 *
 * @return
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void QEPRegistHandler(unsigned int moduleId, void (*handler)(unsigned int flag)) {
   unsigned int index = modulelist[moduleId].index;
   qephandler[index] = handler;
}



/**
 * @brief  获取当前速度
 * @param [in] moduleId \b MODULE_ID_eQEPX
 * @return  当前速度
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
signed int QEPGetVelocity(unsigned int moduleId) {
   unsigned int index = modulelist[moduleId].index;
   return velocity[index];
}



/**
 * @brief
 *        使能速度测试，不用测速时不需要调用此函数
 * @param [in] moduleId \b MODULE_ID_eQEPX
 * @param [in] timeResolution_us    时间分辨率，单位 us
 * @return
 * @date    2013/7/9
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void QEPVelocityDetectStart(unsigned int moduleId, unsigned int timeResolution_us) {
   unsigned int index = modulelist[moduleId].index;
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   unsigned int inFreq = modulelist[moduleId].moduleClk->iClk[0]->clockSpeedHz;
   unitTime = timeResolution_us;
   unsigned long long tick = inFreq / 1000000 * timeResolution_us;
   ASSERT(tick <= 0xffffffff);
   oldpos[index] = (HWREG(baseAddr + EQEP_QPOSCNT) - POSORIGIN);
   HWREG(baseAddr + EQEP_QUPRD) = (unsigned int)tick;
   //capture latch mode :Latch on unit time out ;eQEP unit timer enable
   HWREGH(baseAddr + EQEP_QEPCTL) |= 1 << 2 | 1 << 1;
}


void isr_qep(unsigned intnum) {
   unsigned int baseaddr = modulelist[intnum].baseAddr;
   unsigned int index = modulelist[intnum].index;
   //read interrupt status
   unsigned short stat = HWREGH(baseaddr + EQEP_QFLG);
   //clear interrupt status
   HWREGH(baseaddr + EQEP_QCLR) = stat;
   //masked by interrupt enable  value
   stat &= HWREGH(baseaddr + EQEP_QEINT); 
   if (stat & 1 << 11) { //UTO  calculate velocity
      velocity[index] = QEPCalcuLatchVelocity(intnum);
   }
   if ((qephandler[index] == NULL)) {
       return;
   }
   if (stat & 1 << 8) { // compare match event
      qephandler[index](QEP_HANDER_FlAG_COMPARE_MATCH);
   }
   if (stat & 1 << 2) { //Quadrature phase error
      qephandler[index](QEP_HANDER_FlAG_PHASE_ERROR);
   }
   if (stat & 1 << 3) { //Dir change
      qephandler[index](QEP_HANDER_FlAG_DIR_CHANGE);
   }
   if (stat & 1 << 1) { //Quadrature phase error
      qephandler[index](QEP_HANDER_FlAG_POSCNT_ERROR);
   }
   if (stat & 1 << 6) { //Position counter overflow
      qephandler[index](QEP_HANDER_FlAG_POSCNT_OVERFLOW);
   }
   if (stat & 1 << 5) { //Position counter underflow
      qephandler[index](QEP_HANDER_FlAG_POSCNT_UNDERFLOW);
   }
   if (stat & 1 << 9){ //strobe
      qephandler[index](QEP_HANDER_FlAG_STROB);
   }
}

//! @}
