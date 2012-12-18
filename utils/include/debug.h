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

#endif








