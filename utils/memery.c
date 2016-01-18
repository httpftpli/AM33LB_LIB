/**
 *  \file   mem.c
 *
 *  \brief
 *  \author  lfl
 *  \addtogroup MEM
 *  \# include "mem.h"
 *
 *  内存操作有关的函数
 *
 *   @{
 *
 */


#include "type.h"
#include <wchar.h>
#include "debug.h"
#include "type.h"
#include "mem.h"
#include "mmath.h"


extern void memset_eabi_16(void *s,unsigned short val,unsigned int size);
extern void memset_eabi_32(void *s,unsigned int val,unsigned int size);



#define OS_OBJ_TYPE_MEM       1;

/**
 * @brief memset 的16位版本
 *
 *  和函数库 \b wcsmemset
 *  函数功能一样，并对ARM内核进行了优化
 * @param [in] s
 * @param [in]  val
 * @param [in]  n 个数，以16位为单位
 * @return
 * @date    2013/7/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void  memset16(void *s, unsigned short val, size_t16 n) {
   ASSERT(((unsigned int)s&0x01)==0);
   unsigned int s_temp = (unsigned int)s;
   if((unsigned int)s % 4){
       *((unsigned short *)s) = val;
       s_temp += 2;
       n -= 1;
   }
   unsigned int n_16 = n / 16;
   if(n_16){
      memset_eabi_16((void*)s_temp, val, n_16*16);
      s_temp  +=  n_16*16*2 ;
   }
   unsigned int nmode = n % 16;
   for (int i = 0; i < nmode; i++) {
      ((unsigned short *)s_temp)[i] = val;
   }
}

void memset32(void *s,unsigned int val,size_t32 n){
   ASSERT(((unsigned int)s&0x03)==0);
   unsigned int s_temp = (unsigned int)s;
   unsigned int n_8 = n / 8;
   if(n_8){
      memset_eabi_32((void*)s_temp, val, n_8*8);
      s_temp  +=  n_8 * 8 *4 ;
   }
   unsigned int nmode = n % 8;
   for (int i = 0; i < nmode; i++) {
      ((unsigned int *)s_temp)[i] = val;
   }
}

BOOL memis(void *mem,unsigned char dat,unsigned int memlen){
   unsigned char * buf = mem;
   for (int i= 0;i<memlen;i++) {
      if(buf[i]!=dat)
         return FALSE;
   }
   return TRUE;
}

BOOL memis_32(void *mem,unsigned int dat,unsigned int memlen32){
   unsigned int * buf = mem;
   for (int i= 0;i<memlen32;i++) {
      if(buf[i]!=dat)
         return FALSE;
   }
   return TRUE;
}


/**
 * @brief 判断字符串是不是以指定的字符串结尾
 * @param [in] s 要判断的字符串
 * @param [in] endwith  结尾的字符串
 * @return  BOOL
 * @date    2013/7/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL strendwith(const char *s,const  char *endwith){
   ASSERT((s!=NULL)&&(endwith!=NULL));
   char *temp = (char *)endwith;
   while (*s) s++;
   while (*temp) temp++;
   do {
      if (*--temp !=  *--s)
          return FALSE;
   } while (temp != endwith);
   return TRUE;
}


/**
 * @brief strendwith 的双字节版本，比如UNICODE
 * @param [in] s
 * @param [in]  endwith
 * @return  BOOL
 * @date    2013/7/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 * strendwith()
 */
BOOL wcsendwith(const wchar_t *s,const wchar_t *endwith){
   ASSERT((s!=NULL)&&(endwith!=NULL));
   wchar_t *temp = (wchar_t *)endwith;
   while (*s) s++;
   while (*temp++) temp++;
   do {
      if (*--temp !=  *--endwith)
          return FALSE;
   }
   while (temp != endwith);
   return TRUE;
}



static unsigned int memQty = 0;

void  MemCreate (OS_MEM      *p_mem,
                   char    *p_name,
                   void        *p_addr,
                   unsigned int   n_blks,
                   unsigned int  blk_size,
                   MEM_ERR      *p_err)
{

    uint16     i;
    uint16     loops;
    char    *p_blk;
    void         **p_link;

    ASSERT((unsigned int)p_addr %4 ==0);  //4 bytes aligne
    blk_size = BOUNDUP(blk_size,4);
    p_link = (void **)p_addr;                               /* Create linked list of free memory blocks               */
    p_blk  = p_addr;
    loops  = n_blks - 1u;
    for (i = 0u; i < loops; i++) {
        p_blk +=  blk_size;
       *p_link = (void  *)p_blk;                            /* Save pointer to NEXT block in CURRENT block            */
        p_link = (void **)(void *)p_blk;                    /* Position     to NEXT block                             */
    }
    *p_link            = (void *)0;                         /* Last memory block points to NULL                       */

    //OS_CRITICAL_ENTER();
    p_mem->Type        = OS_OBJ_TYPE_MEM;                   /* Set the type of object                                 */
    p_mem->NamePtr     = p_name;                            /* Save name of memory partition                          */
    p_mem->AddrPtr     = p_addr;                            /* Store start address of memory partition                */
    p_mem->FreeListPtr = p_addr;                            /* Initialize pointer to pool of free blocks              */
    p_mem->NbrFree     = n_blks;                            /* Store number of free blocks in MCB                     */
    p_mem->NbrMax      = n_blks;
    p_mem->BlkSize     = blk_size;                          /* Store block size of each memory blocks                 */

    memQty++;
    //OS_CRITICAL_EXIT();
   *p_err = MEM_ERR_NONE;
}



void  *MemGet (OS_MEM  *p_mem,
                 MEM_ERR  *p_err)
{
    void    *p_blk;

    //CPU_CRITICAL_ENTER();
    if (p_mem->NbrFree == (uint16)0) {                  /* See if there are any free memory blocks                */
        //CPU_CRITICAL_EXIT();
       *p_err = MEM_ERR_NO_FREE_BLKS;                    /* No,  Notify caller of empty memory partition           */
        return ((void *)0);                                 /*      Return NULL pointer to caller                     */
    }
    p_blk              = p_mem->FreeListPtr;                /* Yes, point to next free memory block                   */
    p_mem->FreeListPtr = *(void **)p_blk;                   /*      Adjust pointer to new free list                   */
    p_mem->NbrFree--;                                       /*      One less memory block in this partition           */
    //CPU_CRITICAL_EXIT();
   *p_err = MEM_ERR_NONE;                                    /*      No error                                          */
    return (p_blk);                                         /*      Return memory block to caller                     */
}



void  MemPut (OS_MEM  *p_mem,
                void    *p_blk,
                MEM_ERR  *p_err)
{

    //CPU_CRITICAL_ENTER();
    if (p_mem->NbrFree >= p_mem->NbrMax) {                  /* Make sure all blocks not already returned              */
        //CPU_CRITICAL_EXIT();
       *p_err = MEM_ERR_MEM_FULL;
        return;
    }
    *(void **)p_blk    = p_mem->FreeListPtr;                /* Insert released block into free block list             */
    p_mem->FreeListPtr = p_blk;
    p_mem->NbrFree++;                                       /* One more memory block in this partition                */
    //CPU_CRITICAL_EXIT();
   *p_err              = MEM_ERR_NONE;                       /* Notify caller that memory block was released           */
}


//! @}



