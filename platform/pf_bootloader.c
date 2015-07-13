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




void appdecrypt(void *dat, unsigned int len) {
    unsigned int *dattemp = (unsigned int *)dat;
    unsigned int *crypttemp = (unsigned int *)ProgramTable;
    for (int i = 0; i < len / 256; i++) {
        for (int j = 0; j < 64; j++) {
            dattemp[i * 64 + j] ^= crypttemp[j];
        }
    }
}


//before call this func ,caller should prepare 512 aligned 3K BYTE buf for data process
BURN_RET  burnAppFormBuf(void *appBuf, unsigned int appLen) {
    unsigned int ret;
    APPHEADER *sheader = (APPHEADER *)appBuf;
    APPSETCTION *ssection = (APPSETCTION *)((unsigned char *)appBuf + sizeof(APPHEADER));
    if (appBuf == NULL || appLen == 0 || appLen % 512 != 0 || (unsigned int)appBuf % 512 != 0
        || appLen > APP_BAK_MAX_SIZE*512) {
        return BURN_PARAM_ERROR;
    }
    if (sheader->magic != APP_MAGIC_OK) {
        return BURN_SRC_ERROR;
    }
    if (sheader->packSize!=appLen) {
        return BURN_SRC_ERROR;
    }
    //appbuf not include main app
    if ((sheader->secflag & 0x01) == 0) {
        return BURN_SRC_ERROR;
    }
    //check imageSize 512 byte allign
    for (int i = 0; i < sheader->numOfAppsec; i++) {
        if (ssection[i].imageSize % 512 != 0 || ssection[i].imageaddr % 512 != 0) {
            return BURN_SRC_ERROR;
        }
    }

    //decypt app data
    //unsigned char *addr = (unsigned char *)appBuf + sheader->dataOffset;
    //appdecrypt(addr, appLen - sheader->dataOffset);

    //clear the magic
    sheader->magic = APP_MAGIC_NO;
    //write app pack to nand
    ret = MMCSDP_Write(mmcsdctr, sheader, APP_BAK_SECTOR, appLen/512);
    if (false == ret) {
        return BURN_DES_ERROR;
    }
    sheader->magic = APP_MAGIC_OK;
    ret =  MMCSDP_Write(mmcsdctr, sheader, APP_BAK_SECTOR,1); //write MAGIC
    if (false == ret) {
        return BURN_DES_ERROR;
    }
    return BURN_OK;
}


bool burnRunAPP() {
    unsigned int ret;
    unsigned char buf[512];
    APPHEADER *header = (APPHEADER *)buf;
    APPSETCTION *section =  (APPSETCTION *)(buf + sizeof(APPHEADER));
    ret = MMCSDP_Read(mmcsdctr, buf, APP_BAK_SECTOR, 1);
    if (ret == 0) {
        return false;
    }
    if (header->magic != APP_MAGIC_OK || section->imageCheck != APP_MAGIC_OK
        || (header->secflag & 0x01) == 0) {
        return false;
    }
    unsigned int sappaddr = header->dataOffset + section->imageaddr+APP_BAK_SECTOR*512;
    unsigned int appsize = section->imageSize;
    if (appsize >= APP_RUN_SECTOR_SIZE * 512) {
        return false;
    }

    ret = MMCSDP_Read(mmcsdctr, buf, APP_RUN_FLAG_SECTOR, 1);
    if (ret == 0) {
        return false;
    }
    RUNAPPHEAD *runheader = (RUNAPPHEAD *)buf;
    unsigned char diimage = !runheader->iimage;
    unsigned int  dappaddr = (diimage == 0 ? APP1_RUN_SECTOR : APP2_RUN_SECTOR) * 512;
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
        appdecrypt(buf, 512);
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


void eraseRunApp(void){
   char buf[512];
   memset(buf,0,sizeof buf);
   MMCSDP_Write(mmcsdctr, buf, APP_RUN_FLAG_SECTOR, 1);
}


void reboot() {
    //((void (*)())0x402F0400)();
    *(unsigned int *)0x44E00F00 |= 0x01;
}

