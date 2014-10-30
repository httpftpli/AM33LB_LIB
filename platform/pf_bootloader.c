#include "pf_bootloader.h"
#include "algorithm.h"
#include "stdbool.h"
#include "stddef.h"
#include "mmcsd_proto.h"
#include <string.h>
#include "debug.h"


extern mmcsdCtrlInfo mmcsdctr[2];

#pragma data_alignment = 4
const unsigned char ProgramTable[256] =
{
    0xD5, 0xFD, 0xC3, 0xB6, 0xBE, 0xD9, 0x55, 0x53, 0x42, 0xB4, 0xC5, 0xC5, 0xCC, 0x2E, 0x2E, 0x2E,
    0x55, 0x53, 0x42, 0x20, 0x45, 0x6E, 0x75, 0x6D, 0x65, 0x72, 0x61, 0x74, 0x65, 0x64, 0x2E, 0x3E,
    0xC3, 0xB6, 0xBE, 0xD9, 0xC9, 0xE8, 0xB1, 0xB8, 0xCA, 0xA7, 0xB0, 0xDC, 0x21, 0x21, 0x21, 0xc0,
    0x45, 0x6E, 0x75, 0x6D, 0x20, 0x66, 0x61, 0x69, 0x6C, 0x75, 0x72, 0x65, 0x20, 0x21, 0x01, 0x27,
    0xD5, 0xFD, 0xD4, 0xDA, 0xD0, 0xB4, 0xC8, 0xEB, 0xCE, 0xC4, 0xBC, 0xFE, 0x21, 0x21, 0x21, 0x70,
    0x50, 0x6C, 0x65, 0x61, 0x73, 0x65, 0x20, 0x57, 0x61, 0x69, 0x74, 0x69, 0x6E, 0x67, 0x2E, 0x2E,
    0x43, 0x68, 0x69, 0x70, 0x20, 0x72, 0x65, 0x73, 0x65, 0x74, 0x20, 0x4F, 0x4B, 0x21, 0x20, 0x00,
    0x52, 0x65, 0x73, 0x65, 0x74, 0x20, 0x63, 0x68, 0x69, 0x70, 0x20, 0x45, 0x72, 0x72, 0x6F, 0x72,
    0x52, 0xd3, 0x44, 0xD0, 0xBE, 0xC6, 0xAC, 0xB8, 0xB4, 0xCE, 0xB3, 0xB3, 0xC9, 0xB9, 0xA6, 0xf1,
    0x54, 0xc3, 0x46, 0xDb, 0xBE, 0xC3, 0xA2, 0xB2, 0xB4, 0xC3, 0xBB, 0xca, 0xa7, 0xB0, 0xdc, 0x31,
    0x35, 0xb3, 0x48, 0xDc, 0xBE, 0xC5, 0xA3, 0xC9, 0xE8, 0xD6, 0xC3, 0xCA, 0xA7, 0xB0, 0xDC, 0x71,
    0x85, 0xa3, 0x42, 0xB4, 0xC5, 0xC5, 0xCC, 0xC9, 0xE8, 0xD6, 0xC3, 0xCA, 0xA7, 0xB0, 0xDC, 0xc1,
    0xD6, 0xB8, 0xC1, 0xEE, 0xC3, 0xBB, 0xD3, 0xD0, 0xD5, 0xFD, 0xB3, 0xA3, 0xCA, 0xE4, 0xB3, 0xF6,
    0x55, 0x79, 0xab, 0xDb, 0x3E, 0xC7, 0x89, 0x34, 0x34, 0x11, 0x35, 0x90, 0xfd, 0xf3, 0xf3, 0xf9,
    0x35, 0xb3, 0x48, 0xDc, 0xBE, 0xC5, 0xA3, 0xbb, 0xc3, 0x43, 0xC3, 0xCA, 0xA7, 0x34, 0x1C, 0x28,
    0x16, 0x55, 0x11, 0xcE, 0xC3, 0xB7, 0xD3, 0xD0, 0xD5, 0xFD, 0xB3, 0xA3, 0x33, 0x45, 0x34, 0x36,
};




