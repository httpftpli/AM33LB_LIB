#ifndef _PL_TSC_H
#define _PL_TSC_H


extern volatile unsigned int IsTSPress = 0;

extern unsigned int touchRelease = 0;

extern void TouchScreenInit(void);
extern void tsEnalbe(void);
extern void tsDisable(void);
extern void TouchCalibrate(void);
extern void TSCADCModuleClkConfig(void);




#endif
