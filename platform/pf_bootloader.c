#include "pf_bootloader.h"
#include "algorithm.h"
#include "stdbool.h"
#include "stddef.h"
#include "mmcsd_proto.h"
#include <string.h>


extern mmcsdCtrlInfo mmcsdctr[2];


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

BURN_RET  burnAppFormBuf(void *appBuf, unsigned int appLen, unsigned int burnSegFlag) {
    unsigned int ret;
    unsigned char *buf = (unsigned char *)appBuf;
    unsigned char bufsector[512];
    APPHEADER *realheader = (APPHEADER *)bufsector;
    APPSETCTION *realsection = &(realheader->appsec1);

    if ((appBuf == NULL) || (appLen == 0) || (burnSegFlag == 0) ||
        ((unsigned int)appBuf % 4 != 0)) {
        return BURN_PARAM_ERROR;
    }
    //process burn
    ret = MMCSDP_Read(mmcsdctr, bufsector, APP_HEAD_SECTOR, 1);
    if (false == ret) {
        return BURN_DES_ERROR;
    }

    realsection->imageaddr = APP_BEGIN_SECTOR ;
    realsection->imageSize = appLen;
    realsection->imageCheck = APP_MAGIC_NO;
    //clear the magic
    ret = MMCSDP_Write(mmcsdctr, realheader, APP_HEAD_SECTOR, 1); //modify app heard
    if (false == ret) {
        return BURN_DES_ERROR;
    }
    //burn data
    ret = MMCSDP_Write(mmcsdctr, buf, APP_BEGIN_SECTOR,DIVUP(realsection->imageSize,512));
    if (false == ret) {
        return BURN_DES_ERROR;
    }

    //flag the magic
    realsection->imageCheck = APP_MAGIC_OK;
    realheader->magic = APP_MAGIC_OK;
    ret =  MMCSDP_Write(mmcsdctr, realheader, APP_HEAD_SECTOR, 1); //modify app heard
    if (false == ret) {
        return BURN_DES_ERROR;
    }
   return BURN_OK;
}



/*BURN_RET  burnAppFormBuf(void *appBuf, unsigned int appLen, unsigned int burnSegFlag) {
    unsigned int ret;
    unsigned char *buf = (unsigned char *)appBuf;
    unsigned char bufsector[512];
    APPHEADER *appheader = (APPHEADER *)appBuf;
    APPSETCTION *appsection = &(appheader->appsec1);
    APPHEADER *realheader = (APPHEADER *)bufsector;
    APPSETCTION *realsection = &(realheader->appsec1);
    if ((appBuf == NULL) || (appLen == 0) || (burnSegFlag == 0) ||
        ((unsigned int)appBuf % 4 != 0) || (appLen % 512 != 0)) {
        return BURN_PARAM_ERROR;
    }

    //decrypt
    for (int i = 0; i < appLen / 512; i++, buf += 512) {
        for (int j = 0; j < 64; j++) {
            ((unsigned int *)buf)[j] ^= ((unsigned int *)ProgramTable)[j];
            ((unsigned int *)buf)[j + 64] ^= ((unsigned int *)ProgramTable)[j];
        }
    }
    //check app file date md5sum
    MD5_CTX md5context;
    unsigned char decrypt[16];
    MD5Init(&md5context);
    MD5Update(&md5context, buf, appLen - 16);
    MD5Final(&md5context, decrypt);
    if (memcmp(buf + appLen - 16, decrypt, 16)) {
        return BURN_FILE_CHECKSUM_ERROR;
    }
    if (burnSegFlag & appheader->secflag == 0) {
        return BURN_PARAM_ERROR;
    }
    //check app file data format
    unsigned short temp =0;
    for (int i=0;i<16;i++) {//check number of app section
        if (appheader->secflag&(1<<i) == 1) {
            temp++;
        }
    }
    if (temp != appheader->numOfAppsec) {
        return BURN_FILE_ERROR;
    }
    for (int i = 0; i < NUM_OF_APPSECTION; i++) { //check addr of app section
        if ((burnSegFlag & 0x01 << i) && ((appsection+i)->imageaddr + (appsection+i)->imageSize) > (appLen - 16)) {
            return BURN_FILE_ERROR;
        }
    }
    //process burn
    ret = MMCSDP_Read(mmcsdctr, bufsector, APP_HEAD_SECTOR, 1);
    if (false == ret) {
        return BURN_DES_ERROR;
    }
    if (appheader->secflag & burnSegFlag & 0x20) {//if flag burn main app, clear magic
        realheader->magic = APP_MAGIC_NO;
    }
    for (int i = 0; i < NUM_OF_APPSECTION; i++) {
        if (appheader->secflag & burnSegFlag & 0x01 << i) {
            (realsection + i)->imageaddr = APP_BEGIN_SECTOR + i * APP_SECTION_SIZE;
            (realsection + i)->imageSize = DIVUP((appsection + i)->imageSize, 512);
            (realsection + i)->imageRevPrefix = (appsection + i)->imageRevPrefix;
            (realsection + i)->imageMainRev = (appsection + i)->imageMainRev;
            (realsection + i)->imageMidRev = (appsection + i)->imageMidRev;
            (realsection + i)->imageMinRev = (appsection + i)->imageMinRev;
            (realsection + i)->imageCheck = APP_MAGIC_NO;
        }
    }
    ret = MMCSDP_Write(mmcsdctr, realheader, APP_HEAD_SECTOR, 1); //modify app heard
    if (false == ret) {
        return BURN_DES_ERROR;
    }
    for (int i = 0; i < NUM_OF_APPSECTION; i++) {
        if (appheader->secflag & burnSegFlag & 0x01 << i) {
            ret = MMCSDP_Write(mmcsdctr, buf + (appsection + i)->imageaddr, (realsection + i)->imageaddr, (realsection + i)->imageSize);
            if (false == ret) {
                return BURN_DES_ERROR;
            }
            (realsection + i)->imageCheck = APP_MAGIC_OK;
        }
    }
    ret =  MMCSDP_Write(mmcsdctr, realheader, APP_HEAD_SECTOR, 1); //modify app heard
    if (false == ret) {
        return BURN_DES_ERROR;
    }
   return BURN_OK;
}*/



void reboot() {
    ((void (*)())0x402F0400)();
}

