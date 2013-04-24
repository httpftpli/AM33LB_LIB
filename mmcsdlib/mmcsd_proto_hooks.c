#include "edma.h"
#include "edma_event.h"
#include "pf_edma.h"
#include "hs_mmcsd.h"
#include "mmcsd_proto_hooks.h"
#include "pf_hs_mmcsd.h"


volatile unsigned  int mmcreadfinish = 0,mmcwritefinish = 0;


void HSMMCSDXferSetup(mmcsdCtrlInfo *ctrl, unsigned char rwFlag, void *ptr, 
                             unsigned int blkSize, unsigned int nBlks)
{
   unsigned int index = HSMMCSDBaseAddrToIndex(ctrl->memBase);
   unsigned int dmachannelr,dmachannelw;
   
   if (0 == index) {
      dmachannelr = EDMA3_CHA_MMCSD0_RX;
      dmachannelw = EDMA3_CHA_MMCSD0_TX;
   }else{
      dmachannelr = EDMA3_CHA_MMCSD1_RX;
      dmachannelw = EDMA3_CHA_MMCSD1_TX;
   }
   //g_Mmcsd[index].fgXferComp = 0;

   if (rwFlag == 1)
    {
        mmcreadfinish = 0;
        EDMARegisterHandler(dmachannelr,NULL);
        EDMARequestXferWithBufferEntry(EDMA3_TRIG_MODE_EVENT,dmachannelr,
                                       ctrl->memBase + MMCHS_DATA,
                                       (unsigned int)ptr,1,32,blkSize,
                                       nBlks,dmachannelr);
    }
    else
    {
        mmcwritefinish = 0;
        EDMARegisterHandler(dmachannelw,NULL);
        EDMARequestXferWithBufferEntry(EDMA3_TRIG_MODE_EVENT,dmachannelw,
                                       ctrl->memBase + MMCHS_DATA,(unsigned int)ptr,
                                       0,32,blkSize,nBlks,dmachannelw);
    }
    ctrl->dmaEnable = 1;                                           
}


/*
 * Check command status.will block
 */
unsigned int HSMMCSDCmdStatusGet(mmcsdCtrlInfo *ctrl) {

   unsigned int index = HSMMCSDBaseAddrToIndex(ctrl->memBase);
   
   while ((g_Mmcsd[index].fgCmdComp == 0) && (g_Mmcsd[index].fgCmdTimeout == 0));
   if (1 == g_Mmcsd[index].fgCmdComp) {
      g_Mmcsd[index].fgCmdComp = 0;
      return 1;
   }
   if (1 == g_Mmcsd[index].fgCmdTimeout) {
      g_Mmcsd[index].fgCmdTimeout = 0;
      return 0;
   }
   return 0;
}


/*
 * Check data transfer finish .will block
 */
unsigned int HSMMCSDXferStatusGet(mmcsdCtrlInfo *ctrl)
{
    unsigned int index = HSMMCSDBaseAddrToIndex(ctrl->memBase);
    while ((g_Mmcsd[index].fgXferComp == 0)&&(g_Mmcsd[index].fgDataTimeout == 0));
    if (g_Mmcsd[index].fgXferComp == 1) {
       g_Mmcsd[index].fgXferComp = 0; 
       return 1;
    } 
    if (g_Mmcsd[index].fgDataTimeout ==1) {
       g_Mmcsd[index].fgDataTimeout = 0;
       return 0;
    }     
    return 0;
}

