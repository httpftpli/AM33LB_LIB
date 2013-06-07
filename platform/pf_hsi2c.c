/**
 * \file   hsi2c.c
 *
 * \brief  This file contains functions which configure the hsi2c
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


#include "hw_control_AM335x.h"
#include "hw_cm_wkup.h"
#include "soc_AM335x.h"
#include "platform.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "hsi2c.h"
#include "type.h"
#include "module.h"
#include "mmath.h"


#define I2C_MODE_SLAVE     0
#define I2C_MODE_MASTER    1
/**
 * \brief   This function will configure the required clocks for I2C1 instance.
 *
 * \return  None.
 *
 */
void I2C1ModuleClkConfig(void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) |= 
                             CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) & 
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) |= 
                             CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) & 
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) |= 
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) & 
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) != 
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) |= 
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) & 
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |= 
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) & 
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) != 
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) |= 
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) & 
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) != 
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) |= 
                             CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) & 
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_I2C1_CLKCTRL) |= 
                             CM_PER_I2C1_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_I2C1_CLKCTRL) & 
      CM_PER_I2C1_CLKCTRL_MODULEMODE) != CM_PER_I2C1_CLKCTRL_MODULEMODE_ENABLE);

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) & 
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) & 
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) & 
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK | 
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK)));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) & 
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK | 
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_I2C_FCLK)));
    
}


/*
** This function enables the system L3 and system L4_WKUP clocks.
** This also enables the clocks for I2C0 instance.
*/

void I2C0ModuleClkConfig(void)
{
    /* Configuring L3 Interface Clocks. */

    /* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
          CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
          CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
          CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
          CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
           CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));

    /* Checking fields for necessary values.  */

    /* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
    while((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
           CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
    ** attain the desired value.
    */
    while(CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
    ** register to attain the desired value.
    */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
          CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));


    /* Configuring registers related to Wake-Up region. */

    /* Writing to MODULEMODE field of CM_WKUP_CONTROL_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) |=
          CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) |=
          CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_L3_AON_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) |=
          CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL));

    /* Writing to MODULEMODE field of CM_WKUP_I2C0_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_I2C0_CLKCTRL) |=
          CM_WKUP_I2C0_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_I2C0_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_I2C0_CLKCTRL) &
           CM_WKUP_I2C0_CLKCTRL_MODULEMODE));

    /* Verifying if the other bits are set to required settings. */

    /*
    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_AON_GCLK field in CM_L3_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_L4WKUP_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_L4WKUP_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_GCLK field in CM_WKUP_CLKSTCTRL register
    ** to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_AON_GCLK field in CM_L4_WKUP_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) &
           CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK));

    /*
    ** Waiting for CLKACTIVITY_I2C0_GFCLK field in CM_WKUP_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_I2C0_GFCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_I2C0_GFCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_I2C0_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_I2C0_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_I2C0_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_I2C0_CLKCTRL) &
           CM_WKUP_I2C0_CLKCTRL_IDLEST));
}






/**
 * @brief I2C初始化
 * @param [in] I2C的moduleId \b MODULE_ID_I2CX
 * @param [in] i2cClkFreq   I2C总线频率         
 * @param [in] slaveAddr 
 *        slaveAddr缓冲区，当I2C控制器作为从机时使用，不作为从机时时可为NULL
 * @param [in] szSlave  从机地址个数，当szSlave最大为4
 * @return    NONE       
 * @date    2013/5/29
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
void I2CInit(unsigned int moduleId,unsigned int i2cClkFreq,
             unsigned short *slaveAddr,
             unsigned int szSlave)
{
   moduleEnable(moduleId);
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   I2CMasterReset(baseAddr);
   /* Put i2c in reset/disabled state */
   I2CMasterDisable(baseAddr);
   /* Disable auto Idle functionality */
   I2CAutoIdleDisable(baseAddr);
   /* Configure i2c bus speed to 100khz */
   I2CMasterInitExpClk(baseAddr, 48000000, 12000000, i2cClkFreq);
   I2CMasterIntEnableEx(baseAddr, I2C_INT_TRANSMIT_READY);
   if (slaveAddr!=NULL) {
      unsigned int n = MIN(4,szSlave);
      for (int i = 0; i < n; i++) {
         I2COwnAddressSet(baseAddr, slaveAddr[i], i);
      }
   }
   
/* Bring I2C out of reset */
   I2CMasterEnable(baseAddr);
}


BOOL I2CMasterWrite(unsigned baseAddr,unsigned short addr,const void *buf,unsigned int szWrite){
   unsigned int datap = 0;
   I2CMasterIntClearEx(baseAddr, 0xffffffff);
   I2CModeSet(baseAddr, I2C_MODE_MASTER_SEND);
   I2CMasterSlaveAddrSet(baseAddr, addr);
   I2CFIFOClear(baseAddr,I2C_TX_MODE);
   if (szWrite<=32) {
      for (datap=0;datap<szWrite;datap++) {
         HWREG(baseAddr+I2C_DATA) = ((unsigned char *)buf)[datap];
      }
   } else {
      I2CFIFOThresholdConfig(baseAddr, 7, I2C_TX_MODE);
      for (datap = 0; datap < 32; datap++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)buf)[datap];
      }
   }
   I2CSetDataCount(baseAddr, szWrite);
   HWREG(baseAddr + I2C_CON) |= 0x03;
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 8)) { //ARDY
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         return SUCCESS;
      } else if (st & 1 << 4) { //transmit data ready
         for (int i = 0; (datap < szWrite) && (i < 16); i++, datap++) {
            HWREG(baseAddr + I2C_DATA) = ((unsigned char *)buf)[datap];
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 4; //clear
      }
   }
}

