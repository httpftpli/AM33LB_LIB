/**
 * \file  edma.c
 *
 * \brief Platform related APIs for EDMA
 * @addtogroup EDMA
 * @brief EDMA
 *
 * \#include  "pf_edma.h"
 * @{
 */


#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "edma.h"
#include "interrupt.h"
#include "debug.h"
#include "pf_edma.h"
#include "module.h"


/* EDMA instance related macros. */
#define  EDMA_INST_BASE            (SOC_EDMA30CC_0_REGS)
#define  EDMA_COMPLTN_INT_NUM      (SYS_INT_EDMACOMPINT)
#define  EDMA_ERROR_INT_NUM        (SYS_INT_EDMAERRINT)

/* EDMA callback function array */

volatile  G_EDMA g_Edma;
void (*dma_Fxn[EDMA3_NUM_TCC])(unsigned int tcc, unsigned int status);


static int evtqueue = -1;



inline static unsigned int numberToFwid(unsigned int val){
   switch (val) {
   case 8:return 0;
   case 16:return 1;
   case 32:return 2;
   case 64:return 3;
   case 128:return 4;
   case 256:return 5;
   }
   return 0;
}

void isr_Edma3Completion(unsigned int intnum) {
   UNUSED(intnum);
   unsigned int pendingIrqsL, pendingIrqsH;
   pendingIrqsL = EDMA3GetIntrStatus(EDMA_INST_BASE);
   pendingIrqsH = EDMA3IntrStatusHighGet(EDMA_INST_BASE);

   /*shift and test status bit ,if set call hook function that registed by user*/
   if (pendingIrqsL) {
      for (int i = 0; i < 32; i++) {
         if (pendingIrqsL & (1ul << i)) {
            if (dma_Fxn[i] != NULL) {
               (*dma_Fxn[i])(i, EDMA3_XFER_COMPLETE);
            }
            EDMA3ClrIntr(EDMA_INST_BASE, i);
            pendingIrqsL &= ~(1ul << i);
            if (!pendingIrqsL) break;
         }
      }
   }
   if (pendingIrqsH) {
      for (int i = 0; i < 32; i++) {
         if (pendingIrqsH & (1ul << i)) {
            if (dma_Fxn[i + 32] != NULL) {
               (*dma_Fxn[i + 32])(i + 32, EDMA3_XFER_COMPLETE);
            }
            EDMA3ClrIntr(EDMA_INST_BASE, i + 32);
            pendingIrqsH &= ~(1ul << i);
            if (!pendingIrqsH) break;
         }
      }
   }
   /*revaluate interrupt bit ,retrig interrupt if interrupt state bit set*/
   pendingIrqsL = EDMA3GetIntrStatus(EDMA_INST_BASE);
   pendingIrqsH = EDMA3IntrStatusHighGet(EDMA_INST_BASE);
   if (pendingIrqsL || pendingIrqsH) {
      EDMA3ErrIntrTrig(EDMA_INST_BASE);
   }
}



void isr_Edma3CCError(unsigned int intnum) {
   UNUSED(intnum);
   unsigned int qdmamisserror, dmamisserror_L, dmamisserror_H, ccerror;

   qdmamisserror = EDMA3QdmaGetErrIntrStatus(EDMA_INST_BASE);
   dmamisserror_L = EDMA3GetErrIntrStatus(EDMA_INST_BASE);
   dmamisserror_H = EDMA3ErrIntrHighStatusGet(EDMA_INST_BASE);
   ccerror  = EDMA3GetCCErrStatus(EDMA_INST_BASE);

   /*set globle error bit*/
   g_Edma.bmQdmaMissError |= qdmamisserror;
   g_Edma.bmDmaMissErrorL |= dmamisserror_L;
   g_Edma.bmDmaMissErrorH |= dmamisserror_H;
   g_Edma.bmEdmaCcError |= ccerror;

   /*if error bit set,clear conrrisponding error bit*/
   if (qdmamisserror) {
      EDMA3QdmaClrMissEvt_all(EDMA_INST_BASE, qdmamisserror);
   }
   if (dmamisserror_L) {
      EDMA3ClrMissEvt_L(EDMA_INST_BASE, dmamisserror_L);
   }
   if (dmamisserror_H) {
      EDMA3ClrMissEvt_H(EDMA_INST_BASE, dmamisserror_H);
   }
   if (ccerror) {
      EDMA3ClrCCErr(EDMA_INST_BASE, ccerror);
   }

   /*revalue the error bit, if have new error bit trig ERROR intr*/
   qdmamisserror = EDMA3QdmaGetErrIntrStatus(EDMA_INST_BASE);
   dmamisserror_L = EDMA3GetErrIntrStatus(EDMA_INST_BASE);
   dmamisserror_H = EDMA3ErrIntrHighStatusGet(EDMA_INST_BASE);
   ccerror  = EDMA3GetCCErrStatus(EDMA_INST_BASE);
   if (qdmamisserror || dmamisserror_L || dmamisserror_H || ccerror) {
      EDMA3ErrIntrTrig(EDMA_INST_BASE);
   }
}




