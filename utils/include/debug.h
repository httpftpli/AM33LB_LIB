/**
*    @file  debug.h
*    @author  lfl
*    @brief  macro for debug output
*
*    ���ļ��к����ڴ�ӡ��ʽ��Ϣ������ͨ�������
*    DEBUG_ERROR_NOTOUTPUT DEBUG_WARN_NOTOUTPUT
*    DEBUG_DEBUG_NOTOUTPUT �ر�
*    @todo
*
*
*
*
*/

#ifndef __debug__h___
#define __debug__h___

#include "uartstdio.h"
#include <stdbool.h>

#ifdef DEBUG_ERROR_NOTOUTPUT
#define mdAssert(message, assertion)  do{if(!(assertion)){while(__LINE__!=0);}}while(0)
#define mdError(A)    while(1);
#else
#define mdAssert(message, assertion)   do{if(!(assertion)){UARTprintf("mdAssert: ("#assertion")   file:%s  line:%d  function:%s\r\n",  __FILE__, __LINE__ ,  __func__ ); while(__LINE__!=0);}}while(0)
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

#define ASSERT(assertion)   mdAssert(" ",assertion )

#define STATIC_ASSERT(A)  static_assert(A, #A)

#define NOT_IN_IRQ() ASSERT(__not_in_irq())

static inline bool __not_in_irq(){
    #pragma section="CSTACK"
    unsigned int __not_in_irq;
    unsigned int valaddr = (unsigned int)&__not_in_irq;
    unsigned int cstackb = (unsigned int)__sfb("CSTACK");
    unsigned int cstacke = (unsigned int)__sfe("CSTACK");
    if (valaddr>=cstackb && valaddr<=cstacke){
        return true;
    }
    return false;
}

#endif


