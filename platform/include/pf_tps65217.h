#ifndef  __PF__TPS65217__H__
#define  __PF__TPS65217__H__

#include <stdbool.h>


extern unsigned char TPS65217RegRead(unsigned char regOffset);

extern bool TPS65217RegWrite(unsigned char regOffset,
                      unsigned char dest_val, unsigned char mask);
  
extern bool TPS65217VoltageUpdate(unsigned char dc_cntrl_reg, unsigned char volt_sel);

extern unsigned char Tps65712ChipId();


#endif