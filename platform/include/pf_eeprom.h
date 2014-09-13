
#ifndef  _PF_EEPROM_H_
#define  _PF_EEPROM_H_
#include "type.h"
#include  <stdbool.h>



extern BOOL i2cEepromRead(unsigned short slaveAddr,unsigned int addr,void *buf,unsigned int szBuf);
extern BOOL i2cEepromWrite(unsigned short slaveAddr,unsigned int addr,const void *buf,unsigned int szBuf);
extern BOOL spiFmromInit(void *mirrorBuf,unsigned int szBuf);
extern BOOL spiFmromWrite(unsigned short addr,void *buf,unsigned int szbuf);
extern BOOL spiFmromRead(unsigned short addr,void *buf,unsigned int szbuf);
extern BOOL spiFmromWrEnable(void);
extern BOOL spiFmromWrDisable(void);
extern unsigned char  spiFmromRdStatReg(void);

extern BOOL spiFlashRead(unsigned int addr,void *buf,unsigned int szbuf);
extern BOOL spiFlashWren(void);
//extern BOOL spiFlashWrite(unsigned int addr,void *prebuf,unsigned int prebuflen,void *buf,unsigned int szbuf);
extern BOOL spiFlashSwitch256PageSize(void);
extern unsigned char spiFlashReadStatus(void);
extern bool spiFlashIsBusy(void);
extern bool spiFlashChipErase_block(void);
extern void spiFlashReadId(void *buf,unsigned int len);
extern bool spiFlashPageWrite(unsigned int addr,void *buf,unsigned int szbuf);



#endif /* _PF_EEPROM_H_ */


