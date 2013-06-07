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

/**
 * \brief   This function configures the L3 and L4_PER system clocks.
 *          It also configures the system clocks for the specified ePWMSS
 *          instance.
 *
 * \param   instanceNum    The instance number of ePWMSS whose system clocks
 *                         have to be configured.
 *
 * 'instanceNum' can take one of the following values:
 * (0 <= instanceNum <= 2)
 *
 * \return  None.
 *
 */
void PWMSSModuleClkConfig(unsigned int instanceNum)
{
   HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) |=
      CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   while ((HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) &
              CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

   HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) |=
      CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   while ((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) &
              CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

   HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
      CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

   while ((HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) &
              CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

   HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) |=
      CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

   while ((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) &
              CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

   HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
      CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   while ((HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

   HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) |=
      CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

   while ((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) &
              CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

   HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) |=
      CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

   while ((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) &
              CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

   if (0 == instanceNum) {
      HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS0_CLKCTRL) |=
         CM_PER_EPWMSS0_CLKCTRL_MODULEMODE_ENABLE;

      while (CM_PER_EPWMSS0_CLKCTRL_MODULEMODE_ENABLE !=
                (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS0_CLKCTRL) &
                    CM_PER_EPWMSS0_CLKCTRL_MODULEMODE));

      while ((CM_PER_EPWMSS0_CLKCTRL_IDLEST_FUNC <<
              CM_PER_EPWMSS0_CLKCTRL_IDLEST_SHIFT) !=
                (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS0_CLKCTRL) &
                    CM_PER_EPWMSS0_CLKCTRL_IDLEST));

   } else if (1 == instanceNum) {
      HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS1_CLKCTRL) |=
         CM_PER_EPWMSS1_CLKCTRL_MODULEMODE_ENABLE;

      while (CM_PER_EPWMSS1_CLKCTRL_MODULEMODE_ENABLE !=
                (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS1_CLKCTRL) &
                    CM_PER_EPWMSS1_CLKCTRL_MODULEMODE));

      while ((CM_PER_EPWMSS1_CLKCTRL_IDLEST_FUNC <<
              CM_PER_EPWMSS1_CLKCTRL_IDLEST_SHIFT) !=
                (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS1_CLKCTRL) &
                    CM_PER_EPWMSS1_CLKCTRL_IDLEST));

   } else if (2 == instanceNum) {
      HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS2_CLKCTRL) |=
         CM_PER_EPWMSS2_CLKCTRL_MODULEMODE_ENABLE;

      while (CM_PER_EPWMSS2_CLKCTRL_MODULEMODE_ENABLE !=
                (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS2_CLKCTRL) &
                    CM_PER_EPWMSS2_CLKCTRL_MODULEMODE));

      while ((CM_PER_EPWMSS2_CLKCTRL_IDLEST_FUNC <<
              CM_PER_EPWMSS2_CLKCTRL_IDLEST_SHIFT) !=
                (HWREG(SOC_PRCM_REGS + CM_PER_EPWMSS2_CLKCTRL) &
                    CM_PER_EPWMSS2_CLKCTRL_IDLEST));
   } else {

   }

   while (!(HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) &
               CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

   while (!(HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) &
               CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

   while (!(HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
               (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK |
                   CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK)));

   while (!(HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) &
               (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK)));

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