unsigned int EDMARegisterHandler(unsigned int handlerIndex, void (*handler)(unsigned int tcc, unsigned int status)){
   if (handlerIndex >= 64) {
      return 0;
   }else{
      dma_Fxn[handlerIndex] = handler;
   }
   return 1;
}


unsigned int EDMARequestXferArray(unsigned int trigMode,
                           unsigned int chNum,
                           unsigned int scrAddr,
                           unsigned int dstAddr,
                           unsigned int bytenumber,
                           unsigned int handleIndex) {
   unsigned int synctype ,paramid,chtype;
   volatile EDMA3CCPaRAMEntry *paramSet;

   ASSERT(handleIndex < 64);
   //scrAddr and destAddr shoud 4bit allign;
   ASSERT((scrAddr&0x3)==0 && (dstAddr&0x3)==0 );

   if (EDMA3_0_NUM_TC == ++evtqueue) {
      evtqueue = 0;
   }

   if (EDMA3_TRIG_MODE_QDMA == trigMode){
      chtype = EDMA3_CHANNEL_TYPE_QDMA;
      paramid = 64 + chNum % 8;
   }else{
      chtype = EDMA3_CHANNEL_TYPE_DMA;
      paramid  = chNum;
   }

   EDMA3MapChToEvtQ(EDMA_INST_BASE,chtype,chNum,evtqueue);

   paramSet = (EDMA3CCPaRAMEntry *)(EDMA_INST_BASE + EDMA3CC_OPT(paramid));

   if (bytenumber <= 0xffff){
      synctype = 0;
   } else {
      synctype = 1;
   }
   if ((1==synctype)&&((bytenumber & 0x3ff)!=0)) {
      return 0; //if bytenumber>0xffff, bytenumber should be multiple of 1k;
   }

   //if QDMA disable the channel to aviod trig when set paRam
   if (EDMA3_TRIG_MODE_IMMEDIATE == trigMode) {
      EDMA3DisableTransfer(EDMA_INST_BASE, chNum, EDMA3_TRIG_MODE_IMMEDIATE);
   }

   paramSet->srcAddr    = (unsigned int)scrAddr;
   paramSet->dstAddr   = dstAddr;
   if (0==synctype) {
       paramSet->aCnt      = bytenumber;
       paramSet->bCnt      = 1;
       paramSet->cCnt      = 1;
       paramSet->opt       = (handleIndex<<12)|(2 << 8)|(1<<20)|(1<<3); // set STATIC BIT,A SYNC
       paramSet->srcBIdx   = 0;
       paramSet->destBIdx   = 0;
   }else{
      paramSet->aCnt       = 1024 ;
      paramSet->bCnt       = bytenumber/1024;
      paramSet->cCnt       = 1;
      paramSet->opt        = (handleIndex<<12)|(2 << 8)|(1<<20)|(1<<3)|(1<<2); //set STATIC BIT, A-B SYNC,ENABLE INT,32BIT WIDTH
      paramSet->srcBIdx    = 1024;
      paramSet->destBIdx   = 1024;
   }

   paramSet->srcCIdx    = 0;
   paramSet->destCIdx   = 0;

   paramSet->bCntReload = 0x0;
   paramSet->linkAddr   = 0xffff;

   EDMA3EnableTransfer(EDMA_INST_BASE, chNum, trigMode);

   if (EDMA3_TRIG_MODE_IMMEDIATE == trigMode) {
      EDMA3SetQdmaTrigWord(EDMA_INST_BASE,chNum,0);
      paramSet->opt  = paramSet->opt; //trig qdma
   }
   return 1;
}


