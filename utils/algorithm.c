/**
 *  \file   algorithm.c
 *
 *  \brief
 *  \author  李飞亮  
 *  \addtogroup Algorithm
 *  各种通用算法，排序，队列等
 *   @{
 *   
 */



#include "type.h"
#include "algorithm.h"
#include "string.h"

void bubbleSortAscend(int *buf, unsigned int nNum) {
   int j, k;
   int flag = nNum;
   unsigned int tmp;
   while (flag > 0) {
      k = flag;
      flag = 0;
      for (j = 1; j < k; j++){
         if (buf[j - 1] > buf[j]) {
            tmp = buf[j-1];
            buf[j - 1] = buf[j];
            buf[j] = tmp;
            flag = j;
         }
      }
   }
}

int sum(int *buf,unsigned int nNum){
   unsigned int sum = 0;
   for (int i=0;i<nNum;i++) {
      sum += buf[i];
   }
   return sum;
}




/**
 * @brief 初始化环形队列
 * @param [in] queue  队列指针
 * @param [in] buf    缓冲区指针     
 * @param [in] sizeOfItem 
 * @param [in] nItem  缓冲区item个数  
 * @return  NONE         
 * @date    2013/5/10
 * @note 
 */
void ringBufInit(RINGBUF *ringBuf, void * buf, unsigned int sizeOfItem,unsigned int nItem)
{
	ringBuf->writeIndex = 0;	
	ringBuf->readIndex  = 0;
	ringBuf->sizeOfItem = sizeOfItem;
	ringBuf->nItem  =  nItem+1;
	ringBuf->buf  =  buf;
}


BOOL isRingBufFull(RINGBUF *ringBuf){
   unsigned int i = ringBuf->writeIndex+1;
   if (i==ringBuf->nItem) {
      i=0;
   }
   if (i==ringBuf->readIndex) {
      return TRUE;
   }
   return FALSE;
}

BOOL isRingBufEmpty(RINGBUF *ringBuf){
   if (ringBuf->writeIndex == ringBuf->readIndex ) {
      return TRUE;
   }
   return FALSE;
}



/**
 * @brief 入队 
 *  
 * 通过数据拷贝入队，入队时把参数 \b item 
 * 指向的内容拷贝到缓冲队列  
 * @param [in] ringBuf
 * @param [in] item 
 * @return 
 * - FALSE   失败，因为当前fifo满 
 * - TRUE    成功 
 * @date    2013/5/10 
 */
BOOL ringBufPush(RINGBUF *ringBuf, void *item) {	

	if(isRingBufFull(ringBuf))
       return FALSE;
	memcpy((unsigned char *)(ringBuf->buf) + ((ringBuf->writeIndex) * ringBuf->sizeOfItem)
			,item,ringBuf->sizeOfItem);
	if(++ringBuf->writeIndex == ringBuf->nItem)
		ringBuf->writeIndex = 0;
	return TRUE;
}


/**
 * @brief 出队 
 *  
 * 通过数据拷贝出队 
 * @param [in]  ringBuf
 * @param [out]  item  item缓冲区 
 * @return 
 * - FALSE   失败，因为队列空 
 * - TRUE    成功 
 * @date    2013/5/10 
 */
BOOL ringBufPop(RINGBUF * ringBuf, void * item)
{
   if (isRingBufEmpty(ringBuf)) {
      return FALSE;
   }
   memcpy(item,(unsigned char *)ringBuf->buf+ringBuf->readIndex*ringBuf->sizeOfItem,ringBuf->sizeOfItem);
   if(++ringBuf->readIndex == ringBuf->nItem)
		ringBuf->readIndex = 0;
   return TRUE;
}


/**
 * @brief 读队列头，但不出队
 * @param [in] ringBuf 
 * @param [out] item 返回item的指针
 * @return 
 * - FALSE   失败，因为队列空 
 * - TRUE    成功  
 * @date    2013/5/10

 */
BOOL ringBufRead(RINGBUF * ringBuf, void ** item){
   if (isRingBufEmpty(ringBuf)) {
      return FALSE;
   }
   *item =  (void *)((unsigned char *)ringBuf->buf+ringBuf->readIndex*ringBuf->sizeOfItem);
   return TRUE;
}

//! @}
