#ifndef __PF_EDMA__H___
#define __PF_EDMA__H___


#include "edma.h"

typedef struct{
   unsigned int bmQdmaMissError;
   unsigned int bmDmaMissErrorL;
   unsigned int bmDmaMissErrorH;
   unsigned int bmEdmaCcError;
} G_EDMA;
  
extern volatile  G_EDMA g_Edma;

extern void EDMAModuleClkConfig(void);
extern void EDMA3AINTCConfigure();
extern unsigned int EDMARegisterHandler(unsigned int handlerIndex, void (*handler)(unsigned int tcc, unsigned int status));
unsigned int EDMARequestXferWithBufferEntry(unsigned int trigMode,
                                            unsigned int chNum,
                                            unsigned int entryAddr,
                                            unsigned int bufAddr,
                                            unsigned int rwFlag,
                                            unsigned int entryBitWidth,
                                            unsigned int blkSize,
                                            unsigned int nblks,
                                            unsigned int handlerIndex);
extern unsigned int EDMARequestXfer2D(unsigned int trigMode,
                           unsigned int chNum,
                           unsigned int scrAddr,
                           unsigned int dstAddr,
                           unsigned int bnOfScrRow,
                           unsigned int numOfScrColum,
                           unsigned int bnOfDstRow,
                           unsigned int numOfDstColum,
                           unsigned int bnOfRow,
                           unsigned int numOfColum,
                           unsigned int handlerIndex) ;
extern unsigned int EDMARequestXferArray(unsigned int trigMode,
                           unsigned int chNum,
                           unsigned int scrAddr,
                           unsigned int dstAddr,
                           unsigned int bytenumber,
                           unsigned int handlerIndex); 





#endif