unsigned int EDMARequestXfer2D( unsigned int trigMode,
                           unsigned int chNum,
                           unsigned int scrAddr,
                           unsigned int dstAddr,
                           unsigned int bnOfScrRow,
                           unsigned int bnOfDstRow,
                           unsigned int bnOfRow,
                           unsigned int numOfColum,
                           unsigned int handlerIndex) {

   unsigned int paramid,chtype;

   ASSERT(handlerIndex < 64);
   //scrAddr and destAddr shoud 4bit allign;
   //ASSERT(((scrAddr&0x3) == 0) && ((dstAddr&0x3) == 0));

   if (EDMA3_0_NUM_TC==++evtqueue) {
      evtqueue = 0;
   }

   if (EDMA3_TRIG_MODE_QDMA == trigMode) {
      chtype = EDMA3_CHANNEL_TYPE_QDMA;
      paramid = 64+chNum % 8;
   } else {
      chtype = EDMA3_CHANNEL_TYPE_DMA;
      paramid = chNum;
   }

   EDMA3MapChToEvtQ(EDMA_INST_BASE,chtype,chNum,evtqueue);

   // QDMA disable the channel to aviod trig when set paRam
   if (EDMA3_TRIG_MODE_IMMEDIATE == trigMode) {
      EDMA3DisableTransfer(EDMA_INST_BASE, chNum, EDMA3_TRIG_MODE_IMMEDIATE);
   }

   EDMA3CCPaRAMEntry *paramSet = (EDMA3CCPaRAMEntry *)(EDMA_INST_BASE + EDMA3CC_OPT(paramid));

   paramSet->srcAddr    = (unsigned int)scrAddr;
   paramSet->dstAddr   = dstAddr;

   paramSet->aCnt       = bnOfRow ;
   paramSet->bCnt       = numOfColum;
   paramSet->cCnt       = 1;
   paramSet->opt        = (handlerIndex<<12)|(2 << 8)|(1<<20)|(1<<3)|(1<<2); //set STATIC BIT, A-B SYNC,32BIT WIDTH


   paramSet->srcBIdx    = bnOfScrRow;
   paramSet->srcCIdx    = 0;
   paramSet->destBIdx   = bnOfDstRow;
   paramSet->destCIdx   = 0;
   paramSet->bCntReload = 0x0;
   paramSet->linkAddr   = 0xffff;
   EDMA3EnableTransfer(EDMA_INST_BASE, chNum, trigMode);
   if (EDMA3_TRIG_MODE_IMMEDIATE == trigMode) {
      EDMA3SetQdmaTrigWord(EDMA_INST_BASE, chNum, 0);
      paramSet->opt  = paramSet->opt; //trig qdma
   }
   return 1;
}





