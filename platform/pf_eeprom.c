
#include "pf_hsi2c.h"
#include "pf_spi.h"
#include "soc_am335x.h"
#include "module.h"
#include "pf_eeprom.h"
#include "string.h"
#include "debug.h"
#include "pf_gpio.h"
#include "mem.h"


#define EEPROM_HSI2C_BASEADDR   SOC_I2C_0_REGS

#define SPI_FLASH_CS       0

#define FMROM_WP_GPIO         SOC_GPIO_0_REGS
#define FMROM_WP_PIN          31
#define FMROM_WP_GPIO_MODULE  MODULE_ID_GPIO0
#define FMROM_SPI_MODULE      MODULE_ID_SPI1



#define FM25XX_OPCODE_WREN    0x06
#define FM25XX_OPCODE_WRDI    0x04
#define FM25XX_OPCODE_RDSR    0x05
#define FM25XX_OPCODE_WRSR    0x01
#define FM25XX_OPCODE_READ    0x03
#define FM25XX_OPCODE_FSTRD   0x0b
#define FM25XX_OPCODE_WRITE   0x02
#define FM25XX_OPCODE_SLEEP   0xb9
#define FM25XX_OPCODE_RDID    0x9f
#define FM25XX_OPCODE_SNR     0xc3


#define FLASH_OPCODE_WREN         0x06
#define FLASH_OPCODE_READ         0x03
#define FLASH_OPCODE_PAGE_PROGRAM 0x82
#define FLASH_OPCODE_READID       0x9f


BOOL i2cEepromRead(unsigned short slaveAddr,unsigned int addr,void *buf,unsigned int szBuf){
   return I2CMasterReadEx(EEPROM_HSI2C_BASEADDR,slaveAddr,&addr,2,buf,szBuf);
}

BOOL i2cEepromWrite(unsigned short slaveAddr,unsigned int addr,const void *buf,unsigned int szBuf){
   return I2CMasterWriteEx(EEPROM_HSI2C_BASEADDR,slaveAddr,&addr,2,buf,szBuf);
}

static char * fmRomMirrorBuf = 0;
static int szFmRomMirrorBuf = 0;

static unsigned char prebuf[2][5],checkprebuf[2][4];

BOOL spiFmromInit(void *mirrorBuf,unsigned int szBuf){
   fmRomMirrorBuf = (char *)mirrorBuf;
   szFmRomMirrorBuf = szBuf;
   unsigned int index = modulelist[FMROM_SPI_MODULE].index;
   ASSERT((index==0)||(index==1));
   prebuf[index][0] = FM25XX_OPCODE_READ;
   prebuf[index][1] = 0;
   prebuf[index][2] = 0;
   GPIOInit(FMROM_WP_GPIO_MODULE,0,0);
   __GPIOPinWrite(FMROM_WP_GPIO,FMROM_WP_PIN,0);
   GPIODirModeSet(FMROM_WP_GPIO,FMROM_WP_PIN,GPIO_DIR_OUTPUT);
   return SPIRead(FMROM_SPI_MODULE,prebuf+index,3,mirrorBuf,szBuf);
}

BOOL spiFmromWrite(unsigned short addr,void *buf,unsigned int szbuf){
   ASSERT(fmRomMirrorBuf!=NULL);
   ASSERT((addr+szbuf)<= szFmRomMirrorBuf);
   if(g_spitransfer.finish==0)
     return FALSE;
   memcpy(addr+fmRomMirrorBuf,buf,szbuf);
   unsigned int index = modulelist[FMROM_SPI_MODULE].index;
   ASSERT((index==0)||(index==1));
   prebuf[index][0] = FM25XX_OPCODE_WRITE;
   prebuf[index][1] = addr >> 8;
   prebuf[index][2] = (char)addr;
   checkprebuf[index][0] = FM25XX_OPCODE_READ;
   checkprebuf[index][1] = addr >> 8;
   checkprebuf[index][2] = (char)addr;
   return SPIWrite(FMROM_SPI_MODULE,prebuf+index,3,fmRomMirrorBuf+addr,szbuf,1,checkprebuf+1,3);
}

BOOL spiFmromRead(unsigned short addr,void *buf,unsigned int szbuf){
   ASSERT(fmRomMirrorBuf!=NULL);
   ASSERT((addr+szbuf)<= szFmRomMirrorBuf);
   if(g_spitransfer.finish==0)
     return FALSE;
   memcpy(buf,addr+fmRomMirrorBuf,szbuf);
   unsigned int index = modulelist[FMROM_SPI_MODULE].index;
   ASSERT((index==0)||(index==1));
   prebuf[index][0] = FM25XX_OPCODE_READ;
   prebuf[index][1] = addr >> 8;
   prebuf[index][2] = (char)addr;
   return SPIRead(FMROM_SPI_MODULE,prebuf+index,3,addr+fmRomMirrorBuf,szbuf);
}

BOOL spiFmromWrEnable(void){
   if(g_spitransfer.finish==0)
     return FALSE;
   //GPIOPinWrite(FMROM_WP_GPIO,FMROM_WP_PIN,1);
   unsigned int index = modulelist[FMROM_SPI_MODULE].index;
   ASSERT((index==0)||(index==1));
   prebuf[index][0] = FM25XX_OPCODE_WREN;
   BOOL re =  SPIWrite(FMROM_SPI_MODULE,prebuf+index,1,NULL,0,0,NULL,0);
   //GPIOPinWrite(FMROM_WP_GPIO,FMROM_WP_PIN,0);
   return re;
}

