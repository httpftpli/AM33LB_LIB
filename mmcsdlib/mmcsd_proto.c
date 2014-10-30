/**
 *  \file   mmcsd_proto.c
 *
 *  \brief  MMCSD
 *
 *
 *
 */

/**
 * @addtogroup MMCSD
 * MMCSD document
 * @{
 */

#include "mmcsd_proto.h"
#include "string.h"
#include "uartStdio.h"
#include "cache.h"
#include "type.h"
#include "mmath.h"
#include "mmcsd_proto_hooks.h"
#include "module.h"
#include "debug.h"


extern void hsMmcSdBusWidthSet(mmcsdCtrlInfo *ctrl, unsigned char buswidth, BOOL ddr);
extern void hsMmcSdBusFreqSet(mmcsdCtrlInfo *ctrl, unsigned int busFrep);
extern void hsMmcSdInit(mmcsdCtrlInfo *ctrl);
extern BOOL hsMmcSdCardPresent(mmcsdCtrlInfo *ctrl);
extern unsigned int hsMmcSdCmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c);


#define DATA_RESPONSE_WIDTH       (SOC_CACHELINE_SIZE)
/*
// Cache size aligned data buffer (minimum of 64 bytes) for command response
#ifdef __TMS470__
#pragma DATA_ALIGN(dataBuffer, SOC_CACHELINE_SIZE);
static unsigned char dataBuffer[DATA_RESPONSE_WIDTH];

#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment = SOC_CACHELINE_SIZE
static unsigned char dataBuffer[DATA_RESPONSE_WIDTH];

#elif defined(gcc)
static unsigned char dataBuffer[DATA_RESPONSE_WIDTH]
__attribute__((aligned(SOC_CACHELINE_SIZE)));

#else
#error "Unsupported compiler\n\r"
#endif
*/

static  unsigned int __mmc_card_version(mmcsdCardInfo *card) {
   return (card->raw_csd[3] >> 26 & 0xf);  //bit 122-125
}

static  unsigned int __mmc_card_rdblklen(mmcsdCardInfo *card) {
   return 1 << (card->raw_csd[2] >> 16 & 0xf);   //bit 80-83
}


static  unsigned int __mmc_card_lowcap_nblks(mmcsdCardInfo *card) {
   unsigned int c_size = (card->raw_csd[1] >> 30 & 0x3) | (card->raw_csd[2] & 0x3ff) << 2; //bit 62-73
   unsigned int mult = card->raw_csd[1] >> 15 & 0x07; //bit 47-49
   return (c_size + 1) * (1 << (mult + 2));
}

static  unsigned int __mmc_card_hicap_sector_count(mmcsdCardInfo *card) {
   return *(unsigned int *)((unsigned char *)card->raw_excsd + 212);
}


/* Note card registers are big endian */

static inline unsigned char __sd_card_version(mmcsdCardInfo *card) {
   return card->raw_scr[0] & 0xF;
}

static inline unsigned int __sd_card_buswidth_spport(mmcsdCardInfo *card) {
   return (card->raw_scr[0] & 0xF00) >> 8;
}


static inline unsigned char __sd_card_csd_version(void *raw_csd) {
   unsigned int *csd = (unsigned int *)raw_csd;
   return (csd[3] & 0xC0000000) >> 30;
}

static inline unsigned int __sd_csd_dev_size(void *raw_csd) {
   unsigned int *csd = (unsigned int *)raw_csd;
   return ((csd[1] & 0xC0000000) >> 30) |
             ((csd[2] & 0x000003FF) << 2);

}

static inline unsigned short __sd_csd_v1_mult(void *raw_csd) {
   unsigned int *csd = (unsigned int *)raw_csd;
   return (csd[1] & 0x00038000) >> 15;
}

static inline unsigned short __sd_csd_rdblklen(void *raw_csd) {
   unsigned int *csd =  (unsigned int *)raw_csd;
   return (csd[2] & 0x000F0000) >> 16;
}

static inline unsigned char  __sd_csd_transpeed(void *raw_csd) {
   unsigned int *csd = (unsigned int *)raw_csd;
   return (csd[3] & 0x000000FF) >> 0;
}

static inline unsigned int __sd_card_numblk_v1(void *raw_csd) {
   unsigned  int *csd = (unsigned int *)raw_csd;
   return (__sd_csd_dev_size(csd) + 1) * (1 << (__sd_csd_v1_mult(csd) + 2));
}


