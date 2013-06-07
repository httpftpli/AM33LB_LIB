#ifndef  __PF_UART_H__
#define  __PF_UART_H__


typedef void (*UARTRCVHANDLER)(void *rcvBuf,unsigned int bufLen);


extern void uartInit(unsigned int moduleId, unsigned int boudRate, 
              unsigned int charLen,  unsigned int parityFlag,
              unsigned int stopBit,  unsigned int intFlag,
              unsigned int rxFifoLen,unsigned int txFiloLen);

extern void UARTRcvRegistHander(UARTRCVHANDLER handler);
extern void UART0ModuleClkConfig(void);



#endif
