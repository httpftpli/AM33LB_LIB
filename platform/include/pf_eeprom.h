
#ifndef  _PF_EEPROM_H_
#define  _PF_EEPROM_H_




extern BOOL EepromRead(unsigned short slaveAddr,unsigned int addr,void *buf,unsigned int szBuf);
extern BOOL EepromWrite(unsigned short slaveAddr,unsigned int addr,const void *buf,unsigned int szBuf);

#endif /* _PF_EEPROM_H_ */ 


