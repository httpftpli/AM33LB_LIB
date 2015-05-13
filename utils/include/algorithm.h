#ifndef __ALGORITHM__H__
#define __ALGORITHM__H__

#include "stdbool.h"
#include "list.h"
#include "type.h"



extern void bubbleSortAscend_I32(int *buf, unsigned int nNum);
extern void bubbleSortAscend_U32(unsigned int *buf, unsigned int nNum);
extern void bubbleSortAscend_I16(short *buf, unsigned int nNum);
extern void bubbleSortAscend_U16(unsigned short *buf, unsigned int nNum);
extern void bubbleSortAscend_I8(char *buf, unsigned int nNum);
extern void bubbleSortAscend_U8(unsigned char *buf, unsigned int nNum);
extern void bubbleSortAscend(int *buf, unsigned int nNum);
extern int sum(int *buf,unsigned int nNum);
extern void list_sort_insert(struct list_head *new ,struct list_head *list
              ,int (*cmp)(const struct list_head *, const struct list_head *));


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
    bool full;
    bool empty;
    bool threadSafe;
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


extern uint8 const UnMapTbl[]; 

extern void ringBufInit(RINGBUF *ringBuf, void * buf, unsigned int sizeOfItem,unsigned int nItem,bool threadsafe);
extern bool isRingBufFull(RINGBUF *ringBuf);
extern bool isRingBufEmpty(RINGBUF *ringBuf);
extern bool ringBufPush(RINGBUF *ringBuf, void *item);
extern bool ringBufPop(RINGBUF * ringBuf, void * item);
extern bool ringBufRead(RINGBUF * ringBuf, void ** item);
extern bool ringBufPop_noread(RINGBUF *ringBuf);

extern unsigned short crc16_IBM(void* dataIn, int length);
extern unsigned short crc16_MD(void *data, unsigned short len);

extern uint32 calculate_crc32(char *p, unsigned int length);
extern uint32 calculate_crc32_jamcrc(char *p, unsigned int length);
extern uint32 calculate_crc32_c(char *p, unsigned int length);
extern uint32 calculate_crc32_d(char *p, unsigned int length);
extern uint32 calculate_crc32_bzip2(char *p, unsigned int length);
extern uint32 calculate_crc32_mpeg2(char *p, unsigned int length);
extern uint32 calculate_crc32_posix(char *p, unsigned int length);
extern uint32 calculate_crc32_k(char *p, unsigned int length);
extern uint32 calculate_crc32_q(char *p, unsigned int length);
extern uint32 calculate_crc32_xfer(char *p, unsigned int length);

extern void MD5Init(MD5_CTX *context);
extern void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
extern void MD5Final(MD5_CTX *context,unsigned char digest[16]);

#endif