static inline unsigned int __sd_csd_v2_dev_size(void *raw_csd) {
   unsigned  int *csd = (unsigned int *)raw_csd;
   return ((csd[2] & 0x0000003F) << 16) | ((csd[1] & 0xFFFF0000) >> 16);
}

static inline unsigned long long __sd_card_size_v2(void *raw_csd) {
   unsigned int *csd = (unsigned int *)raw_csd;
   return (__sd_csd_v2_dev_size(csd) + 1) * (512 * 1024);
}



static unsigned int  emmccardinit(mmcsdCtrlInfo *ctrl) {
   mmcsdCardInfo *card = ctrl->card;
   card->ocr = 0;
   card->inited = 0;
   unsigned int count = 2000;
   if (card->rca == 0) {
      card->rca = 1;
   }
   //unsigned int retry = 0xFFFF;
   unsigned int status = 0;
   mmcsdCmd cmd;

   /* Go ahead and send CMD1, with host capabilities to acquire OCR */
   cmd.idx = MMC_CMD(1);
   cmd.flags = 0;
   cmd.arg = 0; //if mmcscard arg = SD_OCR_HIGH_CAPACITY | SD_OCR_VDD_WILDCARD;
                //cmd1 is different from mmc card

   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (status == 0) {
      return 0;
   }

   while (count--) {
      cmd.arg = cmd.rsp[0] | 0x2<<29;
      status = hsMmcSdCmdSend(ctrl, &cmd);
      if(0 == status)
         return 0;
      if (cmd.rsp[0] & (1UL << 31)) {  //waite for MMC CARD  power on
         break;
      }
   }
   card->ocr = cmd.rsp[0];
   if ((card->ocr & (0x3UL << 29))== 0) {
      card->accmode = MMCSD_ACCMODE_BYTE;
   } else {
      card->accmode = MMCSD_ACCMODE_SECTOR;
   }
   //add m1 finish ////////


   if(-1==count)
     return 0;

   /* Send CMD2, to get the card identification register */
   cmd.idx = MMC_CMD(2);
   cmd.flags = MMCSD_CMDFLAG_RSP_136BITS;
   cmd.arg = 0;
   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (0 == status) {
      return 0;
   }
   memcpy(card->raw_cid, cmd.rsp, 16);


   /* Send CMD3, to assign  card relative address .card go to data transfer mode */
   cmd.idx = MMC_CMD(3);
   card->rca = 0x1234;
   cmd.flags = 0;
   cmd.arg = card->rca << 16;
   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (status == 0) {
      return 0;
   }

   /*finish card indentify mode  ***************************/

   /* Send CMD9, to get the card specific data(CSD) */
   cmd.idx = MMC_CMD(9);
   cmd.flags = MMCSD_CMDFLAG_RSP_136BITS;
   cmd.arg = card->rca << 16;
   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (status == 0) {
      return 0;
   }
   memcpy(card->raw_csd, cmd.rsp, 16);

   /* set DSR register if neccesery*/

   /* Select the card  turn card to transfer state*/
   cmd.idx = SD_CMD(7);
   cmd.flags = 0;
   cmd.arg = card->rca << 16;

   status = hsMmcSdCmdSend(ctrl, &cmd);

   if (status == 0) {
      return 0;
   }
   card->state = MMCSD_STATE_TRANSFER;

/*send cmd16 to set blocklen */
   cmd.idx = MMC_CMD(16);
   cmd.flags = 0;
   cmd.arg = 512;
   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (status == 0) {
      return 0;
   }

   /* Send CMD8, to get ext card specific data(EXT_CSD) */

   ctrl->xferSetup(ctrl, 1, card->raw_excsd, 512, 1);
   cmd.idx = MMC_CMD(8);
   cmd.flags = MMCSD_CMDFLAG_DATA_READ;
   cmd.arg = 0;
   cmd.blksize = 512;
   cmd.nblks = 1;

   /* clean the data cache. */
   CacheDataCleanBuff((unsigned int)(card->raw_excsd), cmd.blksize * cmd.nblks);//add by lfl

   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (status == 0) {
      return 0;
   }
   status = ctrl->xferStatusGet(ctrl);
   if (status == 0) {
      return 0;
   }
   /* Invalidate the data cache. */
   CacheDataInvalidateBuff((unsigned int)(card->raw_excsd), sizeof(card->raw_excsd));


   /* set card struct member according card register */
   card->highCap = (card->ocr & SD_OCR_HIGH_CAPACITY) ? 1 : 0;
   card->cardVer = __mmc_card_version(card);
   if (!card->highCap) {
      // card up to  2GB
      unsigned int blklen = __mmc_card_rdblklen(card);
      unsigned int nblks = __mmc_card_lowcap_nblks(card);
      card->size = blklen * nblks;
      card->blkLen =  512;
      card->nBlks = card->size / 512;
   } else {
      // card larger than 2GB
      card->blkLen = 512;
      card->nBlks = __mmc_card_hicap_sector_count(card);
      card->size = card->nBlks * 512;
   }
   card->busWidthSupport = (1 << 0) | (1 << 3) | (1 << 7);
   card->busDdrSupport = 1;
   if (card->cardVer == MMC_VERSION_4PX) {
      // MMC VERSION > 4   switch bus bit
      if (ctrl->opClk > 400000) {
         //send cmd6 with 0x03B90100 to switch hight speed mode rsp is r1b
         cmd.idx = MMC_CMD(6);
         cmd.flags = MMCSD_CMDFLAG_RSP_BUSY;
         cmd.arg = 0x03B90100;
         if (!hsMmcSdCmdSend(ctrl, &cmd)) return 0;
         hsMmcSdBusFreqSet(ctrl, ctrl->opClk);
         //ctrl->busFreqConfig(ctrl, ctrl->opClk);
      }
      MMCSDP_BusWidthSet(ctrl);

   }
   card->inited = 1;
   return 1;
}


