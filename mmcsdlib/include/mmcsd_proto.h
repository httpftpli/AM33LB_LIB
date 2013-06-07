/*
 * \file	mmcsd_proto.h
 *
 * \brief	MMC/SD definitions
 *
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#ifndef __MMCSD_PROTO_H__
#define __MMCSD_PROTO_H__
#ifdef __cplusplus
extern "C" {
#endif

#define BIT(x) (1 << x)

#define ARRYDIM(x)        ((sizeof (x))/(sizeof (x[0])))
#define BITFILD(x,y,z)         (((x) >> (y)) & (~((-1)<<((z)-(y)))))


/**
 * SD Card information structure
 */
#define MMCSD_CARD_SD		       (0u)
#define MMCSD_CARD_MMC		       (1u)
#define MMCSD_CARD_AUTO                (2u)

#define MMC_VERSION_4PX                (4)

#define MMCSD_ACCMODE_BYTE             (0)
#define MMCSD_ACCMODE_SECTOR           (1)

struct _mmcsdCtrlInfo;

/* Structure for SD Card information */
typedef struct _mmcsdCardInfo {
   struct _mmcsdCtrlInfo *ctrl;
   unsigned int cardType;
   unsigned int cardVer;
   unsigned int rca;
   unsigned int raw_scr[2];
   unsigned int raw_csd[4];
   unsigned int raw_cid[4];
   unsigned int raw_excsd[128];
   unsigned int ocr;
   unsigned int tranSpeed;
   unsigned int blkLen;
   unsigned int nBlks;
   unsigned long long size;
   unsigned int state;
   unsigned int busWidthSupport : 8 ;
   unsigned int busDdrSupport : 1;
   unsigned int busDdr : 1;
   unsigned int accmode : 1;
   unsigned int highCap : 1;
   unsigned int inited  : 1;
}mmcsdCardInfo; 

/* Structure for command */
typedef struct _mmcsdCmd {
   unsigned int idx;
   unsigned int flags;
   unsigned int arg;
   unsigned char *data;
   unsigned int blksize;
   unsigned int nblks;
   unsigned int rsp[4];
}mmcsdCmd; 

/* Structure for controller information */
typedef struct _mmcsdCtrlInfo {
   unsigned int moduleId;
   unsigned int memBase;
   unsigned int ipClk;
   unsigned int opClk;
   unsigned int (*cmdStatusGet)(struct _mmcsdCtrlInfo *ctrl);
   unsigned int (*xferStatusGet)(struct _mmcsdCtrlInfo *ctrl);
   void (*xferSetup)(struct _mmcsdCtrlInfo *ctrl, unsigned char rwFlag,
                     void *ptr, unsigned int blkSize, unsigned int nBlks);
   unsigned int busWidthSupport : 8;
   unsigned int dmaEnable : 1;
   unsigned int highspeed : 1;
   unsigned int busDdrSupport : 1;
   unsigned int busddr : 1;
   unsigned int inited : 1;
   unsigned int ocr;
   unsigned int cdPinNum;
   unsigned int wpPinNum;
   mmcsdCardInfo *card;
}mmcsdCtrlInfo; 

/* SD Commands enumeration */
#define SD_CMD(x)   (x)
#define MMC_CMD(x)  (x)

/* Command/Response flags for notifying some information to controller */
#define MMCSD_CMDFLAG_RSP_NONE			BIT(0)
#define MMCSD_CMDFLAG_TYPE_STOP			BIT(1)
#define MMCSD_CMDFLAG_TYPE_FUNCSEL		BIT(2)
#define MMCSD_CMDFLAG_TYPE_ABORT	        BIT(3)
#define MMCSD_CMDFLAG_RSP_BUSY			BIT(4)
#define MMCSD_CMDFLAG_RSP_136BITS		BIT(5)
#define MMCSD_CMDFLAG_DATA                      BIT(6)
#define MMCSD_CMDFLAG_DATA_RW_SHIFT               (7)
#define MMCSD_CMDFLAG_DATA_READ       ((1<<MMCSD_CMDFLAG_DATA_RW_SHIFT) | MMCSD_CMDFLAG_DATA)
#define MMCSD_CMDFLAG_DATA_WRITE       ((0<<MMCSD_CMDFLAG_DATA_RW_SHIFT) | MMCSD_CMDFLAG_DATA)




