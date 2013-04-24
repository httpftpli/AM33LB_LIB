#ifndef _PF_QEP_H_
#define _PF_QEP_H_


#define QEP_HANDER_FlAG_COMPARE_MATCH     1
#define QEP_HANDER_FlAG_PHASE_ERROR       2
#define QEP_HANDER_FlAG_POSCNT_ERROR      3
#define QEP_HANDER_FlAG_POSCNT_OVERFLOW   4
#define QEP_HANDER_FlAG_POSCNT_UNDERFLOW  5

#define QEP_MODE_QUAD         0
#define QEP_MODE_DERECTION    1
#define QEP_MODE_UPCOUNT      2      
#define QEP_MODE_DOWNCOUNT    3

#define QEP_SETPOS_IMMED                  (1<<0)
#define QEP_SETPOS_INDEX_RISEEDGE         (2<<1)
#define QEP_SETPOS_INDEX_FALLEDGE         (3<<1)
#define QEP_SETPOS_STROBE_RISEEDGE        (2<<3)
#define QEP_SETPOS_STROBE_FALLEDGE        (3<<3)


#ifdef __cplusplus
extern "C" {
#endif

extern void QEPRegistHandler(unsigned int moduleId,void (*handler)(unsigned int flag));
extern int QEPGetVelocity(unsigned int moduleId);
extern void QEPVelocityDetectStart(unsigned int moduleId,unsigned int timeResolution_us);
extern void QEPInit(unsigned int baseAddr,unsigned int inFreq,unsigned int inputmode);
extern void QEPSetPos(unsigned int baseAddr,unsigned int pos,unsigned int setevent);
extern void QEPSwapQuadInput(unsigned int baseAddr);
extern void QEPSetPosFactor(unsigned int factor);
extern void QEPSetPosCompare(unsigned int baseAddr,unsigned int compare);
extern unsigned int QEPReadPos(unsigned int baseAddr);
extern signed int QEPCalcuLatchVelocity(unsigned int moduleId);//pulse*factor/s
extern void QEPSetPosCompareCurrent(unsigned int baseAddr);
            

#ifdef __cplusplus
}
#endif

#endif
