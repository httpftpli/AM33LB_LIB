#include "pf_platform_cfg.h"
#include "interrupt.h"
#include "platform.h"
#include "module.h"




extern void isr_DTimer2(unsigned int intnum);
extern void USB1HostIntHandler(unsigned int intnum);
extern void USB0HostIntHandler(unsigned int intnum);



static void DMTimerIntConfigure(void) {
   IntRegister(SYS_INT_TINT2, isr_DTimer2);
   IntPrioritySet(SYS_INT_TINT2, INT_PRIORITY_TIMER2, AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_TINT2);
}



static void USBIntConfigure(int instatance) {
   if (instatance) {
      IntRegister(SYS_INT_USB1, USB1HostIntHandler);
      IntPrioritySet(SYS_INT_USB1, INT_PRIORITY_USB1, AINTC_HOSTINT_ROUTE_IRQ);
      IntSystemEnable(SYS_INT_USB1);
   } else {
      IntRegister(SYS_INT_USB0, USB0HostIntHandler);
      IntPrioritySet(SYS_INT_USB0, INT_PRIORITY_USB0, AINTC_HOSTINT_ROUTE_IRQ);
      IntSystemEnable(SYS_INT_USB0);
   }
#if USB_USE_CPPI41DMA
   IntRegister(SYS_INT_USBSSINT, USB1HostIntHandler);
   IntPrioritySet(SYS_INT_USBSSINT, INT_PRIORITY_USB_DMA, AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_USBSSINT);
#endif
}




void moduleIntConfigure(unsigned int moduleID){
   MODULE *module = modulelist+moduleID;
   for (int i=0;i<sizeof(module->INTNum)/sizeof(module->INTNum[0]);i++) {
      if (module->INTNum[i]==0) {
         break;
      }
      IntRegister(module->INTNum[i],module->isr[i]);
      IntPrioritySet(module->INTNum[i],module->INTPriority[i],AINTC_HOSTINT_ROUTE_IRQ);
      IntSystemEnable(module->INTNum[i]);
   } 
}


void perAINTCConfigure() {
   DMTimerIntConfigure();
   USBIntConfigure(1);
   for (int i=0;i<128;i++) {
      MODULE *module = modulelist+i;
      if (module->baseAddr==0) {
         continue;
      }
      moduleIntConfigure(i);
   }
}