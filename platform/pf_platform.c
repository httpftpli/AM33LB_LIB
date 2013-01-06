/**
 * \file   platform.c
 *
 * \brief  This file contains functions which does the platform 
 *         specific.
 */



#include "mmu.h"
#include "cache.h"
#include "interrupt.h"
#include "edma.h"
#include "platform.h"
#include "soc_AM335X.h"
#include "pf_mux.h"
#include "debug.h"
#include "hw_cm_per.h"
#include "hw_control_AM335x.h"

extern void EDMAModuleClkConfig(void);


/* MMU related macros. */
#define START_ADDR_OCMC                 0x40300000
#define START_ADDR_DDR                  0x80000000
#define START_ADDR_DEV                  0x44000000
#define NUM_SECTIONS_DDR                512
#define NUM_SECTIONS_DEV                960
#define NUM_SECTIONS_OCMC               1


/* page tables start must be aligned in 16K boundary */                  //
#ifdef __TMS470__
#pragma DATA_ALIGN(pageTable, MMU_PAGETABLE_ALIGN_SIZE);
static volatile  unsigned int pageTable[MMU_PAGETABLE_NUM_ENTRY];
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=MMU_PAGETABLE_ALIGN_SIZE
static volatile __no_init unsigned int pageTable[MMU_PAGETABLE_NUM_ENTRY];
#elif defined(gcc)
static volatile unsigned int pageTable[MMU_PAGETABLE_NUM_ENTRY]
__attribute__((aligned(MMU_PAGETABLE_ALIGN_SIZE)));
#else
#error "Unsupported Compiler. \r\n"
#endif




static void PinMuxSetup(void){
  MUX_VAL(CONTROL_PADCONF_MMC0_DAT3, (IEN | PU | MODE0 )) /* MMC0_DAT3 */\
  MUX_VAL(CONTROL_PADCONF_MMC0_DAT2, (IEN | PU | MODE0 )) /* MMC0_DAT2 */\
  MUX_VAL(CONTROL_PADCONF_MMC0_DAT1, (IEN | PU | MODE0 )) /* MMC0_DAT1 */\
  MUX_VAL(CONTROL_PADCONF_MMC0_DAT0, (IEN | PU | MODE0 )) /* MMC0_DAT0 */\
  MUX_VAL(CONTROL_PADCONF_MMC0_CLK, (IEN | PU | MODE0 )) /* MMC0_CLK */\
  MUX_VAL(CONTROL_PADCONF_MMC0_CMD, (IEN | PU | MODE0 )) /* MMC0_CMD */\
  MUX_VAL(CONTROL_PADCONF_SPI0_CS1, (IEN | OFF | MODE5 )) /* MMC0_SDCD_MUX0 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_AD0,(IEN | PU | MODE1)) /* MMC1_DAT0_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_AD1,(IEN | PU | MODE1)) /* MMC1_DAT1_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_AD2,(IEN | PU | MODE1)) /* MMC1_DAT2_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_AD3,(IEN | PU | MODE1)) /* MMC1_DAT3_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_AD4,(IEN | PU | MODE1)) /* MMC1_DAT4_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_AD5,(IEN | PU | MODE1)) /* MMC1_DAT5_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_AD6,(IEN | PU | MODE1)) /* MMC1_DAT6_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_AD7,(IEN | PU | MODE1)) /* MMC1_DAT7_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_CSN1,(IEN | PU | MODE2)) /* MMC1_CLK_MUX0 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_CSN2,(IEN | PU | MODE2)) /* MMC1_CMD_MUX0 */\
  MUX_VAL(CONTROL_PADCONF_GPMC_A2, (IDIS | PD | MODE6 )) /* EHRPWM1A_MUX1 */\
  MUX_VAL(CONTROL_PADCONF_UART1_CTSN, (IEN | OFF | MODE2 )) /* DCAN0_TX_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_UART1_RTSN, (IEN | OFF | MODE2 )) /* DCAN0_RX_MUX2 */\
  MUX_VAL(CONTROL_PADCONF_I2C0_SDA, (IEN | OFF | MODE0 )) /* I2C0_SDA */\
  MUX_VAL(CONTROL_PADCONF_I2C0_SCL, (IEN | OFF | MODE0 )) /* I2C0_SCL */\
  MUX_VAL(CONTROL_PADCONF_SPI0_D1, (IEN | OFF | MODE2 )) /* I2C1_SDA_MUX3 */\
  MUX_VAL(CONTROL_PADCONF_SPI0_CS0, (IEN | OFF | MODE2 )) /* I2C1_SCL_MUX3 */\
  MUX_VAL(CONTROL_PADCONF_LCD_DATA4, (IEN | OFF | MODE3 )) /* EQEP2A_IN_MUX0 */\
  MUX_VAL(CONTROL_PADCONF_LCD_DATA5, (IEN | OFF | MODE3 )) /* EQEP2B_IN_MUX0 */

}