static unsigned int sdcardinit(mmcsdCtrlInfo *ctrl) {
   /* CMD8 - send oper voltage */
   unsigned int status = 0;
   unsigned int retry = 0xFFFF;
   mmcsdCardInfo *card = ctrl->card;
   mmcsdCmd cmd;
   card->inited = 0;
   cmd.idx = SD_CMD(8);
   cmd.flags = 0;
   cmd.arg = (SD_CHECK_PATTERN | SD_VOLT_2P7_3P6);

   status = hsMmcSdCmdSend(ctrl, &cmd);

   if (status == 0) {
      /* If the cmd fails, it can be due to version < 2.0, since
       * we are currently supporting high voltage cards only
       */

      //TODO  finish support SD version < 2.0
   }

   /* Go ahead and send ACMD41, with host capabilities */
   cmd.idx = SD_CMD(41);
   cmd.flags = 0;
   cmd.arg = SD_OCR_HIGH_CAPACITY | SD_OCR_VDD_WILDCARD;

   status = MMCSDP_AppCmdSend(ctrl, &cmd);

   if (status == 0) {
      return 0;
   }

   /* Poll until we get the card status (BIT31 of OCR) is powered up */
   do {
      cmd.idx = SD_CMD(41);
      cmd.flags = 0;
      cmd.arg = SD_OCR_HIGH_CAPACITY | SD_OCR_VDD_WILDCARD;

      MMCSDP_AppCmdSend(ctrl, &cmd);

   }
   while (!(cmd.rsp[0] & ((unsigned int)BIT(31))) && retry--);

   if (retry == 0) {
      /* No point in continuing */
      return 0;
   }

   card->ocr = cmd.rsp[0];

   card->highCap = (card->ocr & SD_OCR_HIGH_CAPACITY) ? 1 : 0;
   card->accmode = card->highCap;

   /* Send CMD2, to get the card identification register */
   cmd.idx = SD_CMD(2);
   cmd.flags = MMCSD_CMDFLAG_RSP_136BITS;
   cmd.arg = 0;

   status = hsMmcSdCmdSend(ctrl, &cmd);

   memcpy(card->raw_cid, cmd.rsp, 16);

   if (status == 0) {
      return 0;
   }

   /* Send CMD3, to get the card relative address */
   cmd.idx = SD_CMD(3);
   cmd.flags = 0;
   cmd.arg = 0;

   status = hsMmcSdCmdSend(ctrl, &cmd);

   card->rca = SD_RCA_ADDR(cmd.rsp[0]);

   if (status == 0) {
      return 0;
   }

   /* Send CMD9, to get the card specific data */
   cmd.idx = SD_CMD(9);
   cmd.flags = MMCSD_CMDFLAG_RSP_136BITS;
   cmd.arg = card->rca << 16;

   status = hsMmcSdCmdSend(ctrl, &cmd);

   memcpy(card->raw_csd, cmd.rsp, 16);

   if (status == 0) {
      return 0;
   }
   card->tranSpeed = 25000000;
   if (__sd_csd_transpeed(card->raw_csd)==SD_TRANSPEED_50MBPS){
       card->tranSpeed = 50000000;
   }

   card->blkLen = 1 << (__sd_csd_rdblklen(card->raw_csd));

   if (__sd_card_csd_version(card->raw_csd)) {
      card->size = __sd_card_size_v2(card->raw_csd);
      card->nBlks = card->size / card->blkLen;
   } else {
      card->nBlks = __sd_card_numblk_v1(card->raw_csd);
      card->size = card->nBlks * card->blkLen;
   }

   /* Set data block length to 512 (for byte addressing cards) */
   if (!(card->highCap)) {
      cmd.idx = SD_CMD(16);
      cmd.flags = MMCSD_CMDFLAG_RSP_NONE;
      cmd.arg = 512;
      status = hsMmcSdCmdSend(ctrl, &cmd);

      if (status == 0) {
         return 0;
      } else {
         card->blkLen = 512;
      }
   }

   /* Select the card */
   cmd.idx = SD_CMD(7);
   cmd.flags = MMCSD_CMDFLAG_RSP_BUSY;
   cmd.arg = card->rca << 16;

   status = hsMmcSdCmdSend(ctrl, &cmd);

   if (status == 0) {
      return 0;
   }

   card->state = MMCSD_STATE_TRANSFER;
   /*
    * Send ACMD51, to get the SD Configuration register details.
    * Note, this needs data transfer (on data lines).
    */
   cmd.idx = SD_CMD(55);
   cmd.flags = 0;
   cmd.arg = card->rca << 16;

   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (status == 0) {
      return 0;
   }

   ctrl->xferSetup(ctrl, 1, card->raw_scr, 8, 1);//NOTICE: check mmu and cache aplay

   cmd.idx = SD_CMD(51);
   cmd.flags = MMCSD_CMDFLAG_DATA_READ;
   cmd.arg = card->rca << 16;
   cmd.nblks = 1;
   cmd.blksize =8;
   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (status == 0) {
      return 0;
   }

   status = ctrl->xferStatusGet(ctrl);

   if (status == 0) {
      return 0;
   }

   /* Invalidate the data cache. */
   CacheDataInvalidateBuff((unsigned int)card->raw_scr,8);//NOTICE: check mmu and cache aplay

   /*card->raw_scr[0] = (dataBuffer[3] << 24) | (dataBuffer[2] << 16) |\
      (dataBuffer[1] << 8) | (dataBuffer[0]);
   card->raw_scr[1] = (dataBuffer[7] << 24) | (dataBuffer[6] << 16) |\
      (dataBuffer[5] << 8) | (dataBuffer[4]);*/

   card->cardVer =  __sd_card_version(card);
   card->busWidthSupport = __sd_card_buswidth_spport(card);
   MMCSDP_BusWidthSet(ctrl);
   MMCSDP_TranSpeedSet(ctrl);
   card->inited = 1;
   return 1;
}

