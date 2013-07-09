/**
 * \file   hs_mmcsd.c
 *
 * \brief  This file contains functions which configure the HS/MMCSD controller
 */



#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "hw_cm_dpll.h"
#include "hs_mmcsd.h"
#include "pf_mux.h"
#include "interrupt.h"
#include "module.h"
#include "pf_hs_mmcsd.h"



volatile G_MMCSD g_Mmcsd[3];

void isr_HSMMCSD(unsigned int intnum) {
   unsigned int status = 0;
   unsigned int index = modulelist[intnum].index;
   unsigned int basemem = modulelist[intnum].baseAddr;
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

