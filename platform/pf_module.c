
#include "clock.h"
#include "module.h"
#include "interrupt.h"
#include "debug.h"



void moduleEnable(unsigned int moduleId){
    MODULE  *module = &modulelist[moduleId];
    ModuleClock *moduleclk = module->moduleClk;
    enableModuleClock(moduleclk);
}

void moduleDisable(unsigned int moduleId){
    MODULE  *module = &modulelist[moduleId];
    ModuleClock *moduleclk = module->moduleClk;
    disableModuleClock(moduleclk, false);
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

void moduleSetIsrHandler( unsigned int moduleId,unsigned int handlerIndex,void (*isr)(unsigned int)){
   ASSERT(handlerIndex < 3);
   MODULE *module = modulelist+moduleId;
   ASSERT(module->INTNum[handlerIndex]!=0);
   module->isr[handlerIndex] = isr;
}