static unsigned int  appsecindex2imageaddr(unsigned int index) {
    unsigned int addr;
    if (index == 0) {
        addr = APP_BAK_BEGIN_SECTOR;
    } else {
        addr = APP_BAK_BEGIN_SECTOR
               + APP_MAIN_BAK_MAX_SIZE + (index - 1) * APP_DEV_BAK_SECTOR_SIZE;
    }
    return addr * 512;
}


static void appdecrypt(void *dat, unsigned int len) {
    unsigned int *dattemp = (unsigned int *)dat;
    unsigned int *crypttemp = (unsigned int *)ProgramTable;
    for (int i = 0; i < len / 256; i++) {
        for (int j = 0; j < 64; j++) {
            dattemp[i*64+j] ^= crypttemp[j];
        }
    }
}


//before call this func ,caller should prepare 512 aligned 3K BYTE buf for data process
BURN_RET  burnAppFormBuf(void *appBuf, unsigned int appLen,
                         unsigned short appSegBitMap, void *workbuf,
                         unsigned int workbuflen) {
    unsigned int ret;
    unsigned char appsectag[16];
    APPHEADER *dheader = (APPHEADER *)workbuf;
    APPSETCTION *dsection =  (APPSETCTION *)((unsigned char *)workbuf + sizeof(APPHEADER));
    APPHEADER *sheader = (APPHEADER *)appBuf;
    APPSETCTION *ssection = (APPSETCTION *)((unsigned char *)appBuf + sizeof(APPHEADER));
    char *desc = (char *)workbuf + 1024;
    if (appBuf == NULL || appLen == 0 || appSegBitMap == 0
        || (unsigned int)appBuf % 512 != 0 || (unsigned int)workbuf % 512 != 0
        || workbuflen != 1024 * 3) {
        return BURN_PARAM_ERROR;
    }

    ret = MMCSDP_Read(mmcsdctr, dheader, APP_BAK_FLAG_SECTOR, 2); //read 1k
    if (false == ret) {
        return BURN_DES_ERROR;
    }

    ret = MMCSDP_Read(mmcsdctr, desc, APP_DESC_SECTOR, 4); //read 2k descption
    if (false == ret) {
        return BURN_DES_ERROR;
    }

    if (sheader->magic != APP_MAGIC_OK) {
        return BURN_SRC_ERROR;
    }
    if (sheader->numOfAppsec == 0 || sheader->numOfAppsec > 16) {
        return BURN_SRC_ERROR;
    }
    //check imageSize 512 byte allign
    for (int i = 0; i < sheader->numOfAppsec; i++) {
        if (ssection[i].imageSize % 512 != 0 || ssection[i].imageaddr % 512 != 0) {
            return BURN_SRC_ERROR;
        }
    }

    appSegBitMap &= sheader->secflag;
    if (appSegBitMap == 0)  return BURN_SRC_ERROR;

    //appsegflag bitmap to number series
    for (int i = 0, j = 0; i < sheader->numOfAppsec; i++) {
        if (sheader->secflag & (1 << i)) {
            appsectag[i] = j++;
        } else {
            appsectag[i] = 0;
        }
    }
    //clear the magic
    dheader->magic = APP_MAGIC_NO;
    ret = MMCSDP_Write(mmcsdctr, dheader, APP_BAK_FLAG_SECTOR, 1);
    if (false == ret) {
        return BURN_DES_ERROR;
    }
    dheader->descriptOffset = APP_DESC_SECTOR * 512;
    dheader->dataOffset = APP_BAK_BEGIN_SECTOR * 512;

    //burn data
    for (unsigned int i = 0; i < sheader->numOfAppsec; i++) {
        if (appSegBitMap & 1 << i) {
            //set dsection
            memcpy(dsection + i, ssection + appsectag[i], sizeof dsection[i]);
            dsection[i].imageaddr = appsecindex2imageaddr(i)- dheader->dataOffset;
            dsection[i].imageCheck = APP_MAGIC_OK;
            dsection[i].descriptOffset = APP_DESC_SECTOR * 512 + 2048 / 16 * i;

            if (i == 0 && dsection[i].imageSize > APP_MAIN_BAK_MAX_SIZE * 512) return BURN_SRC_ERROR;
            if (i != 0 && dsection[i].imageSize > APP_DEV_BAK_SECTOR_SIZE * 512) return BURN_SRC_ERROR;
            //copy image version descipt
            strncpy(desc + 2048 / 16 * i, (char *)sheader
                    + sheader->descriptOffset + ssection[i].descriptOffset
                    , 2048 / 16);
            desc[2048 / 16 * i + 2048 / 16 - 1] = 0;
            //decryption
            unsigned char *addr = (unsigned char *)appBuf +
                                  sheader->dataOffset +
                                  ssection[appsectag[i]].imageaddr;
            appdecrypt(addr, ssection[appsectag[i]].imageSize);
            ret = MMCSDP_Write(mmcsdctr, addr, (dsection[i].imageaddr+ dheader->dataOffset)/512, dsection[i].imageSize / 512);
            if (false == ret) {
                return BURN_DES_ERROR;
            }
        }
    }

    ret =  MMCSDP_Write(mmcsdctr, desc, APP_DESC_SECTOR, 4); //write descript
    if (false == ret) {
        return BURN_DES_ERROR;
    }
    //flag the magic
    dheader->magic = APP_MAGIC_OK;
    dheader->secflag |= appSegBitMap;
    unsigned int j= 0;
    for (int i=0;i<16;i++) {
        if ((appSegBitMap & 1<<i)!=0) j++;
    }
    dheader->numOfAppsec = j;
    ret =  MMCSDP_Write(mmcsdctr, dheader, APP_BAK_FLAG_SECTOR, 2); //modify app heard
    if (false == ret) {
        return BURN_DES_ERROR;
    }
    //if(!burnRunAPP()) return BURN_DES_ERROR;
    return BURN_OK;
}