/*
 * \brief   This function determines the type of MMCSD card.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \returns  type of the MMCSD card
 *
 */
static unsigned int mmcsdcardtypecheck(mmcsdCtrlInfo *ctrl) {
   unsigned int status;
   mmcsdCmd cmd;

   /*
    * Card type can be found by sending CMD55. If the card responds,
    * it is a SD card. Else, we assume it is a MMC Card
    */

   cmd.idx = SD_CMD(55);
   cmd.flags = 0;
   cmd.arg = 0;
   status = MMCSDP_AppCmdSend(ctrl, &cmd);

   return status == 1 ? MMCSD_CARD_SD : MMCSD_CARD_MMC;
}

static unsigned int sdbuswidthset(mmcsdCtrlInfo *ctrl) {
   unsigned int status = 0;
   mmcsdCardInfo *card = ctrl->card;
   mmcsdCmd capp;
   capp.idx = SD_CMD(6);
   capp.arg = 0;  //1bit buswidth
   capp.flags = 0;

   if ((ctrl->busWidthSupport & MMCSD_BUSWIDTH_4BIT) &&
       (card->busWidthSupport & MMCSD_BUSWIDTH_4BIT)) {
      capp.arg = 2; //4bit buswidth
   }

   status = MMCSDP_AppCmdSend(ctrl, &capp);

   if (1 == status) {
      if (capp.arg == 0) {
         //ctrl->busWidthConfig(ctrl, 1,0);
          hsMmcSdBusWidthSet(ctrl,1,0);
      } else {
         //ctrl->busWidthConfig(ctrl, 4,0);
          hsMmcSdBusWidthSet(ctrl,4,0);
      }
   }
   return status;
}

