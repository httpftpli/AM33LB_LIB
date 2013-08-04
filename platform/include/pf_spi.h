#ifndef __PF__SPI__H__
#define __PF__SPI__H__



typedef enum {
   STATE_OUTPUT_PREFIX,
   STATE_OUTPUT_BUF,
   STATE_INPUT,
   STATE_CHECKERROR,
   STATE_OK,
} SPI_TRANSFER_STATE;

typedef struct __spitrasfer {
   void *prefixWrbuf;
   unsigned int prefixWrbufLen;
   void *buf;
   unsigned int len;
   unsigned int bufpointer;
   unsigned int txprivate;
   unsigned int rxprivate;
   unsigned char dir:1;
   unsigned char needWrCheck:1;
   unsigned char inWrCheck:1;
   void *checkPrefixWrbuf;
   unsigned int  checkprefixWrbufLen;
   BOOL(*check)(int date);
   SPI_TRANSFER_STATE state;
   volatile unsigned short finish;
} SPI_TRANSFER;


extern SPI_TRANSFER g_spitransfer ;
void SPIMasterInit(unsigned int moduleId, unsigned char csChanel, unsigned int spiClk, unsigned char cpha, unsigned char cpol, unsigned char wordLen);
extern BOOL SPIWrite(unsigned int moduleId,void *wrbuf0, unsigned int lenOfBuf0, void *wrbuf1,unsigned int lenOfBuf1, BOOL needCheck, void *checkPrefixWrbuf, unsigned int checkPrefixWrbufLen); 
extern BOOL SPIRead(unsigned int moduleId,void *wrBuf, unsigned int wrBufLen, void *buf, unsigned int len);



#endif

