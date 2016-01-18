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
#include "pf_platform.h"
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
   for (int i=0;i<32;i++) {;
      if (irq & 1<<i) {
         if (handler != NULL) {
            handler[index][i]();
         }
         HWREG(baseaddr + GPIO_IRQSTATUS(0)) = 1<<i ;//clear irq;
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
   ASSERT(modulOfGpio<4);
   ASSERT(bitOfGpio<32);
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
   ASSERT(modulOfGpio<4);
   ASSERT(bitOfGpio<32);
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
    __GPIOPinWrite(baseAdd,pinNumber,value);
}


unsigned int GPIOMultiplePinsRd(unsigned int moduleId,unsigned int mask){
    unsigned int baseAddr = modulelist[moduleId].baseAddr;
    return GPIOMultiplePinsRead(baseAddr,mask);
}

void GPIOMultiplePinsWr(unsigned int moduleId,unsigned int set,unsigned int clr){
    unsigned int baseAddr = modulelist[moduleId].baseAddr;
    GPIOMultiplePinsWrite(baseAddr,set,clr);
}


unsigned int GPIOPinRd(unsigned int moduleId,
                         unsigned int pinNumber){
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    return __GPIOPinRead(baseAdd,pinNumber);
}

void GPIOPinTogle(unsigned int moduleId, unsigned int pinNumber) {
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    unsigned int dataout =HWREG(baseAdd + GPIO_DATAOUT);
    dataout &= 1<<pinNumber;
    __GPIOPinWrite(baseAdd,pinNumber,!dataout);
}

/**
 * \brief  This API set the GPIO PIN interrrupt
 *
 *         Whenever the selected event occurs on that GPIO pin and if interrupt
 *         generation is enabled for that pin, the GPIO module will send an
 *         interrupt to CPU.
 *
 * \param  moduleId    MODULE_ID_GPIOX
 * \param  pinNumber  The number of the pin in the GPIO instance
 * \param  eventType  This specifies the event type on whose detection,
 *                    the GPIO module will send an interrupt to CPU,
 *                    provided interrupt generation for that pin is enabled.
 *
 * 'pinNumber' can take one of the following values:
 * (0 <= pinNumber <= 31)\n
 *
 * 'eventType' can take one of the following values:
 * - GPIO_INT_TYPE_LEVEL_LOW - interrupt request on occurence of a LOW level
 *   (logic 0) on the input GPIO pin\n
 * - GPIO_INT_TYPE_LEVEL_HIGH - interrupt request on occurence of a HIGH level
 *   (logic 1) on the input GPIO pin\n
 * - GPIO_INT_TYPE_BOTH_LEVEL - interrupt request on the occurence of both the
 *   LOW level and HIGH level on the input GPIO pin\n
 * - GPIO_INT_TYPE_RISE_EDGE - interrupt request on occurence of a rising edge
 *   on the input GPIO pin\n
 * - GPIO_INT_TYPE_FALL_EDGE - interrupt request on occurence of a falling edge
 *   on the input GPIO pin\n
 * - GPIO_INT_TYPE_BOTH_EDGE - interrupt request on occurence of both a rising
 *   and a falling edge on the pin\n
 *
 * \return  None
 */

void GPIOPinIntEnable(unsigned int moduleId,unsigned int pinNumber,unsigned int intTrigFlag){
    ASSERT(pinNumber<32);
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    __GPIOPinIntDisable(baseAdd,GPIO_INT_LINE_1,pinNumber);
    GPIOIntTypeSet(baseAdd, pinNumber, intTrigFlag);
    __GPIOPinIntEnable(baseAdd,GPIO_INT_LINE_1,pinNumber);
}

void GPIOPinIntDisable(unsigned int moduleId,unsigned int pinNumber,unsigned int intTrigFlag){
    ASSERT(pinNumber<32);
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    __GPIOPinIntDisable(baseAdd,GPIO_INT_LINE_1,pinNumber);
}

//! @}
/****************************** End of file *********************************/