static unsigned int mmcbuswidthset(mmcsdCtrlInfo *ctrl) {
   mmcsdCmd cmd;
   mmcsdCardInfo *card = ctrl->card;
   unsigned short buswidthsupport = (ctrl->busWidthSupport) & (card->busWidthSupport);
   unsigned short ddr = ctrl->busDdrSupport && card->busDdrSupport;

   if (card == NULL) {
      return 0;
   }
   if (card->state != MMCSD_STATE_TRANSFER) {
      return 0;
   }

   unsigned short  buswidth;
   if (buswidthsupport & MMCSD_BUSWIDTH_8BIT) {
      buswidth = 8;
   }else if(buswidthsupport & MMCSD_BUSWIDTH_4BIT ){
      buswidth = 4;
   }else{
      buswidth = 1;
   }
   if (0x01 == buswidth) {
      return 1;  //1bit buswidth
   }

   if (ddr) {
      (buswidth == 4)?
         (cmd.arg = 0x03B70500):(cmd.arg = 0x03B70600);  //4bit or 8bit
   }else{
      (buswidth == 4)?
         (cmd.arg = 0x03B70100):(cmd.arg = 0x03B70200);  //4bit or 8bit
   }

   //send cmd6  switch  bus width
   cmd.idx = MMC_CMD(6);
   cmd.flags = MMCSD_CMDFLAG_RSP_BUSY;

   if (!hsMmcSdCmdSend(ctrl, &cmd)) {
      return 0;
   }
   if(ddr) {
      card->busDdr = 1;
      ctrl->busddr = 1;
   }
   //ctrl->busWidthConfig(ctrl, buswidth,ddr);
   hsMmcSdBusWidthSet(ctrl,buswidth,ddr);
   return 1;
}


/**
 * @brief mmcsdCtrlInfo结构体初始化
 *  mmcsdCtrlInfo结构体代表主机控制器，首先需调用本函数初始化，然后调用
 *  MMCSDP_CtrlInit()完成控制器的初始化
 * @param [out] ctrl    mmcsdCtrlInfo结构体
 * @param [in]  moduleId 控制器moduleId  \b MODULE_ID_MMCSDX
 * @param [in]  opClk
 *        mmcsd的时钟频率，由mmcclk引脚输出
 * @param [in]  ddrSupport
 *        mmcsd双边沿支持，48mhz时不支持ddr
 * @param [in]  card
 *        mmcsdCardInfo结构体，该结构体代表卡
 * @param [in]  preXferHook
 *        发送数据前的回调函数，如果 \b NULL
 *        ,使用默认回调函数
 * @param [in]  cmdstatusget
 *        获取控制器命令信息回调函数，如果 \b
 *        NULL ,使用默认回调函数
 * @param [in]  xferstatusget
 *        获取控制器发送数据状态回调函数 如果 \b
 *        NULL ,使用默认回调函数
 *
 * @param [in]  busWidthSupport  总线宽度支持
 *        MMCSD_BUSWIDTH_8BIT  \b MMCSD_BUSWIDTH_4BIT
 *        \b MMCSD_BUSWIDTH_1BIT
 *
 * @return  void
 * @date    2013/5/4
 * @note
 * opClk = 48000000是 不支持ddr busWidthSupport不支持 \b
 * MMCSD_BUSWIDTH_8BIT
 * @see
 */
