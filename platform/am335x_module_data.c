

#include "module.h"
#include "clock.h"
#include "soc_AM335X.h"
#include "pf_platform_cfg.h"




extern ModuleClock  mpuModClock;
extern ModuleClock  emifModClock;
extern ModuleClock  timer2ModClock;
extern ModuleClock  i2c0ModClock;
extern ModuleClock  i2c1ModClock;
extern ModuleClock  i2c2ModClock;
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
extern ModuleClock	gpio2ModClock;
extern ModuleClock	gpio3ModClock;
extern ModuleClock	timer0ModClock;
extern ModuleClock	timer1ModClock;
extern ModuleClock	timer2ModClock;
extern ModuleClock	timer3ModClock;
extern ModuleClock	timer4ModClock;
extern ModuleClock	timer5ModClock;
extern ModuleClock	timer6ModClock;
extern ModuleClock	timer7ModClock;
extern ModuleClock	epwmss0ModClock;
extern ModuleClock	tpccModClock;
extern ModuleClock	tptc0ModClock;
extern ModuleClock	tptc1ModClock;
extern ModuleClock	tptc2ModClock;
extern ModuleClock      uart1ModClock;
extern ModuleClock      uart2ModClock;
extern ModuleClock      uart3ModClock;
extern ModuleClock      uart4ModClock;
extern ModuleClock      uart5ModClock;
extern ModuleClock      dcan0ModClock;
extern ModuleClock      dcan1ModClock;
extern ModuleClock      mmcsd0ModClock;
extern ModuleClock   epwmss0ModClock;
extern ModuleClock   epwmss1ModClock;
extern ModuleClock   epwmss2ModClock;
extern ModuleClock   spi0ModClock;
extern ModuleClock   spi1ModClock;



extern __weak  void isr_Edma3CCError(unsigned int intnum);
extern __weak  void isr_Edma3Completion(unsigned int intnum);
extern __weak  void isr_HSMMCSD(unsigned int intnum);
extern __weak  void isr_RTC(unsigned int intnum);
extern __weak  void isr_DCANLine0(unsigned int intnum);
extern __weak  void isr_qep(unsigned int  intnum);
extern __weak  void USB1HostIntHandler(unsigned int intnum);
extern __weak  void USB0HostIntHandler(unsigned int intnum);
extern __weak  void isr_Gpio(unsigned int intnum);
extern __weak  void isr_lcd(unsigned int num);
extern __weak  void isr_ecap(unsigned int num);
extern __weak  void isr_tsc(unsigned int intnum);
extern __weak  void isr_uart_for_keyboard(unsigned int intnum);
extern __weak  void isr_mcspi(unsigned int intnum);
extern __weak  void isr_dmtimer(unsigned int num);


#ifdef AM335X_ADC_TSC
#define IS_TSC isr_tsc
#else
#define IS_TSC isr_uart_for_keyboard
#endif