bool burnRunAPP(void *workbuf,unsigned int len) {
    unsigned int ret;
    unsigned char buf[512];
    APPHEADER *header = (APPHEADER *)buf;
    APPSETCTION *section =  (APPSETCTION *)(buf + sizeof(APPHEADER));
    ASSERT(len%512==0);
    ret = MMCSDP_Read(mmcsdctr, buf, APP_BAK_FLAG_SECTOR, 1);
    if (ret == 0) {
        return false;
    }
    if (header->magic != APP_MAGIC_OK || section->imageCheck != APP_MAGIC_OK
        ||(header->secflag&0x01)==0) {
        return false;
    }
    unsigned int sappaddr = header->dataOffset + section->imageaddr ;
    unsigned int appsize = section->imageSize ;
    if (appsize >= APP_RUN_SECTOR_SIZE*512) {
        return false;
    }
    ret = MMCSDP_Read(mmcsdctr, buf, APP_RUN_FLAG_SECTOR, 1);
    if (ret == 0) {
        return false;
    }
    RUNAPPHEAD *runheader = (RUNAPPHEAD *)buf;
    unsigned char diimage = !runheader->iimage;
    unsigned int  dappaddr = (diimage == 0 ? APP1_RUN_SECTOR : APP2_RUN_SECTOR)*512;
    runheader->image[diimage].imageaddr =  dappaddr;
    runheader->image[diimage].imagelen =  appsize;
    //set des image tag to APP_MAGIC_NO
    runheader->image[diimage].imageCheck = APP_MAGIC_NO;
    ret = MMCSDP_Write(mmcsdctr, buf, APP_RUN_FLAG_SECTOR, 1);
    if (ret == 0) {
        return false;
    }
    for (int i = 0; i < DIVUP(appsize, 512); i++) {
        ret = MMCSDP_Read(mmcsdctr, buf, sappaddr / 512 + i, 1);
        if (ret == 0) {
            return false;
        }
        ret = MMCSDP_Write(mmcsdctr, buf, dappaddr / 512 + i, 1);
        if (ret == 0) {
            return false;
        }
    }
    ret = MMCSDP_Read(mmcsdctr, buf, APP_RUN_FLAG_SECTOR, 1);
    if (ret == 0) {
        return false;
    }
    runheader->iimage = diimage;
    runheader->image[diimage].imageCheck = APP_MAGIC_OK;
    ret = MMCSDP_Write(mmcsdctr, buf, APP_RUN_FLAG_SECTOR, 1);
    if (ret == 0) {
        return false;
    }
    return true;
}


void reboot() {
    ((void (*)())0x402F0400)();
}

