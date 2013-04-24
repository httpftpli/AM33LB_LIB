

#include "module.h"
#include "clock.h"
#include "soc_AM335X.h"
#include "pf_platform_cfg.h"




extern ModuleClock  mpuModClock;
extern ModuleClock  emifModClock;
extern ModuleClock  timer2ModClock;
extern ModuleClock  i2c1ModClock;
extern ModuleClock 	gpio3ModClock;
extern ModuleClock	spi0ModClock;
extern ModuleClock	uart0ModClock;
extern ModuleClock	mailbox0ModClock;
extern ModuleClock	rtcModClock;
extern ModuleClock	l4lsModClock;
extern ModuleClock	wdt1ModClock;
extern ModuleClock	adcTscModClock;
extern ModuleClock	lcdcModClock;
extern ModuleClock	wkupM3ModClock;
extern ModuleClock	l4WkupModClock;
extern ModuleClock	mcasp1ModClock;
extern ModuleClock	ocmcramModClock;
extern ModuleClock	l3ModClock;
extern ModuleClock	gpmcModClock;
extern ModuleClock	usbModClock;
extern ModuleClock	l4fwModClock;
extern ModuleClock	cpgmac0ModClock;
extern ModuleClock	i2c0ModClock;
extern ModuleClock	gpio0ModClock;
extern ModuleClock	gpio1ModClock;
extern ModuleClock	timer1ModClock;
extern ModuleClock	timer3ModClock;
extern ModuleClock	timer4ModClock;
extern ModuleClock	timer7ModClock;
extern ModuleClock	epwmss0ModClock;
extern ModuleClock	tpccModClock;
extern ModuleClock	tptc0ModClock;
extern ModuleClock	tptc1ModClock;
extern ModuleClock	tptc2ModClock;



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
extern void isr_ecap(unsigned int num);


MODULE modulelist[128] = {
   [MODULE_ID_EDMA] =  {.index=0, .baseAddr = SOC_EDMA30CC_0_REGS,.INTNum={SYS_INT_EDMACOMPINT,0,0},  .INTPriority={INT_PRIORITY_DMA_COMP,0,0},.isr={isr_Edma3Completion,0,0}},
   [MODULE_ID_EDMA_ERR] =  {.index=0, .baseAddr = SOC_EDMA30CC_0_REGS,.INTNum={SYS_INT_EDMAERRINT,0,0},  .INTPriority={INT_PRIORITY_DMA_ERROR,0,0},.isr={isr_Edma3CCError,0,0}},
   [MODULE_ID_eCAP0] = {.index=0, .baseAddr = SOC_ECAP_0_REGS,.INTNum={SYS_INT_eCAP0INT,0,0},  .INTPriority={INT_PRIORITY_eCAP0,0,0},.isr={isr_ecap,0,0}},
   [MODULE_ID_eCAP2] = {.index=2, .baseAddr = SOC_ECAP_2_REGS,.INTNum={SYS_INT_eCAP2INT,0,0},  .INTPriority={INT_PRIORITY_eCAP2,0,0},.isr={isr_ecap,0,0}},
   [MODULE_ID_GPIO0] = {.index=0, .baseAddr = SOC_GPIO_0_REGS,.INTNum={SYS_INT_GPIOINT0A,0,0}, .INTPriority={INT_PRIORITY_GPIO0,0,0},.isr={isr_Gpio,0,0}},
   [MODULE_ID_GPIO1] = {.index=1, .baseAddr = SOC_GPIO_1_REGS,.INTNum={SYS_INT_GPIOINT1A,0,0}, .INTPriority={INT_PRIORITY_GPIO1,0,0},.isr={isr_Gpio,0,0}},
   [MODULE_ID_GPIO2] = {.index=2, .baseAddr = SOC_GPIO_2_REGS,.INTNum={SYS_INT_GPIOINT2A,0,0}, .INTPriority={INT_PRIORITY_GPIO2,0,0},.isr={isr_Gpio,0,0}},
   [MODULE_ID_GPIO3] = {.index=3, .baseAddr = SOC_GPIO_3_REGS,.INTNum={SYS_INT_GPIOINT3A,0,0}, .INTPriority={INT_PRIORITY_GPIO3,0,0},.isr={isr_Gpio,0,0}},
   [MODULE_ID_LCDC]  = {.index=0, .baseAddr = SOC_LCDC_0_REGS,.INTNum={SYS_INT_LCDCINT,0,0}  , .INTPriority={INT_PRIORITY_LCD,0,0}  ,.isr={isr_lcd,0,0}},
   [MODULE_ID_eQEP2] = {.index=2, .baseAddr = SOC_EQEP_2_REGS,.INTNum={SYS_INT_eQEP2INT,0,0} , .INTPriority={INT_PRIORITY_EQEP2,0,0},.isr={isr_qep,0,0}},
   [MODULE_ID_DCAN0] = {.index=0, .baseAddr = SOC_DCAN_0_REGS,.INTNum={SYS_INT_DCAN0_INT0,0,0},.INTPriority={INT_PRIORITY_DCAN0_LINE0,0,0},.isr={isr_DCANLine0,0,0}},
   [MODULE_ID_DCAN1] = {.index=1, .baseAddr = SOC_DCAN_1_REGS,.INTNum={SYS_INT_DCAN1_INT0,0,0},.INTPriority={INT_PRIORITY_DCAN1_LINE0,0,0},.isr={isr_DCANLine0,0,0}},
   [MODULE_ID_MMCSD0] = {.index=0, .baseAddr = SOC_MMCHS_0_REGS,.INTNum={SYS_INT_MMCSD0INT,0,0},.INTPriority={INT_PRIORITY_MMCSD0,0,0},.isr={isr_HSMMCSD,0,0}},
   [MODULE_ID_MMCSD1] = {.index=1, .baseAddr = SOC_MMCHS_1_REGS,.INTNum={SYS_INT_MMCSD1INT,0,0},.INTPriority={INT_PRIORITY_MMCSD1,0,0},.isr={isr_HSMMCSD,0,0}}
};





