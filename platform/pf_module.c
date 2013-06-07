
#include "clock.h"
#include "module.h"
#include "interrupt.h"



void moduleEnable(unsigned int moduleId){
    MODULE  *module = &modulelist[moduleId];
    ModuleClock *moduleclk = module->moduleClk;
    enableModuleClock(moduleclk);
}


void moduleIntConfigure(unsigned int moduleID){
   MODULE *module = modulelist+moduleID;
   for (int i=0;i<sizeof(module->INTNum)/sizeof(module->INTNum[0]);i++) {
      if (module->INTNum[i]==0) {
         break;
      }
      if(module->isr[i]==NULL){
         continue;
      }
      IntRegister(module->INTNum[i],module->isr[i]);
      IntPrioritySet(module->INTNum[i],module->INTPriority[i],AINTC_HOSTINT_ROUTE_IRQ);
      IntSystemEnable(module->INTNum[i]);
   } 
}
