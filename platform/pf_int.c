
#include "pf_platform_cfg.h"
#include "interrupt.h"
#include "platform.h"



extern void isr_Edma3CCError(unsigned int intnum);
extern void isr_Edma3Completion(unsigned int intnum); 
extern void isr_HSMMCSD(unsigned int intnum);
extern void isr_RTC(unsigned int intnum);
extern void isr_DCANLine0(unsigned int intnum);
extern void isr_DTimer2(unsigned int intnum);
extern void isr_qep(unsigned int  intnum);
extern void USB1HostIntHandler(unsigned int intnum);
extern void USB0HostIntHandler(unsigned int intnum);
extern void isr_Gpio(unsigned int intnum);
extern void isr_lcd(unsigned int num);



/*
** This function configures the AINTC to receive EDMA3 interrupts.
*/
static void EDMAAINTCConfigure() {
   /* Registering EDMA3 Channel Controller transfer completion interrupt.  */
   IntRegister(SYS_INT_EDMACOMPINT, isr_Edma3Completion);

   /* Setting the priority for EDMA3CC completion interrupt in AINTC. */
   IntPrioritySet(SYS_INT_EDMACOMPINT,INT_PRIORITY_DMA_COMP , AINTC_HOSTINT_ROUTE_IRQ);
   
  
   /* Registering EDMA3 Channel Controller Error Interrupt. */
   IntRegister(SYS_INT_EDMAERRINT,isr_Edma3CCError);

   /* Setting the priority for EDMA3CC Error interrupt in AINTC. */
   IntPrioritySet(SYS_INT_EDMAERRINT, INT_PRIORITY_DMA_ERROR, AINTC_HOSTINT_ROUTE_IRQ);

   /* Enabling the EDMA3CC completion interrupt in AINTC. */
   IntSystemEnable(SYS_INT_EDMACOMPINT);

   /* Enabling the EDMA3CC Error interrupt in AINTC. */
   IntSystemEnable(SYS_INT_EDMAERRINT);
}

static void MMCSDAINTCConfigure(){
   IntRegister(SYS_INT_MMCSD0INT, isr_HSMMCSD);
   IntPrioritySet(SYS_INT_MMCSD0INT,INT_PRIORITY_MMCSD0 , AINTC_HOSTINT_ROUTE_IRQ);  
   IntSystemEnable(SYS_INT_MMCSD0INT);
   
   IntRegister(SYS_INT_MMCSD1INT, isr_HSMMCSD);
   IntPrioritySet(SYS_INT_MMCSD1INT, INT_PRIORITY_MMCSD1, AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_MMCSD1INT);
}

static void RTCAINTCConfigure(void)
{   
    IntRegister(SYS_INT_RTCINT, isr_RTC); 
    IntPrioritySet(SYS_INT_RTCINT, INT_PRIORITY_RTC, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(SYS_INT_RTCINT);    
}


static void DCANINTConfigure(void){
   IntRegister(SYS_INT_DCAN0_INT0,isr_DCANLine0);
   IntPrioritySet(SYS_INT_DCAN0_INT0,INT_PRIORITY_DCAN0_LINE0,AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_DCAN0_INT0);  
}

static void DMTimerIntConfigure(void){
   IntRegister(SYS_INT_TINT2,isr_DTimer2);
   IntPrioritySet(SYS_INT_TINT2,INT_PRIORITY_TIMER2,AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_TINT2);
    
}

static void QEP2IntConfigure(void){
   IntRegister(SYS_INT_eQEP2INT,isr_qep);
   IntPrioritySet(SYS_INT_eQEP2INT,INT_PRIORITY_EQEP2,AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_eQEP2INT);  
}

static void LCDIntConfigure(void) {
   IntRegister(SYS_INT_LCDCINT, isr_lcd);
   IntPrioritySet(SYS_INT_LCDCINT, INT_PRIORITY_LCD, AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_LCDCINT);
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


static void GPIOIntConfigure(void){
   IntRegister(SYS_INT_GPIOINT0A,isr_Gpio);
   IntPrioritySet(SYS_INT_GPIOINT0A,INT_PRIORITY_GPIO0,AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_GPIOINT0A);

   IntRegister(SYS_INT_GPIOINT1A,isr_Gpio);
   IntPrioritySet(SYS_INT_GPIOINT1A,INT_PRIORITY_GPIO1,AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_GPIOINT1A);

   IntRegister(SYS_INT_GPIOINT2A,isr_Gpio);
   IntPrioritySet(SYS_INT_GPIOINT2A,INT_PRIORITY_GPIO2,AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_GPIOINT2A);

   IntRegister(SYS_INT_GPIOINT3A,isr_Gpio);
   IntPrioritySet(SYS_INT_GPIOINT3A,INT_PRIORITY_GPIO3,AINTC_HOSTINT_ROUTE_IRQ);
   IntSystemEnable(SYS_INT_GPIOINT3A);
}



void perAINTCConfigure(){
   EDMAAINTCConfigure();
   MMCSDAINTCConfigure();
   //RTCAINTCConfigure();
   DCANINTConfigure();
   DMTimerIntConfigure();
   QEP2IntConfigure();
   USBIntConfigure(1);
   GPIOIntConfigure();
   LCDIntConfigure();
}