void  MMCSDP_CtrlInfoInit(mmcsdCtrlInfo *ctrl,unsigned int moduleId,
                       unsigned int opClk,unsigned short busWidthSupport,
                       unsigned short ddrSupport,
                       mmcsdCardInfo *card,
                       void preXferHook(mmcsdCtrlInfo *ctrl,unsigned char rwFlag,
                                      void *buf,unsigned int blksize,unsigned int nBlks),
                       unsigned int cmdstatusget(mmcsdCtrlInfo *ctrl),
                       unsigned int xferstatusget(mmcsdCtrlInfo *ctrl)
                       ){
   memset(ctrl,0,sizeof(*ctrl));
   ctrl->moduleId = moduleId;
   ctrl->memBase = modulelist[moduleId].baseAddr;
   ctrl->ipClk = modulelist[moduleId].moduleClk->fClk[0]->clockSpeedHz;
   ctrl->opClk = opClk;
   ctrl->busWidthSupport = busWidthSupport;
   ctrl->busDdrSupport = !!ddrSupport;
   ctrl->card = card;

   if (preXferHook == NULL) {
      preXferHook = HSMMCSDXferSetup;
   }
   ctrl->xferSetup = preXferHook;

   if (cmdstatusget == NULL) {
      cmdstatusget = HSMMCSDCmdStatusGet;
   }
   ctrl->cmdStatusGet = cmdstatusget;

   if (xferstatusget == NULL) {
      xferstatusget = HSMMCSDXferStatusGet;
   }
   ctrl->xferStatusGet = xferstatusget;
}

/*
 * \brief   This function sends the command to MMCSD.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \param    mmcsdCmd It determines the mmcsd cmd
 *
 * \return   status of the command.
 *
 */
unsigned int MMCSDP_CmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c) {
   return hsMmcSdCmdSend(ctrl, c);
}




/*
 * \brief   This function sends the application command to MMCSD.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \param    mmcsdCmd It determines the mmcsd cmd
 *
 * \return   status of the command.
 *
 */
unsigned int MMCSDP_AppCmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c) {
   unsigned int status = 0;
   mmcsdCmd capp;


   /* APP cmd should be preceeded by a CMD55 */
   capp.idx = SD_CMD(55);
   capp.flags = 0;
   capp.arg = ctrl->card->rca << 16;
   status = hsMmcSdCmdSend(ctrl, &capp);

   if (status == 0) {
      /* return safely, since we cannot continue if CMD55 fails */
      return 0;
   }

   status = hsMmcSdCmdSend(ctrl, c);

   return status;
}



/*
 * \brief   Configure the MMC/SD bus width
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \param   buswidth   SD/MMC bus width.\n
 *
 *
 * \return  None.
 *
 */
unsigned int MMCSDP_BusWidthSet(mmcsdCtrlInfo *ctrl) {
   mmcsdCardInfo *card = ctrl->card;

   if (card->cardType == MMCSD_CARD_SD) {
      return sdbuswidthset(ctrl);
   } else if (card->cardType == MMCSD_CARD_MMC) {
      return mmcbuswidthset(ctrl);
   }
   return 0;
}

/*
 * \brief    This function configures the transmission speed in MMCSD.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \returns  1 - successfull.
 *           0 - failed.
 */
unsigned int MMCSDP_TranSpeedSet(mmcsdCtrlInfo *ctrl) {
   mmcsdCardInfo *card = ctrl->card;
   unsigned int speed;
   unsigned int cmdStatus = 0;
   mmcsdCmd cmd;
   unsigned char dataBuffer[64];

   ctrl->xferSetup(ctrl, 1, dataBuffer, 64, 1);

   cmd.idx = SD_CMD(6);
   cmd.arg = ((SD_SWITCH_MODE & SD_CMD6_GRP1_SEL) | (SD_CMD6_GRP1_HS));
   cmd.flags = MMCSD_CMDFLAG_DATA_READ;
   cmd.nblks = 1;
   cmd.blksize = 64;
   //cmd.data = (unsigned char *)dataBuffer;

   /* clean the data cache. */
   CacheDataCleanBuff((unsigned int)(dataBuffer), cmd.blksize * cmd.nblks);//add by lfl

   cmdStatus = hsMmcSdCmdSend(ctrl, &cmd);

   if (cmdStatus == 0) {
      return 0;
   }

   cmdStatus = ctrl->xferStatusGet(ctrl);

   if (cmdStatus == 0) {
      return 0;
   }

   /* Invalidate the data cache. */
   CacheDataInvalidateBuff((unsigned int)dataBuffer, DATA_RESPONSE_WIDTH);

   if ((dataBuffer[16] & 0xF) == SD_CMD6_GRP1_HS) {
      card->tranSpeed = 50000000;
   } else {
      card->tranSpeed = 25000000;
   }

   //status = ctrl->busFreqConfig(ctrl, 50000000);
   speed = MIN(ctrl->opClk, card->tranSpeed);
   hsMmcSdBusFreqSet(ctrl, speed);
   ctrl->opClk = speed;
   return 1;
}