MODULE modulelist[128] = {
   [MODULE_ID_EDMA] =  {.index=0, .baseAddr = SOC_EDMA30CC_0_REGS,.INTNum={SYS_INT_EDMACOMPINT,0,0},  .INTPriority={INT_PRIORITY_DMA_COMP,0,0},.isr={isr_Edma3Completion,0,0}},
   [MODULE_ID_EDMA_ERR] =  {.index=0, .baseAddr = SOC_EDMA30CC_0_REGS,.INTNum={SYS_INT_EDMAERRINT,0,0},  .INTPriority={INT_PRIORITY_DMA_ERROR,0,0},.isr={isr_Edma3CCError,0,0}},
   [MODULE_ID_eCAP0] = {.index=0, .baseAddr = SOC_ECAP_0_REGS,.INTNum={SYS_INT_eCAP0INT,0,0},  .INTPriority={INT_PRIORITY_eCAP0,0,0},.isr={isr_ecap,0,0},.moduleClk=&epwmss0ModClock},
   [MODULE_ID_eCAP1] = {.index=1, .baseAddr = SOC_ECAP_1_REGS,.INTNum={SYS_INT_eCAP1INT,0,0},  .INTPriority={INT_PRIORITY_eCAP1,0,0},.isr={isr_ecap,0,0},.moduleClk=&epwmss1ModClock},
   [MODULE_ID_eCAP2] = {.index=2, .baseAddr = SOC_ECAP_2_REGS,.INTNum={SYS_INT_eCAP2INT,0,0},  .INTPriority={INT_PRIORITY_eCAP2,0,0},.isr={isr_ecap,0,0},.moduleClk=&epwmss2ModClock},
   [MODULE_ID_ePWM0] = {.index=0, .baseAddr = SOC_EPWM_0_REGS,.INTNum={SYS_INT_ePWM0INT,0,0},  .INTPriority={0,0,0},.isr={0,0,0},.moduleClk=&epwmss0ModClock},
   [MODULE_ID_ePWM1] = {.index=1, .baseAddr = SOC_EPWM_1_REGS,.INTNum={SYS_INT_ePWM0INT,0,0},  .INTPriority={0,0,0},.isr={0,0,0},.moduleClk=&epwmss1ModClock},
   [MODULE_ID_ePWM2] = {.index=2, .baseAddr = SOC_EPWM_2_REGS,.INTNum={SYS_INT_ePWM0INT,0,0},  .INTPriority={0,0,0},.isr={0,0,0},.moduleClk=&epwmss2ModClock},
   [MODULE_ID_GPIO0] = {.index=0, .baseAddr = SOC_GPIO_0_REGS,.INTNum={SYS_INT_GPIOINT0A,0,0}, .INTPriority={INT_PRIORITY_GPIO0,0,0},.isr={isr_Gpio,0,0},.moduleClk=&gpio0ModClock},
   [MODULE_ID_GPIO1] = {.index=1, .baseAddr = SOC_GPIO_1_REGS,.INTNum={SYS_INT_GPIOINT1A,0,0}, .INTPriority={INT_PRIORITY_GPIO1,0,0},.isr={isr_Gpio,0,0},.moduleClk=&gpio1ModClock},
   [MODULE_ID_GPIO2] = {.index=2, .baseAddr = SOC_GPIO_2_REGS,.INTNum={SYS_INT_GPIOINT2A,0,0}, .INTPriority={INT_PRIORITY_GPIO2,0,0},.isr={isr_Gpio,0,0},.moduleClk=&gpio2ModClock},
   [MODULE_ID_GPIO3] = {.index=3, .baseAddr = SOC_GPIO_3_REGS,.INTNum={SYS_INT_GPIOINT3A,0,0}, .INTPriority={INT_PRIORITY_GPIO3,0,0},.isr={isr_Gpio,0,0},.moduleClk=&gpio3ModClock},
   [MODULE_ID_LCDC]  = {.index=0, .baseAddr = SOC_LCDC_0_REGS,.INTNum={SYS_INT_LCDCINT,0,0}  , .INTPriority={INT_PRIORITY_LCD,0,0}  ,.isr={isr_lcd,0,0}, .moduleClk=&lcdcModClock},
   [MODULE_ID_eQEP0] = {.index=0, .baseAddr = SOC_EQEP_0_REGS,.INTNum={SYS_INT_eQEP0INT,0,0} , .INTPriority={INT_PRIORITY_EQEP0,0,0},.isr={isr_qep,0,0}, .moduleClk=&epwmss0ModClock},
   [MODULE_ID_eQEP1] = {.index=1, .baseAddr = SOC_EQEP_1_REGS,.INTNum={SYS_INT_eQEP1INT,0,0} , .INTPriority={INT_PRIORITY_EQEP0,0,0},.isr={isr_qep,0,0}, .moduleClk=&epwmss1ModClock},
   [MODULE_ID_eQEP2] = {.index=2, .baseAddr = SOC_EQEP_2_REGS,.INTNum={SYS_INT_eQEP2INT,0,0} , .INTPriority={INT_PRIORITY_EQEP2,0,0},.isr={isr_qep,0,0}, .moduleClk=&epwmss2ModClock},
   [MODULE_ID_DCAN0] = {.index=0, .baseAddr = SOC_DCAN_0_REGS,.INTNum={SYS_INT_DCAN0_INT0,0,0},.INTPriority={INT_PRIORITY_DCAN0_LINE0,0,0},.isr={isr_DCANLine0,0,0},.moduleClk=&dcan0ModClock},
   [MODULE_ID_DCAN1] = {.index=1, .baseAddr = SOC_DCAN_1_REGS,.INTNum={SYS_INT_DCAN1_INT0,0,0},.INTPriority={INT_PRIORITY_DCAN1_LINE0,0,0},.isr={isr_DCANLine0,0,0},.moduleClk=&dcan1ModClock},
   [MODULE_ID_MMCSD0] = {.index=0,.baseAddr = SOC_MMCHS_0_REGS,.INTNum={SYS_INT_MMCSD0INT,0,0},.INTPriority={INT_PRIORITY_MMCSD0,0,0},.isr={isr_HSMMCSD,0,0},.moduleClk=&mmcsd0ModClock},
   [MODULE_ID_MMCSD1] = {.index=1,.baseAddr = SOC_MMCHS_1_REGS,.INTNum={SYS_INT_MMCSD1INT,0,0},.INTPriority={INT_PRIORITY_MMCSD1,0,0},.isr={isr_HSMMCSD,0,0}},
   [MODULE_ID_ADCTSC] = {.index=0,.baseAddr = SOC_ADC_TSC_0_REGS,.INTNum={SYS_INT_ADC_TSC_GENINT,0,0},.INTPriority={INT_PRIORITY_ADCTSC,0,0},.isr={IS_TSC,0,0},.moduleClk=&adcTscModClock},
   [MODULE_ID_UART0] =  {.index=1,.baseAddr = SOC_UART_0_REGS,.INTNum={SYS_INT_UART0INT,0,0},.INTPriority={0,0,0},.isr={0,0,0},.moduleClk=&uart0ModClock},
   [MODULE_ID_UART1] =  {.index=1,.baseAddr = SOC_UART_1_REGS,.INTNum={SYS_INT_UART1INT,0,0},.INTPriority={INT_PRIORITY_UART1,0,0},.isr={0,0,0},.moduleClk=&uart1ModClock}, 
   [MODULE_ID_UART2] =  {.index=1,.baseAddr = SOC_UART_2_REGS,.INTNum={SYS_INT_UART2INT,0,0},.INTPriority={INT_PRIORITY_UART2,0,0},.isr={0,0,0},.moduleClk=&uart2ModClock},
   [MODULE_ID_UART3] =  {.index=1,.baseAddr = SOC_UART_3_REGS,.INTNum={SYS_INT_UART3INT,0,0},.INTPriority={INT_PRIORITY_UART3,0,0},.isr={0,0,0},.moduleClk=&uart3ModClock},
   [MODULE_ID_UART4] =  {.index=1,.baseAddr = SOC_UART_4_REGS,.INTNum={SYS_INT_UART4INT,0,0},.INTPriority={INT_PRIORITY_UART4,0,0},.isr={IS_TSC,0,0},.moduleClk=&uart4ModClock},
   [MODULE_ID_UART5] =  {.index=1,.baseAddr = SOC_UART_5_REGS,.INTNum={SYS_INT_UART5INT,0,0},.INTPriority={INT_PRIORITY_UART5,0,0},.isr={0,0,0},.moduleClk=&uart5ModClock},
   [MODULE_ID_TIMER0] = {.index=0,.baseAddr = SOC_DMTIMER_0_REGS,.INTNum={SYS_INT_TINT0,0,0},.INTPriority={INT_PRIORITY_TIMER0,0,0},.isr={0,0,0},.moduleClk=&timer0ModClock},
   [MODULE_ID_TIMER1] = {.index=1,.baseAddr = SOC_DMTIMER_1_REGS,.INTNum={SYS_INT_TINT1_1MS,0,0},.INTPriority={INT_PRIORITY_TIMER1,0,0},.isr={0,0,0},.moduleClk=&timer1ModClock},
   [MODULE_ID_TIMER2] = {.index=2,.baseAddr = SOC_DMTIMER_2_REGS,.INTNum={SYS_INT_TINT2,0,0},.INTPriority={INT_PRIORITY_TIMER2,0,0},.isr={isr_dmtimer,0,0},.moduleClk=&timer2ModClock},
   [MODULE_ID_TIMER3] = {.index=3,.baseAddr = SOC_DMTIMER_3_REGS,.INTNum={SYS_INT_TINT3,0,0},.INTPriority={INT_PRIORITY_TIMER3,0,0},.isr={isr_dmtimer,0,0},.moduleClk=&timer3ModClock},
   [MODULE_ID_TIMER4] = {.index=4,.baseAddr = SOC_DMTIMER_4_REGS,.INTNum={SYS_INT_TINT4,0,0},.INTPriority={INT_PRIORITY_TIMER4,0,0},.isr={isr_dmtimer,0,0},.moduleClk=&timer4ModClock},
   [MODULE_ID_TIMER5] = {.index=5,.baseAddr = SOC_DMTIMER_5_REGS,.INTNum={SYS_INT_TINT5,0,0},.INTPriority={INT_PRIORITY_TIMER5,0,0},.isr={isr_dmtimer,0,0},.moduleClk=&timer5ModClock},
   [MODULE_ID_TIMER6] = {.index=6,.baseAddr = SOC_DMTIMER_6_REGS,.INTNum={SYS_INT_TINT6,0,0},.INTPriority={INT_PRIORITY_TIMER6,0,0},.isr={isr_dmtimer,0,0},.moduleClk=&timer6ModClock},
   [MODULE_ID_TIMER7] = {.index=7,.baseAddr = SOC_DMTIMER_7_REGS,.INTNum={SYS_INT_TINT7,0,0},.INTPriority={INT_PRIORITY_TIMER7,0,0},.isr={isr_dmtimer,0,0},.moduleClk=&timer7ModClock},
   [MODULE_ID_GPMC]   = {.index=0,.baseAddr = SOC_GPMC_0_REGS,   .moduleClk=&gpmcModClock},
   [MODULE_ID_I2C0]   = {.index=0,.baseAddr = SOC_I2C_0_REGS,    .moduleClk=&i2c0ModClock},
   [MODULE_ID_I2C1]   = {.index=1,.baseAddr = SOC_I2C_1_REGS,    .moduleClk=&i2c1ModClock},
   [MODULE_ID_I2C2]   = {.index=2,.baseAddr = SOC_I2C_2_REGS,    .moduleClk=&i2c2ModClock},
   [MODULE_ID_SPI0]   = {.index=0,.baseAddr = SOC_SPI_0_REGS,.INTNum={SYS_INT_SPI0INT,0,0},.INTPriority={INT_PRIORITY_SPI1,0,0},.isr={isr_mcspi,0,0}, .moduleClk=&spi0ModClock},
   [MODULE_ID_SPI1]   = {.index=1,.baseAddr = SOC_SPI_1_REGS,.INTNum={SYS_INT_SPI1INT,0,0},.INTPriority={INT_PRIORITY_SPI1,0,0},.isr={isr_mcspi,0,0}, .moduleClk=&spi1ModClock},
   [MODULE_ID_RTC]    = {.index=0,.baseAddr = SOC_RTC_0_REGS,.INTNum={0,0,0},              .INTPriority={0,0,0},                .isr={0,0,0},         .moduleClk=&rtcModClock},
};





