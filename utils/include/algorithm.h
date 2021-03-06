#ifndef __ALGORITHM__H__
#define __ALGORITHM__H__


extern void bubbleSortAscend(int *buf, unsigned int nNum);
extern int sum(int *buf,unsigned int nNum);


/**
 * @brief FIFI 环形队列 
 *  
 * 一个没有加锁的环形队列，一读一写环形队列
 * @date    2013/5/10 
 */
typedef struct tagfilo {
    unsigned long writeIndex;   /** < @brief 写下标  */
    unsigned long readIndex;    /** < @brief 写下标  */
	unsigned char sizeOfItem;
    unsigned long nItem;        /** < @brief item总个数 */  
	unsigned long nAvaItem;		/** < @brief 记录当前fifi中的item个数*/
    void *        buf;        /** < @brief  缓冲区指针 */
}RINGBUF;


/**
 * @brief MD5散列结构体 
 * @date    2013/5/10 
 */
typedef struct
{
	unsigned int count[2];
	unsigned int state[4];
	unsigned char buffer[64];   
}MD5_CTX;


extern void ringBufInit(RINGBUF *ringBuf, void * buf, unsigned int sizeOfItem,unsigned int nItem);
extern BOOL isRingBufFull(RINGBUF *ringBuf);
extern BOOL isRingBufEmpty(RINGBUF *ringBuf);
extern BOOL ringBufPush(RINGBUF *ringBuf, void *item);
extern BOOL ringBufPop(RINGBUF * ringBuf, void * item);
extern BOOL ringBufRead(RINGBUF * ringBuf, void ** item);

extern unsigned short crc16_IBM(void* dataIn, int length);
extern unsigned short crc16_MD(void *date, uint16 len);

extern void MD5Init(MD5_CTX *context);
extern void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
extern void MD5Final(MD5_CTX *context,unsigned char digest[16]);

#endif

