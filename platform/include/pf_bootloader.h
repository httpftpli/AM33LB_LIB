#ifndef __PF__BOOTLOADER__
#define __PF__BOOTLOADER__


#include <stdbool.h>


#define BOOTLOADER_BEGIN_SECTOR      256

#define UID_SAVE_SECTOR              641
#define INAND_TEST_SECTOR            768
#define TOUCH_CAL_PARAM_SECTOR       769

#define APP_RUN_FLAG_SECTOR          770
#define APP_RUN_SECTOR_SIZE          (32*1024*2)  //32M BYTE    UNIT:SECTOR
#define APP1_RUN_SECTOR              771
#define APP2_RUN_SECTOR       (APP1_RUN_SECTOR+APP_RUN_SECTOR_SIZE)
#define APP_BAK_SECTOR        (APP2_RUN_SECTOR+APP_RUN_SECTOR_SIZE)
#define APP_BAK_MAX_SIZE      (128*1024*2)  //APP bak size  UNIT:SECTOR

#define USER_SECTOR   (APP_BAK_SECTOR + APP_BAK_MAX_SIZE)
#define USER_SECTOR_SIZE      (64*1024*2)     //USER_SECTOR  UNIT:SECTOR

#define FS_BEGIN_SECTOR           (USER_SECTOR+USER_SECTOR_SIZE)
#define FIRST_PARTITION_SIZE      (128*1024*2) //UNIT:SECTOR

#define APP_ENTRY             0x80000000
#define NUM_OF_APPSECTION     8
#define APP_MAGIC_OK          0x5555aaaa
#define APP_MAGIC_NO          0xaaaa5555
#define APP_FLAG              0x444e49ed


#define BOOTLOADER_ENTRY          0x402f0400



typedef struct{
    unsigned int imageCheck;
    unsigned int imageaddr;
    unsigned int imagelen;
} RUNIMAGE;

typedef struct {
    unsigned char iimage;      //which image will be boot
    RUNIMAGE image[2];
} RUNAPPHEAD;

typedef struct {
    unsigned int imageaddr;         //地址，相对于APPHEADER.dataOffset
    unsigned int imageSize;         //大小，单位：字节
    unsigned int imageCheck;        //magic
    unsigned int hardwareFlag;
    unsigned int customFlag;
    unsigned short imageMainRev;    //主版本号
    unsigned short imageMidRev;     //中间版本号
    unsigned short imageMinRev;     //小版本号
    unsigned short dummy;          //暂时不用
    unsigned int time;
    unsigned int descriptOffset;    //描述文字偏移
}APPSETCTION;


typedef struct
{
    unsigned int magic;
    unsigned int packSize;
    unsigned short numOfAppsec;
    unsigned short secflag;
    unsigned int descriptOffset; //描述文字偏移，相对于APPHEADER
    unsigned int dataOffset;  //程序内容偏移，相对于APPHEADER
}APPHEADER;





typedef enum{BURN_OK=0,BURN_PARAM_ERROR,BURN_FILE_CHECKSUM_ERROR, BURN_FILE_ERROR,BURN_SRC_ERROR,BURN_DES_ERROR}
BURN_RET;

extern void reboot();
extern void appdecrypt(void *dat, unsigned int len);
extern BURN_RET  burnAppFormBuf(void *appBuf,unsigned int appLen );
extern BURN_RET  burnBootFormBuf(void *bootBuf,unsigned int bootLen);
extern bool burnRunAPP();
extern void eraseRunApp(void);

#endif