/*
 * \brief   This function resets the MMCSD card.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \returns  1 - successfull reset of card.
 *           0 - fails to reset the card.
 */
unsigned int MMCSDP_CardReset(mmcsdCtrlInfo *ctrl) {
   unsigned int status = 0;
   mmcsdCmd cmd;

   cmd.idx = SD_CMD(0);
   cmd.flags = MMCSD_CMDFLAG_RSP_NONE;
   /*cmd.arg = 0xF0F0F0F0;

   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (0 == status) {
      return status;
   }*/

   cmd.arg = 0;
   status = hsMmcSdCmdSend(ctrl, &cmd);
   return status;
}

/*
 * \brief   This function sends the stop command to MMCSD card.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \returns  1 - successfully sends stop command to card.
 *           0 - fails to send stop command to card.
 */
unsigned int MMCSDP_StopCmdSend(mmcsdCtrlInfo *ctrl) {
   unsigned int status = 0;
   mmcsdCmd cmd;

   cmd.idx  = SD_CMD(12);
   cmd.flags = MMCSD_CMDFLAG_TYPE_ABORT | MMCSD_CMDFLAG_RSP_BUSY;
   cmd.arg = 0;
   status = hsMmcSdCmdSend(ctrl, &cmd);
   return  status;
}


/**
 * \brief   This function intializes the mmcsdcontroller.
 *
 * \param   mmcsdCtrlInfo *ctrl It holds the mmcsd control
 *           information.
 *
 * \returns  NO
 **/
void  MMCSDP_CtrlInit(mmcsdCtrlInfo *ctrl) {
   if (ctrl->inited == 0) {
      hsMmcSdInit(ctrl);
   }
   ctrl->inited = 1;
}

/*
 * \brief   This function determines whether card is persent or not.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \returns  1 - Card is present.
 *           0 - Card is not present.
 */
unsigned int MMCSDP_CardPresent(mmcsdCtrlInfo *ctrl) {
   //return ctrl->cardPresent(ctrl);
   return hsMmcSdCardPresent(ctrl);
}



/**
 * \brief This function intializes the MMCSD Card.
 *
 * \param ctrl It holds the mmcsd control information.
 * \param cardType  card type ,the value can be
 * \b MMCSD_CARD_AUTO, \b MMCSD_CARD_MMC, \b MMCSD_CARD_SD
 *
 * \returns 1 - Intialization is successfull.
 *         0 - Intialization is failed.
 * \return
 *
 */
unsigned int MMCSDP_CardInit(mmcsdCtrlInfo *ctrl, unsigned int cardType) {

   mmcsdCardInfo *card = ctrl->card;
   unsigned int status = 0;

   memset(ctrl->card, 0, sizeof(mmcsdCardInfo));

   card->ctrl = ctrl;
   card->cardType = cardType;

   /* CMD0 - reset card */
   status = MMCSDP_CardReset(ctrl);

   if (status == 0) {
      return 0;
   }

   if (card->cardType == MMCSD_CARD_AUTO) {
      /* Returns 1 for a SD card, 0 for a non-SD card */
      card->cardType = mmcsdcardtypecheck(ctrl);
   }

   if (card->cardType == MMCSD_CARD_SD) {
     return  sdcardinit(ctrl);
   }
   return emmccardinit(ctrl);
}

/**
 * \brief   This function sends the write command to MMCSD card.
 *
 * \param    ctrl        It holds the mmcsd control information.
 * \param    ptr           It determines the address from where data has to written
 * \param    block         It determines to which block data to be written
 * \param    nblks         It determines the number of blocks to be written
 *
 * \return  int
 *        1 - successfull written of data
 *        0 - failure to write the data.
 * @note  1 bock = 512byte
 **/