#define MMCSD_BUSWIDTH_1BIT             (BIT(0))
#define MMCSD_BUSWIDTH_4BIT             (BIT(3))
#define MMCSD_BUSWIDTH_8BIT             (BIT(7))




/* SD voltage enumeration as per VHS field of the interface command */
#define SD_VOLT_2P7_3P6                 (0x000100u)

/* SD OCR register definitions */
/* High capacity */
#define SD_OCR_HIGH_CAPACITY    	BIT(30)
/* Voltage */
#define SD_OCR_VDD_2P7_2P8		BIT(15)
#define SD_OCR_VDD_2P8_2P9		BIT(16)
#define SD_OCR_VDD_2P9_3P0		BIT(17)
#define SD_OCR_VDD_3P0_3P1		BIT(18)
#define SD_OCR_VDD_3P1_3P2		BIT(19)
#define SD_OCR_VDD_3P2_3P3		BIT(20)
#define SD_OCR_VDD_3P3_3P4		BIT(21)
#define SD_OCR_VDD_3P4_3P5		BIT(22)
#define SD_OCR_VDD_3P5_3P6		BIT(23)
/* This is for convenience only. Sets all the VDD fields */
#define SD_OCR_VDD_WILDCARD		(0x1FF << 15)

/* SD CSD register definitions */
#define SD_TRANSPEED_25MBPS		(0x32u)
#define SD_TRANSPEED_50MBPS		(0x5Au)

/* Check RCA/status */
#define SD_RCA_ADDR(rca)             ((rca & 0xFFFF0000) >> 16)
#define SD_RCA_STAT(rca)             (rca & 0x0xFFFF)

/* Check pattern that can be used for card response validation */
#define SD_CHECK_PATTERN   0xAA

/* SD SCR related macros */
#define SD_VERSION_1P0		0
#define SD_VERSION_1P1		1
#define SD_VERSION_2P0		2

#define MMCSD_STATE_TRANSFER    1
#define MMCSD_STATE_STANDBY     2
#define MMCSD_STATE_READY       3
#define MMCSD_STATE_IDENTIFY    4 
#define MMCSD_STATE_IDLE        0
  

/* Cacheline size */
#ifndef SOC_CACHELINE_SIZE
#define SOC_CACHELINE_SIZE         128
#endif

/* CM6 Swith mode arguments for High Speed */
#define SD_SWITCH_MODE        0x80FFFFFF
#define SD_CMD6_GRP1_SEL      0xFFFFFFF0
#define SD_CMD6_GRP1_HS       0x1

/*
 * Function prototypes
 */

extern unsigned int MMCSDP_Read(mmcsdCtrlInfo *ctrl, void *ptr, unsigned int block,
				                     unsigned int nblks);
extern unsigned int MMCSDP_Write(mmcsdCtrlInfo *ctrl, const void *ptr, unsigned int block,
				                       unsigned int nblks);
extern unsigned int MMCSDP_AppCmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c);
extern void MMCSDP_CtrlInfoInit(mmcsdCtrlInfo *ctrl,unsigned int moduleId,
                       unsigned int opClk,unsigned short busWidthSupport,
                       unsigned short ddrSupport, 
                       mmcsdCardInfo *card,
                       void preXferHook(mmcsdCtrlInfo *ctrl,unsigned char rwFlag,
                                      void *buf,unsigned int blksize,unsigned int nBlks),
                       unsigned int cmdstatusget(mmcsdCtrlInfo *ctrl),
                       unsigned int xferstatusget(mmcsdCtrlInfo *ctrl)
                       );

extern unsigned int MMCSDP_CmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c);
extern unsigned int MMCSDP_TranSpeedSet(mmcsdCtrlInfo *ctrl);
extern unsigned int MMCSDP_BusWidthSet(mmcsdCtrlInfo *ctrl);
extern unsigned int MMCSDP_StopCmdSend(mmcsdCtrlInfo *ctrl);
extern unsigned int MMCSDP_CardPresent(mmcsdCtrlInfo *ctrl);
extern unsigned int MMCSDP_CardReset(mmcsdCtrlInfo *ctrl);
extern unsigned int MMCSDP_CardInit(mmcsdCtrlInfo *ctrl,unsigned int cardType);
extern void MMCSDP_CtrlInit(mmcsdCtrlInfo *ctrl);
#ifdef __cplusplus
}
#endif

#endif
