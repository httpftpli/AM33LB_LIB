#ifndef  __PF_UART_H__
#define  __PF_UART_H__

#include "uart_irda_cir.h"
#include <stdbool.h>

typedef void (*UARTRCVHANDLER)(void *rcvBuf,unsigned int bufLen);


extern void uartInit(unsigned int moduleId, unsigned int boudRate, 
              unsigned int charLen,  unsigned int parityFlag,
              unsigned int stopBit,  unsigned int intFlag,
              unsigned int rxFifoLen,unsigned int txFiloLen);

extern void uartInitFor9Bit(unsigned int moduleId,unsigned int boudRate,
                     unsigned stopBit, unsigned int intFlag,void (*beforSend)(),void (*afterSend)());

extern void UARTRcvRegistHander(UARTRCVHANDLER handler);
extern void UART0ModuleClkConfig(void);
extern void uartSend9Bit(unsigned int moduleId,unsigned short data);
extern bool uartRcv9bit(unsigned int moduleId,unsigned short *pdat,unsigned int timeoutMs);



#endif
