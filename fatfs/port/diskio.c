/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2012        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
#include "mmcsd_proto.h"
#include "ff.h"
#include "debug.h"
#include "hw_types.h"
#include "rtc.h"
#include "platform.h"
#include "soc_AM335x.h"
#include "usblib.h"
#include "usbmsc.h"
#include "usbhost.h"
#include "usbhmsc.h"
#include "pf_platform_cfg.h"
#include "misc.h"

#define DRIVER_NUM_MMCSD0	   0
#define DRIVER_NUM_USB		   1
#define DRIVER_NUM_MMCSD1          2
#define DRIVER_NUM_DRVGUIDE        3



PARTITION VolToPart[] = {
    {DRIVER_NUM_MMCSD1, 1},     /* Logical drive 0 ==> Physical drive 0, 1st partition */
    {DRIVER_NUM_MMCSD1, 2},     /* Logical drive 1 ==> Physical drive 0, 2nd partition */
    {DRIVER_NUM_USB,    1},     /* Logical drive 2 ==> Physical drive 1 (auto detection) */
    {DRIVER_NUM_MMCSD0, 1},     /* Logical drive 3 ==> Physical drive 0, 3rd partition */    
};

extern tUSBHMSCInstance g_USBHMSCDevice[];
extern mmcsdCtrlInfo mmcsdctr[2];
static volatile DSTATUS USBStat = STA_NOINIT;    /* Disk status */

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv			/* Physical drive nmuber (0..) */
)
{
   unsigned int index;
   mmcsdCtrlInfo *ctrl;
   mmcsdCardInfo *card;
   switch (drv) {
   case DRIVER_NUM_MMCSD0:
   case DRIVER_NUM_MMCSD1:
      if (drv == DRIVER_NUM_MMCSD0) {
         index = 0;
      }else{
         index = 1;
      }
      ctrl =  mmcsdctr + index;
      card = ctrl->card; 

      if (ctrl->inited == 0) {
         mdError("mmcsd controller is not inited\r\n");
         return STA_NODISK;
      }
      if (card == NULL) {
         mdError("NO CARD\r\n");
         return STA_NODISK;
      }
      if (card->inited == 0) {
         if (!MMCSDP_CardInit(ctrl, MMCSD_CARD_AUTO)) {
            mdError("card init Failed\r\n");
            return STA_NODISK;
         }
      }
      if (card->cardType == MMCSD_CARD_MMC) {
         mdDebug("inand ");
      } else {
         mdDebug("sd card ");
      }
      mdDebug("inited \r\n ");
      mdDebug("version : ");
      mdDebugNum(card->cardVer);
      if (card->highCap) {
         mdDebug(" ,High Capacity\r\n");
      } else {
         mdDebug(" ,Low Capacity\r\n");
      }
      return 0; 

   case DRIVER_NUM_USB :
      {
         unsigned int ulMSCInstance;

         ulMSCInstance = (unsigned int)&g_USBHMSCDevice[USB_INSTANCE_FOR_USBDISK];

         /* Set the not initialized flag again. If all goes well and the disk is */
         /* present, this will be cleared at the end of the function.            */
         USBStat |= STA_NOINIT;

         /* Find out if drive is ready yet. */
         if (USBHMSCDriveReady(ulMSCInstance)) return (FR_NOT_READY);

         /* Clear the not init flag. */
         USBStat &= ~STA_NOINIT;
      }
      return 0;
   default:
      return STA_NOINIT;
   }
   return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status(
   BYTE drv        /* Physical drive nmuber (0..) */
   ) {
   unsigned int index;
   switch (drv) {
   case DRIVER_NUM_MMCSD0 :
   case DRIVER_NUM_MMCSD1 :
      {
         if (drv == DRIVER_NUM_MMCSD0) {
            index = 0;
         } else {
            index = 1;
         }

         mmcsdCardInfo *card = mmcsdctr[index].card;
         if (MMCSD_STATE_TRANSFER != card->state) {
            return STA_NOINIT;
         } else {
            return 0;
         }
         break;
      }
   case DRIVER_NUM_USB :
      return USBStat;

   default:
      break;
   }

   return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(
   BYTE drv,       /* Physical drive nmuber (0..) */
   BYTE *buff,     /* Data buffer to store read data */
   DWORD sector,   /* Sector address (LBA) */
   BYTE count      /* Number of sectors to read (1..128) */
   ) {
   int result;
   int index;
   switch (drv) {
   case DRIVER_NUM_MMCSD0 :
   case DRIVER_NUM_MMCSD1 :
      {
         if (drv == DRIVER_NUM_MMCSD0) {
            index = 0;
         } else {
            index = 1;
         }
         mmcsdCtrlInfo *ctrl =  mmcsdctr + index;
         result = MMCSDP_Read(ctrl, buff, sector, count);
         if (1 == result) {
            return RES_OK;
         } else {
            return  RES_ERROR;
         }
         break;
      }
   case DRIVER_NUM_USB :
      {
         unsigned int ulMSCInstance;
         ulMSCInstance = (unsigned int)&g_USBHMSCDevice[USB_INSTANCE_FOR_USBDISK];
         if (USBStat & STA_NOINIT) {
            return (RES_NOTRDY);
         }
         /* READ BLOCK */
         if (USBHMSCBlockRead(ulMSCInstance, sector, buff, count) == 0)
             return RES_OK;
      }
      return RES_ERROR;
   default:
      break;
   }
   return RES_PARERR; 
}


DWORD get_fattime(void) {
   unsigned char y, M, d, h, m, s;
   RTCRead(&y, &M, &d, &h, &m, &s);
   y = bcd2hex_2(y);
   M = bcd2hex_2(M);
   d = bcd2hex_2(d);
   h = bcd2hex_2(h);
   m = bcd2hex_2(m); 
   s = bcd2hex_2(s);
   return (y+2000-1980)<<25 | M<<21 | d<<16 | h<<11 | m<<5 | s;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE drv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	BYTE count			/* Number of sectors to write (1..128) */
)
{
   int result;
   unsigned index;
   switch (drv) {
   case DRIVER_NUM_MMCSD0 :
   case DRIVER_NUM_MMCSD1 :
      {
         if (drv == DRIVER_NUM_MMCSD0) {
            index = 0;
         } else {
            index = 1;
         }
         mmcsdCtrlInfo *ctrl =  mmcsdctr + index;
         result = MMCSDP_Write(ctrl, buff, sector, count);
         if (1 == result) {
            return RES_OK;
         } else {
            return RES_ERROR;
         }
      }
   case DRIVER_NUM_USB :
      {
         unsigned int ulMSCInstance;
         ulMSCInstance = (unsigned int)&g_USBHMSCDevice[USB_INSTANCE_FOR_USBDISK];
         if (USBStat & STA_NOINIT) return RES_NOTRDY;
         if (USBStat & STA_PROTECT) return RES_WRPRT;

         /* WRITE BLOCK */
         if (USBHMSCBlockWrite(ulMSCInstance, sector, (unsigned char *)buff,
                               count) == 0) return RES_OK;
      }
      return RES_ERROR; 
   default:
      break;
   }
   return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE ctrl,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{

   unsigned index;

   switch (drv) {
   case DRIVER_NUM_MMCSD0 :
   case DRIVER_NUM_MMCSD1 :
      {
         if (drv == DRIVER_NUM_MMCSD0) {
            index = 0;
         } else {
            index = 1;
         }
         mmcsdCardInfo *card  = mmcsdctr[index].card;
         switch (ctrl) {
         case CTRL_SYNC:
            return RES_OK;
         case GET_SECTOR_SIZE:
            *(unsigned short *)buff = 512;
            return RES_OK;
         case GET_SECTOR_COUNT:
            *(unsigned int *)buff =  card->nBlks;
            return RES_OK;
         case GET_BLOCK_SIZE:
            *(unsigned int *)buff = 1;  //TODO finish BLOCK_SIZE
            return RES_OK;
         case CTRL_ERASE_SECTOR:
            break;
         }
      }
      return RES_ERROR;

   case DRIVER_NUM_USB :
      if (USBStat & STA_NOINIT) {
         return RES_NOTRDY;
      }
      switch (ctrl){
      case CTRL_SYNC:
            return RES_OK;
         case GET_SECTOR_SIZE:
            *(unsigned short *)buff = 512;
            return RES_OK;
         case GET_SECTOR_COUNT:
            return RES_OK;
         case GET_BLOCK_SIZE:
            *(unsigned int *)buff = 1;  //TODO finish BLOCK_SIZE
            return RES_OK;
         case CTRL_ERASE_SECTOR:
            break; 
      }
   default:
      break;
   }     
	return RES_PARERR;
}
#endif
