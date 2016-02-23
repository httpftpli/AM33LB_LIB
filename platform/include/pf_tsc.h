#ifndef _PL_TSC_H
#define _PL_TSC_H
#include "atomic.h"

extern void TouchScreenInit();
extern void  TouchScreenDeInit();
extern bool TouchScreenTsPadDetect();
extern void tsEnalbe(void);
extern void tsDisable(void);
//extern void TSCADCModuleClkConfig(void);

#endif
