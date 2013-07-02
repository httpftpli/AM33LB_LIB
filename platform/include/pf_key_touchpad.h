#ifndef __PF_KEY_TOUCHPAD_H__
#define __PF_KEY_TOUCHPAD_H__

#include "atomic.h"
#include "type.h"

#define MSG_TYPE_KEY    1
#define MSG_TYPE_TOUCH  2
#define MSG_TYPE_KEYRESET 4

#define KEY_0        	0  	
#define KEY_1        	1
#define KEY_2        	2
#define KEY_3        	3
#define KEY_4       	4
#define KEY_5        	5
#define KEY_6        	6
#define KEY_7        	7
#define KEY_8        	8
#define KEY_9        	9
#define KEY_A		10
#define KEY_B		11
#define KEY_C		12
#define KEY_D		13
#define KEY_E		14
#define KEY_F		15

#define KEY_UP      16
#define KEY_DOWN    17
#define KEY_LEFT    18
#define KEY_RIGHT   19
#define KEY_PU		20
#define KEY_PD		21

#define KEY_OK      22
#define KEY_ESC     23
#define KEY_F1		24
#define KEY_F2		25
#define KEY_F3		26
#define KEY_F4		27
#define KEY_F5		28
#define KEY_F6		29
#define KEY_CE		30		
#define KEY_POP		31

#define KEY_USB		32
#define KEY_MEM		33
#define KEY_QUK		34
#define KEY_ZF		35
#define KEY_POINT	36

#define KEY_RE1		37
#define KEY_RE2		38
#define KEY_RE3		39
#define KEY_NO		0xff


#pragma pack(1)
typedef struct __message{
  unsigned char type;
  unsigned char magic;
  unsigned short keycode;
  unsigned int tscval;
} KEYTOUCHMSG;
#pragma pack()

typedef struct ts_sample {
    int       x;
    int       y;
    int timetick;
} TS_SAMPLE;

typedef struct {
   int An;
   int Bn;
   int Cn;
   int Dn;
   int En;
   int Fn;
   int Divider;
}MATRIX;

typedef struct {
   unsigned int magic;   //0x55555555 flag  CALIBRATION data valid
   int  x[5], xfb[5];
   int  y[5], yfb[5];
   MATRIX  matrix;
} TS_CALIBRATION;

extern KEYTOUCHMSG keyTouchpadMsg;
extern atomic g_keyPushed;
extern atomic g_keyRest;
extern atomic g_touched;
extern unsigned char g_keycode;
extern volatile TS_SAMPLE g_ts ,g_tsRaw;
extern TS_CALIBRATION tsCalibration;

extern BOOL isKeyTouchEvent(KEYTOUCHMSG *msg);
extern unsigned int keyCode(unsigned int scancode);
extern void registKeyHandler(void handler(int keycode));
extern BOOL TouchCalibrate(BOOL force);
extern void ts_linear(TS_CALIBRATION *cal,  int *x,  int *y);

#endif

