/**
*    @file  debug.h
*    @author  lfl
*    @brief  macro for debug output
*  
*    该文件中宏用于打印调式信息，可能通过定义宏
*    DEBUG_ERROR_NOTOUTPUT DEBUG_WARN_NOTOUTPUT
*    DEBUG_DEBUG_NOTOUTPUT 关闭
*    @todo
*
*
*
*
*/

#ifndef __debug__h___
#define __debug__h___

#include "uartstdio.h"

#ifdef DEBUG_ERROR_NOTOUTPUT
#define mdAssert(A)  (void)0
#define mdError(A)   (void)0
#else
#define mdAssert(A)   do{if(!(A)){UARTprintf("mdAssert: ("#A")   file:%s  line:%d  function:%s\r\n",  __FILE__, __LINE__ ,  __func__ ); while(__LINE__!=0);}}while(0)
#define mdError(A)    do{UARTprintf("mdError: %s   file:%s  line:%d  function:%s\r\n", A, __FILE__, __LINE__  , __func__ ); while(__LINE__!=0);} while(0)
#endif


#ifdef DEBUG_WARN_NOTOUTPUT
#define mdWarn(A)    (void)0
#else
#define mdWarn(A)    UARTprintf("mdWarn: %s   file:%s  line:%d  function:%s\r\n",  A,__FILE__, __LINE__  , __func__ )
#endif

#ifdef DEBUG_DEBUG_NOTOUTPUT
#define mdDebug(A)    (void)0
#define mdDebugNum(A) (void)0
#else
#define mdDebug(A)   UARTPuts((A), -1)
#define mdDebugNum(A) UARTPutNum((A))
#endif

#define ASSERT(A)   mdAssert(A)

#define STATIC_ASSERT(A)  static_assert(A, #A) 

#endif


