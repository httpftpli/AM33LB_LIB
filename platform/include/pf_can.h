#ifndef __PF__DCAN__H__
#define __PF__DCAN__H__



#include <stdbool.h>

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


#if 1
typedef struct {
	unsigned int id:29;    //MSB first send ,
	unsigned int dir:1;
	unsigned int xtd:1;	//1:extend frame ,statand frame
	unsigned int dlc;
	unsigned int data[2];
        //if data[0] = 0x01234567 ,data[1] = 0x89abcdef £¬
        //then CAN data sequence is ¡°0x67 0x45,0x23,0x01,0xef,0xcd,0xab,0x89¡±
}CAN_FRAME;

#else
typedef struct
{
	uint32 Command:13;						//?¨¹????
	uint32 SourceId:8;							//??ID
	uint32 TargetId:8;							//??¡À¨ºID
	uint32 dir:1;
	uint32 xtd:1;								//??????¡À¨º??,0=¡À¨º¡Á???,1=??????
	uint32 dlc;								//??????????
	uint32 data[2];
}CAN_APPFRAME;
#endif

extern volatile G_CAN g_can[2];

extern void DCANInit(unsigned int moduleId,unsigned int mode,unsigned int bitRate);
extern unsigned int CANSend_noblock(unsigned int moduleId,CAN_FRAME *frame);
extern bool CANSendFinishGetClr(unsigned int moduleId);
extern void CANRegistRcvedHandler(void (*handler)(unsigned int index,CAN_FRAME *frame));
#endif
