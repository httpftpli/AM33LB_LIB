#ifndef __PF_HSI2C_H_
#define __PF_HSI2C_H_

#include "type.h"



extern void I2CInit(unsigned int moduleId,unsigned int i2cClkFreq,
             unsigned short *slaveAddr,
             unsigned int szSlave);
BOOL I2CMasterWrite(unsigned baseAddr,unsigned short slaveAddr,const void *buf,unsigned int szWrite);
BOOL I2CMasterWriteEx(unsigned baseAddr,unsigned short slaveAddr,const void *w1buf,unsigned int szW1,
                      const void *w2buf,unsigned int szW2);
BOOL I2CMasterRead(unsigned baseAddr,unsigned short slaveAddr,void *buf,unsigned int szRead);
BOOL I2CMasterReadEx(unsigned baseAddr,unsigned short slaveAddr,const void *writeBuf ,unsigned int szWrite,
                     void *readBuf,unsigned int szRead);
     
                
#endif /* __PF_HSI2C_H_ */                 
