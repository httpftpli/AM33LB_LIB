
#include "type.h"
#include <wchar.h>
#include "debug.h"


/**
 * @brief memset 的16位版本 
 *  
 *  和 \b wcsmemset
 *  函数功能一样，但是某些库的wcsmemset函数没有针对ARM处理器
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
void  memset16(void *s,unsigned short val, size_t16 n){
    wmemset((wchar_t *) s, val, n);
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


