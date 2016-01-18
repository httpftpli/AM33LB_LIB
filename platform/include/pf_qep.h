#ifndef _PF_QEP_H_
#define _PF_QEP_H_

#include <stdbool.h>

#define QEP_INPUT_CLK_100MZ         100000000


#define QEP_HANDLER_FLAG_COMPARE_MATCH     1
#define QEP_HANDLER_FLAG_PHASE_ERROR       2
#define QEP_HANDLER_FLAG_POSCNT_ERROR      3
#define QEP_HANDLER_FLAG_POSCNT_OVERFLOW   4
#define QEP_HANDLER_FLAG_POSCNT_UNDERFLOW  5
#define QEP_HANDLER_FLAG_STROB             6
#define QEP_HANDLER_FLAG_DIR_CHANGE        7
#define QEP_HANDLER_FLAG_INDEX             8


#define QEPINT_ENABLE_FLAG_UNITTIME        1<<11
#define QEPINT_ENABLE_FLAG_INDEX           1<<10
#define QEPINT_ENABLE_FLAG_STROB           1<<9
#define QEPINT_ENABLE_FLAG_POSCOMPARE      1<<8
#define QEPINT_ENABLE_FLAG_POSOVERFLOW     1<<6
#define QEPINT_ENABLE_FLAG_POSUNDERFLOW    1<<5
#define QEPINT_ENABLE_FLAG_DIRCHANGE       1<<3
#define QEPINT_ENABLE_FLAG_PHASEERROR      1<<2
#define QEPINT_ENABLE_FLAG_COUNTERERROR    1<<1

#define QEP_MODE_QUAD         0
#define QEP_MODE_DERECTION    1
#define QEP_MODE_UPCOUNT      2
#define QEP_MODE_DOWNCOUNT    3

#define QEP_SWAP_AB           (1<<8)

#define QEP_SETPOS_IMMED                  (1<<0)
#define QEP_SETPOS_INDEX_RISEEDGE         (2<<1)
#define QEP_SETPOS_INDEX_FALLEDGE         (3<<1)
#define QEP_SETPOS_STROBE_RISEEDGE        (2<<3)
#define QEP_SETPOS_STROBE_WISE            (3<<3)


#define QEP_INDEX_RISE_LATCH     1
#define QEP_INDEX_FALL_LATCH     2
#define QEP_INDEX_RISE_POS_INIT  2<<8
#define QEP_INDEX_FALL_POS_INIT  3<<8



#ifdef __cplusplus
extern "C" {
#endif

extern void QEPRegistHandler(unsigned int moduleId,void (*handler)(unsigned int flag));
extern int QEPGetVelocity(unsigned int moduleId);
extern void QEPVelocityDetectStart(unsigned int moduleId,unsigned int timeResolution_us);
extern void QEPInit(unsigned int moduleId,unsigned int inputmode,unsigned int intFlag);
extern void QEPSetPos(unsigned int moduleId, int pos,unsigned int setEvent);
extern void QEPSwapQuadInput(unsigned int moduleId);
extern void QEPSetPosFactor(unsigned int factor);
extern void QEPSetPosCompare(unsigned int moduleId, int compare);
extern int QEPReadPos(unsigned int moduleId);
extern unsigned int QEPReadDir(unsigned int moduleId);
extern int QEPCalcuLatchVelocity(unsigned int moduleId);//pulse*factor/s
extern void QEPSetPosCompareCurrent(unsigned int moduleId);
extern void  QEPSetIndex(unsigned int moduleId, unsigned int flag,bool indexPluseRevers,bool interrupt);
extern void  QEPSetStrobe(unsigned int moduleId,bool strobePluseRevers,bool interrupt);

#ifdef __cplusplus
#endif

#endif
