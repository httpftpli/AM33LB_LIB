
#include "pf_hsi2c.h"
#include "pf_spi.h"
#include "soc_am335x.h"
#include "module.h"

#define EEPROM_HSI2C_BASEADDR   SOC_I2C_0_REGS

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


BOOL i2cEepromRead(unsigned short slaveAddr,unsigned int addr,void *buf,unsigned int szBuf){
   return I2CMasterReadEx(EEPROM_HSI2C_BASEADDR,slaveAddr,&addr,2,buf,szBuf);
}

BOOL i2cEepromWrite(unsigned short slaveAddr,unsigned int addr,const void *buf,unsigned int szBuf){
   return I2CMasterWriteEx(EEPROM_HSI2C_BASEADDR,slaveAddr,&addr,2,buf,szBuf);
}

BOOL spiFmromWrite(unsigned short addr,void *buf,unsigned int szbuf){
   static unsigned char prebuf[3];
   prebuf[0] = FM25XX_OPCODE_WRITE;
   prebuf[1] = addr >> 8;
   prebuf[2] = (char)addr;
   return SPIWrite(MODULE_ID_SPI1,prebuf,3,buf,szbuf,0,NULL,0);
}

BOOL spiFmromRead(unsigned short addr,void *buf,unsigned int szbuf){
   static unsigned char prebuf[3];
   prebuf[0] = FM25XX_OPCODE_READ;
   prebuf[1] = addr >> 8;
   prebuf[2] = (char)addr;
   return SPIRead(MODULE_ID_SPI1,prebuf,3,buf,szbuf);
}
