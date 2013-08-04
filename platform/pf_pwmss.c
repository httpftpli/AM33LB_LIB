/** 
 * \file   pf_pwmss.c 
 *
 * \brief  This file contains functions which does platform specific
 *         configurations for PWMSS.
 */



#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "ehrpwm.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "module.h"




/**
 * \brief   This function Enables TBCLK(Time Base Clock) for specific
 *          EPWM instance of pwmsubsystem.
 *
 * \param   instance  It is the instance number of EPWM of pwmsubsystem.
 *
 **/
void PWMSSTBClkEnable(unsigned int instance)
{
    switch(instance)
    {
    
         case 0:
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) = CONTROL_PWMSS_CTRL_PWMSS0_TBCLKEN;
               break;
 
         case 1:
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) = CONTROL_PWMSS_CTRL_PWMMS1_TBCLKEN;
               break;
  
         case 2:
               HWREG(SOC_CONTROL_REGS +  CONTROL_PWMSS_CTRL) = CONTROL_PWMSS_CTRL_PWMSS2_TBCLKEN;
               break;

         default:
         break;
    } 
}


void PWMSSInit(void){
   moduleEnable(MODULE_ID_eCAP0);
   moduleEnable(MODULE_ID_eCAP1);
   moduleEnable(MODULE_ID_eCAP2);
   unsigned int val = HWREG(SOC_PWMSS0_REGS + 4);
   val &= ~(0x03<2);
   val |= 0x01<<2;
   HWREG(SOC_PWMSS0_REGS + 4) = val;
   HWREG(SOC_PWMSS1_REGS + 4) = val;
   HWREG(SOC_PWMSS2_REGS + 4) = val;
}
