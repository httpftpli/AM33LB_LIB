#ifndef __PF__DCAN__H__
#define __PF__DCAN__H__

/*
** Values that can be passed to 'CANInit' API as 'mode'
*/
#define CAN_MODE_NORMAL                0
#define CAN_MODE_TEST_LOOPBACK         1
#define CAN_MODE_TEST_LOOPBACK_SILENT  2


/*return val of 'CANSend_noblock'*/
#define CAN_SEND_OK             1
#define CAN_SEND_PRE_SENDING    -1

typedef struct {
   unsigned char RxErrCnt;
   unsigned char TxErrCnt;
   unsigned short LastErrCode;
   unsigned short fgErrWarn:1;
   unsigned short fgErrPassive: 1;
   unsigned short fgBusOff:1;
   unsigned short fgSendFinish:1;
   unsigned short fgDataRcved:1;
}G_CAN;

typedef struct {
   unsigned int id:29;
   unsigned int dir:1;
   unsigned int xtd:1;
   unsigned int dlc;
   unsigned int data[2];
}CAN_FRAME;

extern volatile G_CAN g_can[2];

extern void DCANInit(unsigned int moduleId,unsigned int mode,unsigned int bitRate);
extern unsigned int CANSend_noblock(unsigned int baseAddr,CAN_FRAME *frame);
extern BOOL CANSendFinishGetClr(unsigned int baseAddr);
extern void CANRegistRcvedHandler(void (*handler)(unsigned int index,CAN_FRAME *frame));
#endif
