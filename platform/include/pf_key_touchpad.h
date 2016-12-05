#ifndef __PF_KEY_TOUCHPAD_H__
#define __PF_KEY_TOUCHPAD_H__

#include "atomic.h"
#include "type.h"

#define MSG_TYPE_KEY    1
#define MSG_TYPE_TOUCH  2
#define MSG_TYPE_KEYRESET 4



#pragma pack(1)
#if 0 //protal hengji tuji
typedef struct __message{
  unsigned char type;
  unsigned char magic;
  unsigned short keycode;
  unsigned int tscval;
} KEYTOUCHMSG;
#else //protal new wanji yujie
typedef struct __message{
  unsigned char type;
  unsigned char magic;
  unsigned char keycode;
  unsigned int tscval;
  unsigned char end;
} KEYTOUCHMSG;
#endif
#pragma pack()

typedef struct ts_sample {
    int       x;
    int       y;
    int timetick;
} TS_SAMPLE;



typedef struct {
   unsigned int magic;   //0x55555555 flag  CALIBRATION data valid
   int  x[5], xfb[5];
   int  y[5], yfb[5];
   int  a[7];
   unsigned int  xres,yres;
} TS_CALIBRATION;


extern unsigned char  (*keyscancode2key)(int keyscancode);
extern KEYTOUCHMSG keyTouchpadMsg;
extern atomic g_keyPushed;
extern atomic g_keyRest;
extern atomic g_touched;
extern unsigned char g_keycode;
extern volatile TS_SAMPLE g_ts ,g_tsRaw;
extern TS_CALIBRATION tsCalibration;

extern BOOL isKeyTouchEvent(KEYTOUCHMSG *msg);
extern void registKeyHandler(void handler(int keycode));
extern void registTouchHandler(void handler());
extern BOOL isTouched();
extern void touchRegistProcess( void (*process)(void));
extern BOOL TouchCalibrate(BOOL force);
extern void ts_linear(TS_CALIBRATION *cal,  int *x,  int *y);
extern void simulateTouch(unsigned short x,unsigned short y);
extern void keyLedSet(unsigned short mask ,unsigned short val);
extern void keyLedSet1(unsigned char iled,unsigned char func,unsigned short ontime_ms,unsigned short offtime_ms);
extern void registKeyMap(unsigned char scancodeToKey(int keyscancode));
#endif