BOOL I2CMasterWriteEx(unsigned baseAddr,unsigned short slaveAddr,const void *w1buf,unsigned int szW1,
                      const void *w2buf,unsigned int szW2){
   unsigned int datap1=0,datap2=0;
   I2CMasterIntClearEx(baseAddr, 0xffffffff);
   I2CModeSet(baseAddr, I2C_MODE_MASTER_SEND);
   I2CMasterSlaveAddrSet(baseAddr, slaveAddr);
   I2CFIFOClear(baseAddr,I2C_TX_MODE);
   if (szW1<=32) {
      for (datap1=0;datap1<szW1;datap1++) {
         HWREG(baseAddr+I2C_DATA) = ((unsigned char *)w1buf)[datap1];
      }
   } else {
      I2CFIFOThresholdConfig(baseAddr, 7, I2C_TX_MODE);
      for (datap1=0; datap1 < 32; datap1++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w1buf)[datap1];
      }
   }
   I2CSetDataCount(baseAddr, szW1);
   HWREG(baseAddr + I2C_CON) |= 0x01; //STAR ,NOSTOP
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 12)) { //ARDY ,BB
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         goto NEXT;
      } else if (st & 1 << 4) { //transmit data ready
         for (int i = 0; (datap1 < szW1) && (i < 16); i++, datap1++) {
            HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w1buf)[datap1];
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 4; //clear
      }
   }
NEXT:
   if (szW2 <= 32) {
      for (datap2 = 0; datap2 < szW2; datap2++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w2buf)[datap2];
      }
   } else {
      I2CFIFOThresholdConfig(baseAddr, 7, I2C_TX_MODE);
      for (datap2 = 0; datap2 < 32; datap2++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w2buf)[datap2];
      }
   }
   I2CSetDataCount(baseAddr, szW2);
   HWREG(baseAddr + I2C_CON) |= 0x3; //STAR STOP
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 8)) { //ARDY
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         return SUCCESS;
      } else if (st & 1 << 4) { //transmit data ready
         for (int i = 0; (datap2 < szW2) && (i < 16); i++, datap2++) {
            HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w1buf)[datap2];
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 4; //clear
      }
   }
}



BOOL I2CMasterRead(unsigned int baseAddr,unsigned short slaveAddr,void *buf,unsigned int szRead)
{
   unsigned int datap = 0;
   I2CMasterIntClearEx(baseAddr, 0xffffffff);
   I2CModeSet(baseAddr, I2C_MODE_MASTER_RCV);
   I2CMasterSlaveAddrSet(baseAddr, slaveAddr);
   I2CFIFOClear(baseAddr,I2C_RX_MODE);
   if (szRead<=32) {
      I2CFIFOThresholdConfig(baseAddr, szRead, I2C_RX_MODE);
   } else {
      I2CFIFOThresholdConfig(baseAddr, 24, I2C_RX_MODE);
   }
   I2CSetDataCount(baseAddr, szRead);
   HWREG(baseAddr + I2C_CON) |= 0x03;
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 8)) { //ARDY
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         return SUCCESS;
      } else if (st & 1 << 3) { //Receive data ready
         for (int i = 0; (i < 24)&&(datap < szRead); i++, datap++) {
           ((unsigned char *)buf)[datap] =  HWREG(baseAddr + I2C_DATA);
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 3; //clear
      }
   }
}


BOOL I2CMasterReadEx(unsigned baseAddr,unsigned short slaveAddr,const void *writeBuf ,unsigned int szWrite,
                     void *readBuf,unsigned int szRead){
   unsigned int datap1=0,datap2 =0;
   I2CMasterIntClearEx(baseAddr, 0xffffffff);
   I2CModeSet(baseAddr, I2C_MODE_MASTER_SEND);
   I2CMasterSlaveAddrSet(baseAddr, slaveAddr);
   I2CFIFOClear(baseAddr,I2C_TX_MODE);
   if (szWrite<=32) {
      for (datap1=0;datap1<szWrite;datap1++) {
         HWREG(baseAddr+I2C_DATA) = ((unsigned char *)writeBuf)[datap1];
      }
   } else {
      I2CFIFOThresholdConfig(baseAddr, 7, I2C_TX_MODE);
      for (datap1=0; datap1 < 32; datap1++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)writeBuf)[datap1];
      }
   }
   I2CSetDataCount(baseAddr, szWrite);
   HWREG(baseAddr + I2C_CON) |= 0x01; //STAR ,NOSTOP
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 12)) { //ARDY ,BB
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         goto NEXT;
      } else if (st & 1 << 4) { //transmit data ready
         for (int i = 0; (datap1 < szWrite) && (i < 16); i++, datap1++) {
            HWREG(baseAddr + I2C_DATA) = ((unsigned char *)writeBuf)[datap1];
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 4; //clear
      }
   }
NEXT:
   I2CModeSet(baseAddr, I2C_MODE_MASTER_RCV);
   I2CFIFOClear(baseAddr,I2C_RX_MODE);
   if (szRead<=32) {
      I2CFIFOThresholdConfig(baseAddr, szRead, I2C_RX_MODE);
   } else {
      I2CFIFOThresholdConfig(baseAddr, 24, I2C_RX_MODE);
   }
   I2CSetDataCount(baseAddr, szRead);
   HWREG(baseAddr + I2C_CON) |= 0x03; //START STOP
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 8)) { //ARDY
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         return SUCCESS;
      } else if (st & 1 << 3) { //Receive data ready
         for (int i = 0; (i < 24)&&(datap2 < szRead); i++, datap2++) {
           ((unsigned char *)readBuf)[datap2] =  HWREG(baseAddr + I2C_DATA);
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 3; //clear
      }
   }

}
