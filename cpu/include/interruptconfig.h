#ifndef __interruptconfig_h__
#define __interruptconfig_h__

#define INT_PRIORITY_MAX          (0x7f)
#define INT_PRIORITY_MIN           (0)
#define INT_PRIORITY_USER_NORMAL  (0x30)
#define INT_PRIORITY_USER_PRI     (0x10)
#define INT_PRIORITY_DMA_COMP     (INT_PRIORITY_USER_NORMAL - 3)
#define INT_PRIORITY_DMA_ERROR    (INT_PRIORITY_USER_NORMAL - 4) 
#define INT_PRIORITY_MMCSD0       (INT_PRIORITY_USER_NORMAL-2)
#define INT_PRIORITY_MMCSD1       (INT_PRIORITY_USER_NORMAL-1)
#define INT_PRIORITY_RTC          (INT_PRIORITY_USER_NORMAL-5)


#endif
