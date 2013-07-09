#include "gpmc.h"
#include "pf_platform_cfg.h" 
#include "hw_types.h"
#include "soc_am335x.h"
#include "hw_cm_per.h"
#include "debug.h"
#include "type.h"
#include "module.h"


static void GPMCConfigCS(unsigned int baseAddr,unsigned char cs,unsigned int bankBaseAddr,unsigned int bitOfAddrLine){
   mdAssert(cs < 7);
   mdAssert((bankBaseAddr & 0x00ffffff)==0);
   if (bitOfAddrLine < 24) bitOfAddrLine = 24;
   unsigned int basea = (bankBaseAddr & 0x3fffffff)>>24;
   unsigned char maskaddr = 0;
   switch (bitOfAddrLine) {
   case 24: maskaddr = 0xf;break;
   case 25: maskaddr = 0xe;break;
   case 26: maskaddr = 0xc;break;
   case 27: maskaddr = 0x8;break;
   case 28: maskaddr = 0x0;break;
   default: maskaddr = 0x0;
   }
   HWREG(baseAddr + GPMC_CONFIG7(cs)) = basea | (maskaddr<<8);
}


static void GPMCEnableCS(unsigned int baseAddr,unsigned char cs,BOOL enable){
   mdAssert(cs<7);
   unsigned int val = HWREG(baseAddr + GPMC_CONFIG7(cs));
   val |= (!!enable)<<6;
   HWREG(baseAddr + GPMC_CONFIG7(cs)) = val;
}


void GPMCInitForNOR(){
  MODULE *module = modulelist+MODULE_ID_GPMC;
  unsigned int baseAddr = module->baseAddr;
  GPMCModuleSoftReset(baseAddr);
  GPMCIdleModeSelect(baseAddr, GPMC_IDLEMODE_NOIDLE);
  //NOR Timings Configuration
  moduleEnable(MODULE_ID_GPMC);
  for (int i=0;i<7;i++) {
     // NOR Memory type Configuration
     HWREG(baseAddr + GPMC_CONFIG1(i)) = 0<<31   |  0 << 30   |  0<<29 |    0<<28    |  0<<27  |
                                      //WRAPBURST|READMULTIPLE|READTYPE|WRITEMULTIPLE|WRITETYPE|
         0x00 <<25    |     0x00 << 23         |     0<<22        |     0<<21          | 
   //CLKACTIVATIONTIME|ATTACHEDDEVICEPAGELENGTH|WAITREADMONITORING| WAITWRITEMONITORING| 
        1 << 12  | 0 << 10  | 0x00 << 8 |   0 << 4           |  0 << 0;
     //DEVICESIZE|DEVICETYPE| MUXADDDATA|TIMEPARAGRANULARITY | GPMCFCLKDIVIDER
         
     
     // Chip-Select Configuration
     GPMCConfigCS(baseAddr, i, CS_BASEADDR[i], CS_ADDRLINE_BIT[i]);
     //config CS signal parameter
     HWREG(baseAddr + GPMC_CONFIG2(i)) =  15<<8      |   14<<16    | 0<<0;
                                       //CSRDOFFTIME | CSWROFFTIME | CSONTIME
     //config RWn RWn 
     HWREG(baseAddr + GPMC_CONFIG4(i)) = 9<<24     |  2<<16   |    15<<8  |  3<<0;
                                       //WEOFFTIME | WEONTIME | OEOFFTIME | OEONTIME
     //config access time cycle time
     HWREG(baseAddr + GPMC_CONFIG5(i)) = 11<<16        |    14<<8    |    16<<0;
                                       // RDACCESSTIME | WRCYCLETIME | RDCYCLETIME
     //HWREG(baseAddr + GPMC_CONFIG6(i)) = 10<<24 ;
                                       // CYCLE2CYCLEDELAY 
  }
  //enable CSi
  for (int i=0; i < 7; i++) {
     GPMCEnableCS(baseAddr, i, CS_USE & (1 << i));
  }
}

