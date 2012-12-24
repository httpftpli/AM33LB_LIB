
#include "interruptconfig.h"
#include "interrupt.h"
#include "platform.h"


extern void isr_Edma3CCError(unsigned int intnum);
extern void isr_Edma3Completion(unsigned int intnum); 
extern void isr_HSMMCSD(unsigned int intnum);
extern void isr_RTC(unsigned int intnum);
extern void isr_DCANLine0(unsigned intnum);


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


void perAINTCConfigure(){
   EDMAAINTCConfigure();
   MMCSDAINTCConfigure();
   RTCAINTCConfigure();
   DCANINTConfigure();
}




