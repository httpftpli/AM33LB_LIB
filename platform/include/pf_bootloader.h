#ifndef __PF__BOOTLOADER__
#define __PF__BOOTLOADER__

#define APP_HEAD_SECTOR       770
#define APP_BEGIN_SECTOR      771
#define APP_SECTION_SIZE      64*1024   //UNIT:SECTOR 512 byte

#define MAX_SIZE_APPAPPSECTION         64*1024*1024
#define MAX_APP_FILE_SIZE     128*1024*1024    
#define APP_ENTRY             0x80000000
#define NUM_OF_APPSECTION     8
#define APP_MAGIC_OK          0x5555aaaa
#define APP_MAGIC_NO          0xaaaa5555
#define APP_FLAG              0x444e49ed

#define BOOTLOADER_BEGIN_SECTOR   256
#define BOOTLOADER_ENTRY          0x402f0400
#define FS_BEGIN_SECTOR           128*1024
#define FIRST_PARTITION_SIZE      256*1024 //in sector


typedef struct __appsection{
   unsigned int imageaddr;         //地址，相对于打包后的文件头
   unsigned int imageSize;         //大小，单位：字节
   unsigned int imageCheck;        //magic
   char imageRevPrefix;           //版本号前缀,数字，范围0-255
   unsigned short imageMainRev;    //主版本号
   unsigned short imageMidRev;     //中间版本号
   unsigned short imageMinRev;     //小版本号
   unsigned int   dummy;         //暂时不用
}APPSETCTION;



typedef struct app_header_
{
   unsigned int flag;
   unsigned int magic;
   unsigned short numOfAppsec;
   unsigned short secflag;
   APPSETCTION appsec1;
   APPSETCTION appsec2;
}APPHEADER;


typedef enum{BURN_OK=0,BURN_PARAM_ERROR,BURN_FILE_CHECKSUM_ERROR, BURN_FILE_ERROR,BURN_SRC_ERROR,BURN_DES_ERROR}
BURN_RET;
   
extern void reboot();
extern BURN_RET  burnAppFormBuf(void *appBuf,unsigned int appLen,unsigned int burnSegFlag);
extern BURN_RET  burnBootFormBuf(void *bootBuf,unsigned int bootLen);

#endif
