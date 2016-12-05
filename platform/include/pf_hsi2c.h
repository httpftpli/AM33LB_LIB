#ifndef __PF_HSI2C_H_
#define __PF_HSI2C_H_

#include "type.h"


#define I2C_IRQ_TRIG_DATA_READY   1
#define I2C_IRQ_TRIG_SEND_FINISH  2
#define I2C_IRQ_TRIG_STOP         3  


extern void I2CInit(unsigned int moduleId,unsigned int i2cClkFreq,
             unsigned short *slaveAddr,
             unsigned int szSlave);
extern BOOL I2CMasterWrite(unsigned baseAddr,unsigned short slaveAddr,const void *buf,unsigned int szWrite);
extern BOOL I2CMasterWriteEx(unsigned baseAddr,unsigned short slaveAddr,const void *w1buf,unsigned int szW1,
                      const void *w2buf,unsigned int szW2);
extern bool I2CMasterWriteNoBlock(unsigned int baseAddr, unsigned short slaveAddr, void *buf,unsigned int szWrite);

extern BOOL I2CMasterRead(unsigned baseAddr,unsigned short slaveAddr,void *buf,unsigned int szRead);
extern BOOL I2CMasterReadEx(unsigned baseAddr,unsigned short slaveAddr,const void *writeBuf ,unsigned int szWrite,
                     void *readBuf,unsigned int szRead);

extern bool I2CMasterReadNoBlock(unsigned int baseAddr, unsigned short slaveAddr, unsigned int szRead); 

extern void I2CRegistHandler(unsigned int module,void (*handler)(unsigned int index,unsigned int stat));


     
                
#endif /* __PF_HSI2C_H_ */                 