unsigned int MMCSDP_Write(mmcsdCtrlInfo *ctrl, const void *ptr, unsigned int block,
                                 unsigned int nblks) {
   mmcsdCardInfo *card = ctrl->card;
   unsigned int status = 0;
   unsigned int address;
   mmcsdCmd cmd;

   /*
    * Address is in blks for high cap cards and in actual bytes
    * for standard capacity cards
    */
   if (!card->inited) {
      return 0;
   }
   if (card->accmode) {
      address = block;
   } else {
      address = block * card->blkLen;
   }

   if ((nblks > 1)&&(card->cardType==MMCSD_CARD_MMC)) {
      cmd.idx = MMC_CMD(23);
      cmd.arg = nblks;
      cmd.flags = 0;
      status = hsMmcSdCmdSend(ctrl, &cmd);
      if (status == 0) {
         return 0;
      }
   }
   /* Clean the data cache. */
   CacheDataCleanBuff((unsigned int)ptr, (512 * nblks));
   ctrl->xferSetup(ctrl, 0, (char *)ptr, 512, nblks);

   cmd.flags = MMCSD_CMDFLAG_DATA_WRITE | MMCSD_CMDFLAG_RSP_BUSY;
   cmd.arg = address;
   cmd.nblks = nblks;
   cmd.blksize = 512;

   if (nblks > 1) {
      cmd.idx = MMC_CMD(25);
      //cmd.flags |= MMCSD_CMDFLAG_TYPE_ABORT;
   } else {
      cmd.idx = SD_CMD(24);
   }

   status = hsMmcSdCmdSend(ctrl, &cmd);

   if (status == 0) {
      return 0;
   }

   status = ctrl->xferStatusGet(ctrl);

   if (status == 0) {
      return 0;
   }

   if ((nblks > 1)&&(card->cardType==MMCSD_CARD_SD)){
      cmd.idx = MMC_CMD(12);
      cmd.flags = MMCSD_CMDFLAG_RSP_BUSY | MMCSD_CMDFLAG_TYPE_ABORT;
      cmd.arg = 0;
      status = hsMmcSdCmdSend(ctrl, &cmd);
      if (status == 0) {
         return 0;
      }
   }
   return  status;
}

/**
 * \brief   This function sends the write command to MMCSD card.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 * \param    ptr           It determines the address to where data has to read
 * \param    block         It determines from which block data to be read
 * \param    nblks         It determines the number of blocks to be read
 *
 * \returns  1 - successfull reading of data.
 *           0 - failure to the data.
 **/
unsigned int MMCSDP_Read(mmcsdCtrlInfo *ctrl, void *ptr, unsigned int block,
                                unsigned int nblks) {
   mmcsdCardInfo *card = ctrl->card;
   unsigned int status = 0;
   unsigned int address;
   mmcsdCmd cmd;

   ASSERT(nblks>0);

   /*
    * Address is in blks for high cap cards and in actual bytes
    * for standard capacity cards
    */
   if (!card->inited) {
      return 0;
   }
   if (card->accmode) {
      address = block;
   } else {
      address = block * card->blkLen;
   }

   if ((nblks > 1)&&(card->cardType==MMCSD_CARD_MMC)) {
      cmd.idx = MMC_CMD(23);
      cmd.arg = nblks;
      cmd.flags = 0;
      status = hsMmcSdCmdSend(ctrl, &cmd);
      if (status == 0) {
         return 0;
      }
   }

   ctrl->xferSetup(ctrl, 1, ptr, 512, nblks);

   cmd.flags = MMCSD_CMDFLAG_DATA_READ;
   cmd.arg = address;
   cmd.nblks = nblks;
   cmd.blksize = 512;

   if (nblks > 1) {
      //cmd.flags |= MMCSD_CMDFLAG_TYPE_ABORT;
      cmd.idx = SD_CMD(18);
   } else {
      cmd.idx = SD_CMD(17);
   }
    /* clean the data cache. */
   if ((unsigned int)ptr&(SOC_CACHELINE_SIZE-1)) {
      CacheDataCleanBuff((unsigned int)ptr, (512 * nblks));//added by lfl
   }
   status = hsMmcSdCmdSend(ctrl, &cmd);
   if (status == 0) {
      return 0;
   }

   status = ctrl->xferStatusGet(ctrl);

   if (status == 0) {
      return 0;
   }

   if ((nblks > 1)&&(card->cardType==MMCSD_CARD_SD)){
      cmd.idx = MMC_CMD(12);
      cmd.flags = MMCSD_CMDFLAG_RSP_BUSY | MMCSD_CMDFLAG_TYPE_ABORT;
      cmd.arg = 0;
      status = hsMmcSdCmdSend(ctrl, &cmd);
      if (status == 0) {
         return 0;
      }
   }
   /* Invalidate the data cache. */
   CacheDataInvalidateBuff((unsigned int)ptr, (512 * nblks));

   return 1;
}

/**
 * @}
 */