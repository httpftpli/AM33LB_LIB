/**
 *  \file       qep.h
 *
 *  \brief      This file contains the function prototypes for the device
 *              abstraction layer for ECAP. It also contains some
 *              related macro definitions and some files to be included.
 */





#ifndef _QEP_H_
#define _QEP_H_

#include "interrupt.h"
#include "soc_AM335x.h"

#ifdef __cplusplus
extern "C" {
#endif




#define QEP_MODE_QUAD         0
#define QEP_MODE_DERECTION    1
#define QEP_MODE_UPCOUNT      2      
#define QEP_MODE_DOWNCOUNT    3
extern void QEPInit(unsigned int baseAddr,unsigned int inFreq,unsigned int inputmode);

#define QEP_SETPOS_IMMED                  (1<<0)
#define QEP_SETPOS_INDEX_RISEEDGE         (2<<1)
#define QEP_SETPOS_INDEX_FALLEDGE         (3<<1)
#define QEP_SETPOS_STROBE_RISEEDGE        (2<<3)
#define QEP_SETPOS_STROBE_FALLEDGE        (3<<3)
extern void QEPSetPos(unsigned int baseAddr,unsigned int pos,unsigned int setevent);

extern void QEPSwapQuadInput(unsigned int baseAddr);

extern void QEPSetPosFactor(unsigned int factor);

extern void QEPSetPosCompare(unsigned int baseAddr,unsigned int compare);

extern unsigned int QEPReadPos(unsigned int baseAddr);
extern signed int QEPCalcuLatchVelocity(unsigned int baseAddr);//pulse*factor/s

extern void QEPSetPosCompareCurrent(unsigned int baseAddr);

extern void QEPVelocityDetectStart(unsigned int baseAddr,unsigned int timeResolution_us);

 
 
static inline  unsigned int intnum2addr(unsigned int intnum){
   switch (intnum) {
   case SYS_INT_eQEP0INT:
      return SOC_EQEP_0_REGS;
   case SYS_INT_eQEP1INT:
      return SOC_EQEP_1_REGS;
   case SYS_INT_eQEP2INT:
      return SOC_EQEP_2_REGS;
   }
   return 0;
}

static inline unsigned int intnum2index(unsigned int intnum){
   switch (intnum) {
    case SYS_INT_eQEP0INT:
      return 0;
   case SYS_INT_eQEP1INT:
      return 1;
   case SYS_INT_eQEP2INT:
      return 2;
   }
   return -1;
}

static inline unsigned int addr2index(unsigned int baseAddr){
   switch (baseAddr) {
   case SOC_EQEP_0_REGS:
      return 0;
   case SOC_EQEP_1_REGS:
      return 1;
   case SOC_EQEP_2_REGS:
      return 2;
   }
   return -1;
}           
           

#ifdef __cplusplus
}
#endif

#endif
