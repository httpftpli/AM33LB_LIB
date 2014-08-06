#ifndef __PF__CPSW__C_
#define __PF__CPSW__C_

extern void CPSWClkEnable(void);
extern void PortRmiiModeSelect(void);
extern void EVMMACAddrGet(unsigned int addrIdx, unsigned char *macAddr);

#endif 