/**
 * @brief
 *        transfer data betwin fix address and consecutive memey
 *        eara
 * @param [in] trigMode DMA trig mode
 * -- EDMA3_TRIG_MODE_MANUAL    manual trig
 * -- EDMA3_TRIG_MODE_EVENT     event trig 
 * -- EDMA3_TRIG_MODE_IMMEDIATE  immediate trig
 * @param [in] chNum   DMA chanel \b EDMA3_CHA_XXX
 * @param [in] entryAddr fix address
 * @param [in] bufAddr   consecutive memey first address
 * @param [in] rwFlag   read or write
 * -- 1 fix address to consecutive memey
 * -- 0 consecutive memey to fix address
 * @param [in] entryBitWidth, should be 8,16,32
 * @param [in] blkSize    byte number every transfer 
 * @param [in] nblks  transfer times
 * @param [in] handlerIndex handler index number when transfer 
 *        finish
 * @return  always 1
 * @date    2013/6/19
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
unsigned int EDMARequestXferWithBufferEntry(unsigned int trigMode,
                                            unsigned int chNum,
                                            unsigned int entryAddr,
                                            unsigned int bufAddr,
                                            unsigned int rwFlag,
                                            unsigned int entryBitWidth,
                                            unsigned int blkSize,
                                            unsigned int nblks,
                                            unsigned int handlerIndex) {
   unsigned int paramid, chtype;
   volatile EDMA3CCPaRAMEntry *paramSet;

   ASSERT(handlerIndex < 64);
   //entryAddr should 5bit allign;
   ASSERT((entryAddr&0x1fUL) == 0);
   ASSERT((entryBitWidth == 8) || (entryBitWidth == 16) || (entryBitWidth == 32));
   ASSERT(blkSize%(entryBitWidth/8)==0);

   if (EDMA3_0_NUM_TC == ++evtqueue) {
      evtqueue = 0;
   }

   if (EDMA3_TRIG_MODE_QDMA == trigMode) {
      chtype = EDMA3_CHANNEL_TYPE_QDMA;
      paramid = 64+chNum % 8;
   } else {
      chtype = EDMA3_CHANNEL_TYPE_DMA;
      paramid = chNum;
   }

   EDMA3MapChToEvtQ(EDMA_INST_BASE, chtype, chNum, evtqueue);

   /*if QDMA disable the channel to aviod trig when set paRam*/
   if (EDMA3_TRIG_MODE_IMMEDIATE == trigMode) {
      EDMA3DisableTransfer(EDMA_INST_BASE, chNum, EDMA3_TRIG_MODE_IMMEDIATE);
   }

   paramSet = (EDMA3CCPaRAMEntry *)(EDMA_INST_BASE + EDMA3CC_OPT(paramid));

   paramSet->srcAddr    = rwFlag ? entryAddr : bufAddr;
   paramSet->dstAddr    = rwFlag ? bufAddr:  entryAddr;

   unsigned int nbyte = entryBitWidth / 8;
   paramSet->aCnt       = nbyte;
   paramSet->bCnt       = (unsigned short)blkSize / nbyte;
   paramSet->cCnt       = nblks;
   paramSet->opt        = (handlerIndex << 12) |(numberToFwid(entryBitWidth) << 8) |
         (1 << 20) | (1 << !rwFlag) |(1 << 2); // A-B SYNC,ENABLE INT,32BIT WIDTH

   paramSet->srcBIdx    = rwFlag ? 0 : nbyte;
   paramSet->srcCIdx    = rwFlag ? 0 : (unsigned short)blkSize;
   paramSet->destBIdx   = rwFlag ? nbyte : 0;
   paramSet->destCIdx   = rwFlag ? (unsigned short)blkSize : 0;
   paramSet->bCntReload = 0x0;
   paramSet->linkAddr   = 0xffff;

   EDMA3EnableTransfer(EDMA_INST_BASE, chNum, trigMode);

   if (EDMA3_TRIG_MODE_IMMEDIATE == trigMode) {
      EDMA3SetQdmaTrigWord(EDMA_INST_BASE, chNum, 0);
      paramSet->opt  = paramSet->opt; //trig qdma
   }

   return 1;
}





/**
 *  \brief    This function maps the crossbar events.
 *
 *  \param    baseAdd         It is the Control Module Address.
 *
 *  \param    crossBarEvent   It is the crossBar event number.
 *
 *  \param    Channel         It is the channel number to which cross bar
 *                            event needs to be mapped.
 */

unsigned int EDMA3CrossBarChannelMap(unsigned int baseAdd, unsigned int crossBarEvent,
                                     unsigned int Channel)
{

    unsigned int offset;
    unsigned int select;
    unsigned int n = 0;

    /* offset of the TPCC_MUX to be configured */
    offset = Channel / 4;

    /*
    ** Each TPCC_MUX register has four event mux which can be used for
    ** cross bar mapping.Thus "select" variable is used to select,
    ** which of the event mux out of four,for a given TPCC_MUX register
    ** to be used.
    */
    select = Channel - offset * 4;

    switch(select)
    {
         case 0:
              n = 0;
         break;

         case 1:
              n = 8;
         break;

         case 2:
              n = 16;
         break;

         case 3:
              n = 24;
         break;

         default:
         break;
    }

    /* 'n' specifies the offset of the event mux */
    HWREG(baseAdd + TPCC_MUX(offset)) &= ~(crossBarEvent << n);

    HWREG(baseAdd + TPCC_MUX(offset)) |= crossBarEvent << n;

    return 0;
}


 /**
 * \brief  This API returns a unique number which identifies itself
 *         with the EDMA IP in AM335x SoC.
 * \param  None
 * \return This returns a number '2' which is unique to EDMA IP in AM335x.
 */
