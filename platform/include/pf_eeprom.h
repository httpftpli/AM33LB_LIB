
#ifndef   _PF_EEPROM_H_
#define  _PF_EEPROM_H_

BOOL EepromRead(unsigned short slaveAddr,unsigned int addr,void *buf,unsigned int szBuf);
BOOL EepromWrite(unsigned short slaveAddr,unsigned int addr,const void *buf,unsigned int szBuf);

#endif /* _PF_EEPROM_H_ */ 


