
#include "pf_hsi2c.h"
#include "soc_am335x.h"

#define EEPROM_HSI2C_BASEADDR   SOC_I2C_0_REGS


BOOL EepromRead(unsigned short slaveAddr,unsigned int addr,void *buf,unsigned int szBuf){
   return I2CMasterReadEx(EEPROM_HSI2C_BASEADDR,slaveAddr,&addr,2,buf,szBuf);
}

BOOL EepromWrite(unsigned short slaveAddr,unsigned int addr,const void *buf,unsigned int szBuf){
   return I2CMasterWriteEx(EEPROM_HSI2C_BASEADDR,slaveAddr,&addr,2,buf,szBuf);
}
