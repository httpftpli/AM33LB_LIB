#include "soc_am335x.h"
#include "ecap.h"
#include "hw_types.h"



#define NUMBER_OF_ECAP   3


typedef void (*ECAPHANDLERS)(unsigned int val) ;

ECAPHANDLERS   ecaphandlers[NUMBER_OF_ECAP] = {NULL,NULL,NULL};

void ECAPregisterHandler(unsigned int ecapindex,void (*ecaphandler)(unsigned int time)){
   mdAssert(ecapindex<NUMBER_OF_ECAP);
   ecaphandlers[ecapindex] = ecaphandler;
}

void isr_ecap(unsigned int intnum){
   unsigned short val  = HWREGH(SOC_ECAP_0_REGS + ECAP_ECFLG);
   HWREGH(SOC_ECAP_0_REGS + ECAP_ECCLR) = val;
   unsigned int val1;
   if (val & (1 << 2)) {
      val1 = ECAPTimeStampRead(SOC_ECAP_0_REGS, ECAP_CAPTURE_EVENT_2);
      if (ecaphandlers[0] != NULL) {
         ecaphandlers[0](val1);
      }
   }
   if (val & (1 << 4)) {
      val1 = ECAPTimeStampRead(SOC_ECAP_0_REGS, ECAP_CAPTURE_EVENT_2);
      if (ecaphandlers[0] != NULL) {
         ecaphandlers[0](val1);
      }
   }
}
   

void ECAPInit(unsigned int baseAddr) {
   ECAPClockEnable(SOC_PWMSS0_REGS);
   while(ECAPClockEnableStatusGet(SOC_PWMSS0_REGS)==0); //waite for ecap clk enable
   ECAPOperatingModeSelect(baseAddr, ECAP_CAPTURE_MODE);
   ECAPContinousModeConfig(baseAddr);

   ECAPIntStatusClear(baseAddr,0xffffffff);

   ECAPPrescaleConfig(baseAddr, 0);
   ECAPCaptureLoadingEnable(baseAddr);
   ECAPCapeEvtPolarityConfig(baseAddr, 1, 0, 1, 0); //1 falling edge ;0 rising edge
   ECAPCaptureEvtCntrRstConfig(baseAddr, 1, 0, 1, 0);

   ECAPIntEnable(baseAddr,   
                    ECAP_CEVT2_INT |   ECAP_CEVT4_INT);
   ECAPGlobalIntEnable(baseAddr);

   ECAPCounterControl(baseAddr, ECAP_COUNTER_FREE_RUNNING);
}







