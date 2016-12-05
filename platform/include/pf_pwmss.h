#ifndef __PF__PWMSS__H__
#define __PF__PWMSS__H__


#define PWMSS_INT_SOURCE_PRD    0
#define PWMSS_INT_SOURCE_CMP    1

extern void PWMSSTBClkEnable(unsigned int instance);
extern void PWMSSTBClkDisable(unsigned int instance);
extern void PWMSSInit(void);
extern void pwmInitForSimplePwm(unsigned int moduleId,unsigned int pwmFreq,unsigned int duty,bool outputAorB);
extern void pwmOnePlusIntCtr(unsigned int moduleId,bool enable);
extern void pwmSetFreqDuty(unsigned int moduleId,unsigned int freq,unsigned int duty);
extern void pwmRegistIntHandler(void (*handler)(unsigned int moduleid,unsigned int intSource));



#endif
