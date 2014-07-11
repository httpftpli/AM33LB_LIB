/**
 * \file   gpio.c
 *
 * \brief  This file contains functions which performs the platform specific
 *         configurations of GPIO.
 *
 *
 * @addtogroup GPIO
 * @brief GPIO
 *
 * \#include  "gpio_v2.h"
 * @{
 */



#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "hw_cm_wkup.h"
#include "platform.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "interrupt.h"
#include "hw_gpio_v2.h"
#include "debug.h"
#include "gpio_v2.h"
#include "module.h"




const unsigned char  unMapTbl[256] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};

typedef void (*GPIOIntHandler)();

static  GPIOIntHandler handler[4][32];

void isr_Gpio(unsigned int intnum){
   unsigned int index = modulelist[intnum].index;
   unsigned int baseaddr = modulelist[intnum].baseAddr;
   unsigned int irq = HWREG(baseaddr + GPIO_IRQSTATUS(0));
   unsigned char irqbyte;
   unsigned char irqpin;
   for (int i=0;i<4;i++) {
      irqbyte = *(unsigned char *)(&irq+i);
      if (irqbyte==0) {
         continue;
      }else{
         irqpin = i<<3+unMapTbl[irqbyte];
         if (handler != NULL) {
            handler[index][irqpin]();
         }
         HWREG(baseaddr + GPIO_IRQSTATUS(0)) = 1<<irqpin ;//clear irq;
      }
   }
}


/**
 * @brief
 *        注册GPIO中断服务程序
 * @param [in] modulOfGpio
 *        GPIO模块号，例如GPIO2_8,modulOfGpio=2
 * @param [in] bitOfGpio GPIO位,例如GPIO2_8, bitOfGpio=8
 * @param [in]  gpiohandler
 * @return  void
 * @date    2013/5/7
 * @note
 * 只有响应GPIO使能中断的情况下，才会执行gpiohandler函数，gpiohandler在中断线程中执行
 * @code
 *
 * @endcode
 *
 * @pre
 *
 * @see
 */
void GPIORegistHandler(unsigned char modulOfGpio,unsigned bitOfGpio,
                       void (*gpiohandler)()){
   mdAssert(modulOfGpio<4);
   mdAssert(bitOfGpio<32);
   handler[modulOfGpio][bitOfGpio] = gpiohandler;
}


/**
 * @brief 删除GPIO中断服务程序
 * @param [in] modulOfGpio 见 GPIORegistHandler()
 * @param [in] bitOfGpio 见 GPIORegistHandler()
 * @return
 * @date    2013/5/7
 * @note
 * @code
 *
 * @endcode
 *
 * @pre
 *
 * @see
 */
void GPIOClearHandler(unsigned char modulOfGpio, unsigned bitOfGpio){
   mdAssert(modulOfGpio<4);
   mdAssert(bitOfGpio<32);
   handler[modulOfGpio][bitOfGpio] = (GPIOIntHandler)0;
}


/**
 * @brief GPIO初始化
 * @param [in] moduleId GPIOID 模块,\b MODULE_ID_GPIOX
 * @param [in] debounceTimer GPIO消抖动时间
 * @param [in] debounceEnableBitmap
 *        32位gpio使能消抖为映射
 * @return  NONE
 * @date    2013/5/29
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void GPIOInit(unsigned int moduleId, int debounceTimer,unsigned int debounceEnableBitmap) {
   static unsigned int moduleinited0,moduleinited1,moduleinited2,moduleinited3;
   switch(moduleId) {
   case MODULE_ID_GPIO0:
      if (1 == moduleinited0) {
         return;
      } else {
         moduleinited0 = 1;
      }
      break;
   case MODULE_ID_GPIO1:
      if (1 == moduleinited1) {
         return;
      } else {
         moduleinited1 = 1;
      }
      break;
   case MODULE_ID_GPIO2:
      if (1 == moduleinited2) {
         return;
      } else {
         moduleinited2 = 1;
      }
      break;
   case MODULE_ID_GPIO3:
      if (1 == moduleinited3) {
         return;
      } else {
         moduleinited3 = 1;
      }
      break;
   default:
      return;
   }
   moduleEnable(moduleId);
   unsigned int baseaddr = modulelist[moduleId].baseAddr;
   GPIOModuleEnable(baseaddr);
   GPIOModuleReset(baseaddr);
   GPIOIdleModeConfigure(baseaddr, GPIO_IDLE_MODE_NO_IDLE);
   GPIOAutoIdleModeControl(baseaddr, GPIO_AUTO_IDLE_MODE_DISABLE);
   GPIOAutoIdleModeControl(baseaddr, GPIO_AUTO_IDLE_MODE_DISABLE);
   GPIODebounceTimeConfig(baseaddr,debounceTimer);
   HWREG(baseaddr + GPIO_DEBOUNCENABLE) = debounceEnableBitmap;
   moduleIntConfigure(moduleId);
}



void GPIODirSet(unsigned int moduleId,
                    unsigned int pinNumber,
                    unsigned int pinDirection){
    unsigned int baseaddr = modulelist[moduleId].baseAddr;
    GPIODirModeSet(baseaddr,pinNumber,pinDirection);
}

void GPIODirGet(unsigned  int moduleId,unsigned int pinNumber){
    unsigned int baseaddr = modulelist[moduleId].baseAddr;
    GPIODirModeGet(baseaddr,pinNumber);
}


void GPIOPinWr(unsigned int moduleId, unsigned int pinNumber, unsigned int value) {
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    GPIOPinWrite(baseAdd,pinNumber,value);
}


unsigned int GPIOPinRd(unsigned int moduleId,
                         unsigned int pinNumber){
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    return GPIOPinRead(baseAdd,pinNumber);
}

void GPIOPinTogle(unsigned int moduleId, unsigned int pinNumber) {
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    unsigned int dataout =HWREG(baseAdd + GPIO_DATAOUT);
    dataout &= 1<<pinNumber;
    GPIOPinWrite(baseAdd,pinNumber,!dataout);
}
//! @}
/****************************** End of file *********************************/
