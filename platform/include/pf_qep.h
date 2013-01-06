#ifndef _PF_QEP_H_
#define _PF_QEP_H_



#define QEP_HANDER_FlAG_COMPARE_MATCH     1
#define QEP_HANDER_FlAG_PHASE_ERROR       2
#define QEP_HANDER_FlAG_POSCNT_ERROR      3
#define QEP_HANDER_FlAG_POSCNT_OVERFLOW   4
#define QEP_HANDER_FlAG_POSCNT_UNDERFLOW  5


#ifdef __cplusplus
extern "C" {
#endif

extern void QEPRegistHandler(unsigned int baseAddr,void (*handler)(unsigned int flag));
extern int QEPGetVelocity(unsigned int baseAddr);

#ifdef __cplusplus
#endif

#endif