BOOL spiFmromWrDisable(void){
   if(g_spitransfer.finish==0)
     return FALSE;
   //GPIOPinWrite(FMROM_WP_GPIO,FMROM_WP_PIN,1);
   unsigned int index = modulelist[FMROM_SPI_MODULE].index;
   ASSERT((index==0)||(index==1));
   prebuf[index][0] = FM25XX_OPCODE_WRDI;
   BOOL re =  SPIWrite(FMROM_SPI_MODULE,prebuf+index,1,NULL,0,0,NULL,0);
   //GPIOPinWrite(FMROM_WP_GPIO,FMROM_WP_PIN,0);
   return re;
}

unsigned char  spiFmromRdStatReg(void){
   if(g_spitransfer.finish==0)
     return FALSE;
   unsigned int index = modulelist[FMROM_SPI_MODULE].index;
   ASSERT((index==0)||(index==1));
   prebuf[index][0] = FM25XX_OPCODE_RDSR;
   unsigned char dat;
   if(SPIRead(FMROM_SPI_MODULE,prebuf+index,1,&dat,1)==FALSE){
      return -1;
   }
   return dat;
}


BOOL  spiFmromRdId(unsigned char *manId,unsigned char *densitycode,unsigned char *deviceId){
   if(g_spitransfer.finish==0)
     return FALSE;
   unsigned int index = modulelist[FMROM_SPI_MODULE].index;
   ASSERT((index==0)||(index==1));
   prebuf[index][0] = FM25XX_OPCODE_RDID;
   unsigned char dat[9];
   memset(dat,0,sizeof dat);
   if(!SPIRead(FMROM_SPI_MODULE,prebuf+index,1,&dat,9)){
      return FALSE;
   }
   while (!isSpiTransferFinish(MODULE_ID_SPI1));
   if (SPIGetState(MODULE_ID_SPI1)!= STATE_OK) {
      return FALSE;
   }
   if (!memis(dat, 0x7f, 6)) {
      return FALSE;
   }
   *manId = dat[6];
   *densitycode = dat[7];
   *deviceId =  dat[8];
   return TRUE;
}





/*BOOL spiFlashWren(void){
   if(g_spitransfer.finish==0)
     return FALSE;
   prebuf[0] = FLASH_OPCODE_WREN;
   SPIWrite(MODULE_ID_SPI0,prebuf,1,NULL,0,0,NULL,0);
   while (g_spitransfer.finish==0);
   return TRUE;
}*/


bool spiFlashChipErase_block(void){
   unsigned int count = 10000000;
   unsigned char buf[] = {0xc7,0x94,0x80,0x9a};
   SPIWrite_blockPoll(MODULE_ID_SPI0,SPI_FLASH_CS,NULL,0,buf,sizeof buf);
   while(spiFlashIsBusy()&& count--);
   if(count==0)
     return false;
   return true;
}



BOOL spiFlashPageWrite(unsigned int addr,void *buf,unsigned int szbuf){
   unsigned char prebuf[4];
   prebuf[0] = 0x84;
   prebuf[1] = 0;
   prebuf[2] = 0;
   prebuf[3] = 0;
   SPIWrite_blockPoll(MODULE_ID_SPI0,SPI_FLASH_CS,prebuf,4,buf,szbuf);
   prebuf[0] = 0x83;
   prebuf[1] = (unsigned char)(addr >>16);
   prebuf[2] = (unsigned char)(addr >> 8);
   prebuf[3] = 0;
   SPIWrite_blockPoll(MODULE_ID_SPI0,SPI_FLASH_CS,prebuf,4,NULL,0);
   while(spiFlashIsBusy());
   return TRUE;
}

BOOL spiFlashRead(unsigned int addr,void *buf,unsigned int szbuf){
   unsigned char prebuf[4];
   prebuf[0] = FLASH_OPCODE_READ;
   prebuf[1] = (unsigned char)(addr >> 16);
   prebuf[2] = (unsigned char)(addr >> 8);
   prebuf[3] = (unsigned char)addr;
   SPIRead_blockPoll(MODULE_ID_SPI0,SPI_FLASH_CS,prebuf,4,buf,szbuf);
   return TRUE;
}


void spiFlashReadId(void *buf,unsigned int len){
   unsigned char prebuf[1] = {FLASH_OPCODE_READID};
   SPIRead_blockPoll(MODULE_ID_SPI0,SPI_FLASH_CS,prebuf,1,buf,len);
}


BOOL spiFlashSwitch256PageSize(void) {

    unsigned char buf[] = {0x3d, 0x2a ,0x80,0xa6};
    if(spiFlashReadStatus() & 0x01) return true;
    SPIWrite_blockPoll(MODULE_ID_SPI0, SPI_FLASH_CS,NULL,0,buf, sizeof buf);
    while(spiFlashIsBusy()) ;
    if(spiFlashReadStatus() & 0x01){
        return true;
    }
    return false;
}


unsigned char spiFlashReadStatus(void){
   unsigned char buf[2] = {0xd7,0};
   SPIRead_blockPoll(MODULE_ID_SPI0,SPI_FLASH_CS,NULL ,0,buf,sizeof buf);
   return buf[1];
}


bool spiFlashIsBusy(void){
   return !(spiFlashReadStatus()&0x80);
}