unsigned int EDMAVersionGet(void)
{
    return 2;
}

/*
** This function enables the system L3 clocks.
** This also enables the clocks for EDMA instance.
*/

void EDMAModuleClkConfig(void)
{
    /* Configuring L3 Interface Clocks. */

    /* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
          CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
          CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
          CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
          CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
           CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));

    /* Checking fields for necessary values.  */

    /* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
    while((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
           CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
    ** attain the desired value.
    */
    while(CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
    ** register to attain the desired value.
    */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
          CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    /* Configuring clocks for EDMA3 TPCC and TPTCs. */

    /* Writing to MODULEMODE field of CM_PER_TPCC_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_TPCC_CLKCTRL) |=
          CM_PER_TPCC_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_TPCC_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_TPCC_CLKCTRL) &
          CM_PER_TPCC_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_TPTC0_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_TPTC0_CLKCTRL) |=
          CM_PER_TPTC0_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_TPTC0_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC0_CLKCTRL) &
           CM_PER_TPTC0_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_TPTC1_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_TPTC1_CLKCTRL) |=
          CM_PER_TPTC1_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_TPTC1_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC1_CLKCTRL) &
           CM_PER_TPTC1_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_TPTC2_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_TPTC2_CLKCTRL) |=
          CM_PER_TPTC2_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_TPTC2_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC2_CLKCTRL) &
           CM_PER_TPTC2_CLKCTRL_MODULEMODE));

    /*
    ** Waiting for IDLEST field in CM_PER_TPCC_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPCC_CLKCTRL_IDLEST_FUNC <<
           CM_PER_TPCC_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPCC_CLKCTRL) &
            CM_PER_TPCC_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_TPTC0_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC0_CLKCTRL_IDLEST_FUNC <<
           CM_PER_TPTC0_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC0_CLKCTRL) &
            CM_PER_TPTC0_CLKCTRL_IDLEST));

    /*
    ** Waiting for STBYST field in CM_PER_TPTC0_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC0_CLKCTRL_STBYST_FUNC <<
           CM_PER_TPTC0_CLKCTRL_STBYST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC0_CLKCTRL) &
            CM_PER_TPTC0_CLKCTRL_STBYST));

    /*
    ** Waiting for IDLEST field in CM_PER_TPTC1_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC1_CLKCTRL_IDLEST_FUNC <<
           CM_PER_TPTC1_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC1_CLKCTRL) &
            CM_PER_TPTC1_CLKCTRL_IDLEST));

    /*
    ** Waiting for STBYST field in CM_PER_TPTC1_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC1_CLKCTRL_STBYST_FUNC <<
           CM_PER_TPTC1_CLKCTRL_STBYST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC1_CLKCTRL) &
            CM_PER_TPTC1_CLKCTRL_STBYST));

    /*
    ** Waiting for IDLEST field in CM_PER_TPTC2_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC2_CLKCTRL_IDLEST_FUNC <<
           CM_PER_TPTC2_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC2_CLKCTRL) &
            CM_PER_TPTC2_CLKCTRL_IDLEST));

    /*
    ** Waiting for STBYST field in CM_PER_TPTC2_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_TPTC2_CLKCTRL_STBYST_FUNC <<
           CM_PER_TPTC2_CLKCTRL_STBYST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_TPTC2_CLKCTRL) &
            CM_PER_TPTC2_CLKCTRL_STBYST));

}



void EDMAInit(){
   EDMAModuleClkConfig();
   EDMA3Init(SOC_EDMA30CC_0_REGS);
   //enable 64 edma interrupt
   for (int i=0;i<64;i++) {
      EDMA3EnableEvtIntr(SOC_EDMA30CC_0_REGS,i);
   }
   moduleIntConfigure(MODULE_ID_EDMA);
}

/**
 * @}
 */


