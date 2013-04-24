#ifndef __PF_HS_MMCSD_H__
#define __PF_HS_MMCSD_H__



typedef struct{
   unsigned int fgCmdComp:1;
   unsigned int fgCmdTimeout:1;
   unsigned int fgDataTimeout:1;
   unsigned int fgXferComp:1;
   unsigned int bmCurError;
   unsigned int bmHisError;
} G_MMCSD;

extern volatile G_MMCSD g_Mmcsd[3];

extern void HSMMCSDModuleClkConfig(void);


#endif
