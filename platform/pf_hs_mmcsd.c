/**
 * \file   hs_mmcsd.c
 *
 * \brief  This file contains functions which configure the HS/MMCSD controller
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
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "hw_cm_dpll.h"
#include "hs_mmcsd.h"
#include "pf_mux.h"
#include "platform.h"
#include "interrupt.h"



volatile G_MMCSD g_Mmcsd[3];

void isr_HSMMCSD(unsigned int intnum) {
   unsigned int status = 0;
   unsigned int index;
   unsigned int basemem;

   if (SYS_INT_MMCSD0INT == intnum) {
       index = 0;
       basemem = SOC_MMCHS_0_REGS;
   }else if(SYS_INT_MMCSD1INT == intnum){
       index = 1;
       basemem = SOC_MMCHS_1_REGS;
   }else{
       index = 2;
       basemem = SOC_MMCHS_2_REGS;
   }

   status = HSMMCSDIntrStatusGet(basemem, 0xFFFFFFFF);
   HSMMCSDIntrStatusClear(basemem, status);

   if (status & HS_MMCSD_STAT_CMDCOMP) {
      g_Mmcsd[index].fgCmdComp = 1;
   }

   if (status & HS_MMCSD_STAT_TRNFCOMP) {
      g_Mmcsd[index].fgXferComp = 1;
      static int xfercont =0;
      xfercont++;
   }

   if (status & HS_MMCSD_STAT_ERR) {
      unsigned int error = status & 0xFFFF0000;
      g_Mmcsd[index].bmCurError = error;
      g_Mmcsd[index].bmHisError |= error;

      if (status & HS_MMCSD_STAT_CMDTIMEOUT) {
         g_Mmcsd[index].fgCmdTimeout = 1;
      }

      if (status & HS_MMCSD_STAT_DATATIMEOUT) {
         g_Mmcsd[index].fgDataTimeout = 1;
      }
   }
}



/**
 * \brief   This function will configure the required clocks for HS MMC/SD instance.
 *
 * \return  None.
 *
 */
void HSMMCSDModuleClkConfig(void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_MMC0_CLKCTRL) |= 
                             CM_PER_MMC0_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_MMC0_CLKCTRL) & 
      CM_PER_MMC0_CLKCTRL_MODULEMODE) != CM_PER_MMC0_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_MMC1_CLKCTRL) |= 
                             CM_PER_MMC1_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_MMC1_CLKCTRL) & 
      CM_PER_MMC1_CLKCTRL_MODULEMODE) != CM_PER_MMC1_CLKCTRL_MODULEMODE_ENABLE);

}

