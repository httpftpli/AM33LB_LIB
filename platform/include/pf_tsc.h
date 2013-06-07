#ifndef _PL_TSC_H
#define _PL_TSC_H
#include "atomic.h"

typedef struct ts_sample {
    int       x;
    int       y;
    int timetick;
} TS_SAMPLE;

extern volatile TS_SAMPLE ts;
extern atomic touched;
extern void TouchScreenInit();
extern void tsEnalbe(void);
extern void tsDisable(void);
extern BOOL TouchCalibrate(void);
extern void TSCADCModuleClkConfig(void);



#endif