/*
** This function will setup the MMU. The function maps three regions -
** 1. DDR
** 2. OCMC RAM
** 3. Device memory
** The function also enables the MMU.
*/
static void MMUConfigAndEnable(void) {
   /*
   ** Define DDR memory region of AM335x. DDR can be configured as Normal
   ** memory with R/W access in user/privileged modes. The cache attributes
   ** specified here are,
   ** Inner - Write through, No Write Allocate
   ** Outer - Write Back, Write Allocate
   */
   REGION regionDdr = {
      MMU_PGTYPE_SECTION, START_ADDR_DDR, NUM_SECTIONS_DDR,
      MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA,
                                       MMU_CACHE_WB_WA),
      MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
      (unsigned int *)pageTable
   };
   /*
   ** Define OCMC RAM region of AM335x. Same Attributes of DDR region given.
   */
   REGION regionOcmc = {
      MMU_PGTYPE_SECTION, START_ADDR_OCMC, NUM_SECTIONS_OCMC,
      MMU_MEMTYPE_NORMAL_NON_SHAREABLE(MMU_CACHE_WT_NOWA,
                                       MMU_CACHE_WB_WA),
      MMU_REGION_NON_SECURE, MMU_AP_PRV_RW_USR_RW,
      (unsigned int *)pageTable
   };

   /*
   ** Define Device Memory Region. The region between OCMC and DDR is
   ** configured as device memory, with R/W access in user/privileged modes.
   ** Also, the region is marked 'Execute Never'.
   */
   REGION regionDev = {
      MMU_PGTYPE_SECTION, START_ADDR_DEV, NUM_SECTIONS_DEV,
      MMU_MEMTYPE_DEVICE_SHAREABLE,
      MMU_REGION_NON_SECURE,
      MMU_AP_PRV_RW_USR_RW  | MMU_SECTION_EXEC_NEVER,
      (unsigned int *)pageTable
   };

   /* Initialize the page table and MMU */
   MMUInit((unsigned int *)pageTable);

   /* Map the defined regions */
   MMUMemRegionMap(&regionDdr);
   MMUMemRegionMap(&regionOcmc);
   MMUMemRegionMap(&regionDev);

   /* Now Safe to enable MMU */
   MMUEnable((unsigned int *)pageTable);
}


static void EDMAInit(){
   EDMAModuleClkConfig();
   EDMA3Init(SOC_EDMA30CC_0_REGS);
   //enable 64 edma interrupt
   for (int i=0;i<64;i++) {
      EDMA3EnableEvtIntr(SOC_EDMA30CC_0_REGS,i);
   }
}


static void L3L4ClockInit(void){
   /*domain power state transition*/
   HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) =
                             CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP; //L3S
    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) =
                             CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;//L3
    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

   /*clocl control */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) =
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;//L3_INSTR

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) =    
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;//L3_interconnect

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);


/*domain power state transition*/
    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) =
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP; //OCPWP_L3
    while((HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) =
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;//L4LS
    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);
 /*clocl control */
    HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) =
                             CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;//L4LS
    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) &
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

 /*poll L3L4 clock*/
    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK |
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK))); 

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_CAN_CLK)));
}



void platformInit(void) {
   MMUConfigAndEnable();
   CacheEnable(CACHE_ICACHE);
   L3L4ClockInit();
   IntAINTCInit();
   RTCInit(); 
   UARTStdioInit(); 
   PinMuxSetup();
   EDMAInit();
   DCANModuleClkConfig();
   DCANMsgRAMInit(0);
   DCANMsgRAMInit(1);
   I2C0ModuleClkConfig();
   I2C1ModuleClkConfig();
   DMTimer2ModuleClkConfig();  //dtimer2 for tick
   PWMSSModuleClkConfig(2);    //PWMSS for qep2
   HSMMCSDModuleClkConfig();
   perAINTCConfigure();   
   IntMasterIRQEnable();
}
