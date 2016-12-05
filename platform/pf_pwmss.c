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
static bool pwmoutputaorb[3];

void pwmSetFreqDuty(unsigned int moduleId,unsigned int freq,unsigned int duty);

void pwmInitForSimplePwm(unsigned int moduleId,unsigned int pwmFreq,unsigned int duty,bool OutputAorB){
   ASSERT(duty<=100);
   unsigned int addr = modulelist[moduleId].baseAddr;
   unsigned int inputfreq = modulelist[moduleId].moduleClk->iClk[0]->clockSpeedHz;
   freqafterprescale = 25000000 ;
   unsigned int index= modulelist[moduleId].index;
   pwmoutputaorb[index] = OutputAorB;
   PWMSSTBClkDisable(index);
   EHRPWMTimebaseClkConfig(addr,freqafterprescale,inputfreq);//25mhz after prescaled
   /* Configure the period of the output waveform */
   /*EHRPWMPWMOpFreqSet(addr,freqafterprescale,(unsigned int)pwmFreq,
                       (unsigned int)EHRPWM_COUNT_UP,
                       (bool)EHRPWM_SHADOW_WRITE_ENABLE);*/
     /* Disable synchronization*/
    EHRPWMTimebaseSyncDisable(addr);
    /* Disable syncout*/
    EHRPWMSyncOutModeSet(addr, EHRPWM_SYNCOUT_DISABLE);
     /* Configure the emulation behaviour*/
    EHRPWMTBEmulationModeSet(addr, EHRPWM_STOP_AFTER_NEXT_TB_INCREMENT);
    pwmSetFreqDuty(moduleId,pwmFreq,duty);
    /*select ET int source*/
    EHRPWMETIntSourceSelect(addr, EHRPWM_ETSEL_INTSEL_TBCTREQUCMPAINC);

    /* Bypass dead band sub-module */
    EHRPWMDBOutput(addr, EHRPWM_DBCTL_OUT_MODE_BYPASS);

    /* Disable Chopper sub-module */
    EHRPWMChopperDisable(addr);

    /* Disable trip events */
    EHRPWMTZTripEventDisable(addr,(bool)EHRPWM_TZ_ONESHOT);
    EHRPWMTZTripEventDisable(addr,(bool)EHRPWM_TZ_CYCLEBYCYCLE);

    /* Disable High resolution capability */
    EHRPWMHRDisable(addr);
    PWMSSTBClkEnable(index);
    moduleIntConfigure(moduleId);
}


void pwmOnePlusIntCtr(unsigned int moduleId,bool enable){
    unsigned int baseAddr = modulelist[moduleId].baseAddr;
    if(enable){
        EHRPWMETIntEnable(baseAddr);
    }else{
        EHRPWMETIntDisable(baseAddr);
    }
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
    bool OutPutAorB = pwmoutputaorb[modulelist[moduleId].index];
    if(duty==0){
        if(OutPutAorB){
        EHRPWMConfigureAQActionOnA(addr,
                               EHRPWM_AQCTLB_ZRO_DONOTHING,
                               EHRPWM_AQCTLA_PRD_DONOTHING,
                               EHRPWM_AQCTLA_CAU_EPWMXALOW,
                               EHRPWM_AQCTLB_CAD_DONOTHING,
                               EHRPWM_AQCTLB_CBU_DONOTHING,
                               EHRPWM_AQCTLB_CBD_DONOTHING,
                               EHRPWM_AQSFRC_ACTSFB_DONOTHING);
    }else{
        EHRPWMConfigureAQActionOnB(addr,
                               EHRPWM_AQCTLB_ZRO_DONOTHING,
                               EHRPWM_AQCTLB_PRD_DONOTHING,
                               EHRPWM_AQCTLB_CAU_EPWMXBLOW,
                               EHRPWM_AQCTLB_CAD_DONOTHING,
                               EHRPWM_AQCTLB_CBU_DONOTHING,
                               EHRPWM_AQCTLB_CBD_DONOTHING,
                               EHRPWM_AQSFRC_ACTSFB_DONOTHING);
        }
    }else{
        if(OutPutAorB){
        EHRPWMConfigureAQActionOnA(addr,
                               EHRPWM_AQCTLB_ZRO_DONOTHING,
                               EHRPWM_AQCTLA_PRD_EPWMXAHIGH,
                               EHRPWM_AQCTLA_CAU_EPWMXALOW,
                               EHRPWM_AQCTLB_CAD_DONOTHING,
                               EHRPWM_AQCTLB_CBU_DONOTHING,
                               EHRPWM_AQCTLB_CBD_DONOTHING,
                               EHRPWM_AQSFRC_ACTSFB_DONOTHING);
    }else{
       EHRPWMConfigureAQActionOnB(addr,
                               EHRPWM_AQCTLB_ZRO_DONOTHING,
                               EHRPWM_AQCTLB_PRD_EPWMXBHIGH,
                               EHRPWM_AQCTLB_CAU_EPWMXBLOW,
                               EHRPWM_AQCTLB_CAD_DONOTHING,
                               EHRPWM_AQCTLB_CBU_DONOTHING,
                               EHRPWM_AQCTLB_CBD_DONOTHING,
                               EHRPWM_AQSFRC_ACTSFB_DONOTHING);
    }
    }
}


static void (*pwmssinthandler)(unsigned int moduleid,unsigned int intSource);


void pwmRegistIntHandler(void (*handler)(unsigned int moduleid,unsigned int intSource)){
    pwmssinthandler = handler;
}


#define PWMSS_INT_SOURCE_PRD    0
#define PWMSS_INT_SOURCE_CMP    1

void isr_pwmss(unsigned int intnum){
    unsigned int baseAddr= modulelist[intnum].baseAddr;
    if (pwmssinthandler!=NULL) {
        pwmssinthandler(intnum,PWMSS_INT_SOURCE_PRD);
    }
}




