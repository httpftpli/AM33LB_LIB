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
#include "debug.h"
#include "stdbool.h"
#include "hw_ehrpwm.h"




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
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) |= CONTROL_PWMSS_CTRL_PWMSS0_TBCLKEN;
               break;

         case 1:
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) |= CONTROL_PWMSS_CTRL_PWMMS1_TBCLKEN;
               break;

         case 2:
               HWREG(SOC_CONTROL_REGS +  CONTROL_PWMSS_CTRL) |= CONTROL_PWMSS_CTRL_PWMSS2_TBCLKEN;
               break;

         default:
         break;
    }
}


void PWMSSTBClkDisable(unsigned int instance)
{
    switch(instance)
    {

         case 0:
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) &= CONTROL_PWMSS_CTRL_PWMSS0_TBCLKEN;
               break;

         case 1:
               HWREG(SOC_CONTROL_REGS + CONTROL_PWMSS_CTRL) &= CONTROL_PWMSS_CTRL_PWMMS1_TBCLKEN;
               break;

         case 2:
               HWREG(SOC_CONTROL_REGS +  CONTROL_PWMSS_CTRL) &= CONTROL_PWMSS_CTRL_PWMSS2_TBCLKEN;
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

static unsigned int freqafterprescale;

void pwmInitForSimplePwm(unsigned int moduleId,unsigned int pwmFreq,unsigned int duty,bool outputAorB){
   ASSERT(duty<=100);
   unsigned int addr = modulelist[moduleId].baseAddr;
   unsigned int inputfreq = modulelist[moduleId].moduleClk->iClk[0]->clockSpeedHz;
   freqafterprescale = 25000000 ;
   unsigned int prd = 25000000/pwmFreq;
   unsigned int index= modulelist[moduleId].index;
   PWMSSTBClkDisable(index);
   EHRPWMTimebaseClkConfig(addr,freqafterprescale,inputfreq);//25mhz after prescaled
   /* Configure the period of the output waveform */
   EHRPWMPWMOpFreqSet(addr,freqafterprescale,(unsigned int)pwmFreq,
                       (unsigned int)EHRPWM_COUNT_UP,
                       (bool)EHRPWM_SHADOW_WRITE_ENABLE);
     /* Disable synchronization*/
    EHRPWMTimebaseSyncDisable(addr);
    /* Disable syncout*/
    EHRPWMSyncOutModeSet(addr, EHRPWM_SYNCOUT_DISABLE);
     /* Configure the emulation behaviour*/
    EHRPWMTBEmulationModeSet(addr, EHRPWM_STOP_AFTER_NEXT_TB_INCREMENT);

    /* Configure Counter compare cub-module */
    /* Load Compare A value */
    unsigned int cmpa = prd * duty / 100;
    EHRPWMLoadCMPA(addr,cmpa,(bool)EHRPWM_SHADOW_WRITE_ENABLE,
                   (unsigned int)EHRPWM_COMPA_LOAD_COUNT_EQUAL_PERIOD,
                   (bool)EHRPWM_CMPCTL_OVERWR_SH_FL);

    /* Configure Action qualifier */
    //do nothing
    if (outputAorB == true) {
        EHRPWMConfigureAQActionOnA(addr,0,0,0,0,0,0,0);
    } else {
        EHRPWMConfigureAQActionOnB(addr,0,0,0,0,0,0,0);
    }

    /* Bypass dead band sub-module */
    EHRPWMDBOutput(SOC_EPWM_2_REGS, EHRPWM_DBCTL_OUT_MODE_BYPASS);

    /* Disable Chopper sub-module */
    EHRPWMChopperDisable(SOC_EPWM_2_REGS);

    /* Disable trip events */
    EHRPWMTZTripEventDisable(SOC_EPWM_2_REGS,(bool)EHRPWM_TZ_ONESHOT);
    EHRPWMTZTripEventDisable(SOC_EPWM_2_REGS,(bool)EHRPWM_TZ_CYCLEBYCYCLE);

    /* Disable High resolution capability */
    EHRPWMHRDisable(SOC_EPWM_2_REGS);
    PWMSSTBClkEnable(index);
}


void pwmStart(unsigned int moduleId){
    unsigned int addr= modulelist[moduleId].baseAddr;
    EHRPWMConfigureAQActionOnA(addr,
                               EHRPWM_AQCTLB_ZRO_DONOTHING,
                               EHRPWM_AQCTLB_PRD_EPWMXBTOGGLE,
                               EHRPWM_AQCTLB_CAU_EPWMXBTOGGLE,
                               EHRPWM_AQCTLB_CAD_DONOTHING,
                               EHRPWM_AQCTLB_CBU_DONOTHING,
                               EHRPWM_AQCTLB_CBD_DONOTHING,
                               EHRPWM_AQSFRC_ACTSFB_DONOTHING);
}

void pwmStop(unsigned int moduleId) {
    unsigned int addr= modulelist[moduleId].baseAddr;
    EHRPWMConfigureAQActionOnA(addr,0,0,0,0,0,0,0);
}


void pwmSetFreqDuty(unsigned int moduleId,unsigned int freq,unsigned int duty){
    unsigned int addr= modulelist[moduleId].baseAddr;
    unsigned int prd = 25000000/freq;
    unsigned int cmpa = prd * duty / 100;
    EHRPWMPWMOpFreqSet(addr,freqafterprescale,(unsigned int)freq,
                       (unsigned int)EHRPWM_COUNT_UP,
                       (bool)EHRPWM_SHADOW_WRITE_ENABLE);
    EHRPWMLoadCMPA(addr,cmpa,(bool)EHRPWM_SHADOW_WRITE_ENABLE,
                   (unsigned int)EHRPWM_COMPA_LOAD_COUNT_EQUAL_PERIOD,
                   (bool)EHRPWM_CMPCTL_OVERWR_SH_FL);
}






