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


extern void memset_eabi_16(void *s,unsigned short val,unsigned int size);

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
};


BOOL memis(void *mem,unsigned char dat,unsigned int memlen){
   unsigned char * buf = mem;
   for (int i= 0;i<memlen;i++) {
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

//! @}



