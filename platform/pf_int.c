#include "pf_platform_cfg.h"
#include "interrupt.h"
#include "platform.h"
#include "module.h"





extern void USB1HostIntHandler(unsigned int intnum);
extern void USB0HostIntHandler(unsigned int intnum);




void USBIntConfigure(int instatance) {
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
