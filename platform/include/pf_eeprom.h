
#ifndef  _PF_EEPROM_H_
#define  _PF_EEPROM_H_




extern BOOL i2cEepromRead(unsigned short slaveAddr,unsigned int addr,void *buf,unsigned int szBuf);
extern BOOL i2cEepromWrite(unsigned short slaveAddr,unsigned int addr,const void *buf,unsigned int szBuf);
extern BOOL spiFmromWrite(unsigned short addr,void *buf,unsigned int szbuf);
extern BOOL spiFmromRead(unsigned short addr,void *buf,unsigned int szbuf);
 
#endif /* _PF_EEPROM_H_ */ 


