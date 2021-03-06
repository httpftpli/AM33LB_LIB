#ifndef _PF_QEP_H_
#define _PF_QEP_H_

#define QEP_INPUT_CLK_100MZ         100000000  


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
#define QEP_SETPOS_STROBE_WISE            (3<<3)


#ifdef __cplusplus
extern "C" {
#endif

extern void QEPRegistHandler(unsigned int moduleId,void (*handler)(unsigned int flag));
extern int QEPGetVelocity(unsigned int moduleId);
extern void QEPVelocityDetectStart(unsigned int moduleId,unsigned int timeResolution_us);
extern void QEPInit(unsigned int moduleId,unsigned int inputmode);
extern void QEPSetPos(unsigned int moduleId,unsigned int pos,unsigned int setEvent);
extern void QEPSwapQuadInput(unsigned int moduleId);
extern void QEPSetPosFactor(unsigned int factor);
extern void QEPSetPosCompare(unsigned int moduleId,unsigned int compare);
extern unsigned int QEPReadPos(unsigned int moduleId);
extern int QEPCalcuLatchVelocity(unsigned int moduleId);//pulse*factor/s
extern void QEPSetPosCompareCurrent(unsigned int moduleId);
            

#ifdef __cplusplus
}
#endif

#endif
