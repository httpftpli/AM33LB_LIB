/**
 *  \file   pf_key_touchpad.c
 *
 *  \brief
 *  \#include  "pf_key_touchpad.h"
 *  \author  lfl
 *  \addtogroup KEY_TOUCHPAD
 *  @{
 *
 */



#include "pf_key_touchpad.h"
#include "pf_platform_cfg.h"
#include "pf_bootloader.h"
#include "type.h"
#include "pf_lcd.h"
#include "pf_uart.h"
#include "mmcsd_proto.h"
#include "string.h"
#include "delay.h"
#include "mmath.h"
#include "lib_gui.h"
#include "atomic.h"
#include <wchar.h>
#include <stdio.h>

#if 0   //protcal hengji  tuji
#define KEYSCANCODE_0     0x0204
#define KEYSCANCODE_1     0x0100
#define KEYSCANCODE_2     0x0101
#define KEYSCANCODE_3     0x0102
#define KEYSCANCODE_4     0x0103
#define KEYSCANCODE_5     0x0104
#define KEYSCANCODE_6     0x0200
#define KEYSCANCODE_7     0x0201
#define KEYSCANCODE_8     0x0202
#define KEYSCANCODE_9     0x0203
#define KEYSCANCODE_DOT   0x0404
#define KEYSCANCODE_ZF    0x0403
#define KEYSCANCODE_A     0x0300
#define KEYSCANCODE_B     0x0301
#define KEYSCANCODE_C     0x0302
#define KEYSCANCODE_D     0x0303
#define KEYSCANCODE_E     0x0304
#define KEYSCANCODE_F     0x0305
#define KEYSCANCODE_F1    0x0000
#define KEYSCANCODE_F2    0x0001
#define KEYSCANCODE_F3    0x0002
#define KEYSCANCODE_F4    0x0003
#define KEYSCANCODE_F5    0x0004
#define KEYSCANCODE_F6    0x0005
#define KEYSCANCODE_RIGHT 0x0107
#define KEYSCANCODE_LEFT  0x0105
#define KEYSCANCODE_UP    0x0006
#define KEYSCANCODE_DOWN  0x0206
#define KEYSCANCODE_QUK   0x0402
#define KEYSCANCODE_ESC   0x0007
#define KEYSCANCODE_ENTER 0x0207
#define KEYSCANCODE_PU    0x0307
#define KEYSCANCODE_PD    0x0306
#define KEYSCANCODE_USB   0x0400
#define KEYSCANCODE_MEM   0x0401
#define KEYSCANCODE_CE    0x0106
#define KEYSCANCODE_POP   0x0205

#else //protcal waji yujie
#endif


#define CALIBRATION_POINT_OFFSET  20


KEYTOUCHMSG keyTouchpadMsg;

/** @brief 标记是否有按键 */
atomic g_keyPushed;
/**
* @brief
*  标记键盘复位，键盘首次上电或者复位是会发送复位信息
*/
atomic g_keyRest;
/**
* @brief 标记是否有触摸
*/
atomic g_touched;
/**
* @brief 按键值
*/
unsigned char g_keycode;

/**
* @brief 触摸坐标
*/
volatile TS_SAMPLE g_ts;
/**
* @brief 摸触AD采样值
*/

volatile TS_SAMPLE g_tsRaw;
TS_CALIBRATION tsCalibration = {.magic= 0,};

void (*keyhandler)(int keycode) = NULL;
void (*touchhandler)(void) = NULL;

#define SAMPLES       8
#define CALIBRATION_POINT_OFFSET  20

extern mmcsdCtrlInfo mmcsdctr[2];

BOOL TouchCalibrate(BOOL force);



#if 0 //protcal hengji tuji
BOOL isKeyTouchEvent(KEYTOUCHMSG *msg){
   if (0x55 == msg->magic) {
      return TRUE;
   }
   return FALSE;
}
#else  //protcal new waji yujie
BOOL isKeyTouchEvent(KEYTOUCHMSG *msg){
   if ((0xbb==msg->type)&&(1 == msg->magic)&&(0xd==msg->end)) {
      return TRUE;
   }
   return FALSE;
}
#endif



void registKeyHandler(void handler(int keycode)){
    keyhandler = handler;
}


unsigned char (*keyscancode2key)(int keyscancode) = NULL;
void registKeyMap(unsigned char scancodeToKey(int keyscancode)){
    keyscancode2key  = scancodeToKey;
}


void registTouchHandler(void handler()){
  touchhandler = handler;
}



void  ts_linear(TS_CALIBRATION *cal,  int *x,  int *y) {
   *x  = (cal->matrix.An * (int)(*x)) / 1000 + cal->matrix.Bn * (int)(*y) / 1000 + cal->matrix.Cn;
   *y  = (cal->matrix.Dn * (int)(*x)) / 1000 + cal->matrix.En * (int)(*y) / 1000 + cal->matrix.Fn;
}



BOOL TouchCalibrate(BOOL  force) {
#define CALIBRATION_SUCCESS   0x55555555
#define CALIBRATION_FAIL    0xAAAAAAAA
    static const unsigned char calIcon[] = { 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
        0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0xFF, 0xFF,
        0xFF, 0xFF, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
        0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80 };

    char inandsecterbuf[512];
    int x1, x2, x3;
    int y1, y2, y3, K;
    int xL1, xL2, xL3;
    int yL1, yL2, yL3;
    int A, B, C, D, E, F;
    int m, n;
    MMCSDP_Read(mmcsdctr, inandsecterbuf, TOUCH_CAL_PARAM_SECTOR, 1);
    memcpy(&tsCalibration, inandsecterbuf, sizeof tsCalibration);
    if ((CALIBRATION_SUCCESS == tsCalibration.magic) && (force == 0)) {
        return TRUE;
    }
    TS_CALIBRATION tsCalibrationTemp;
    memcpy(&tsCalibrationTemp, inandsecterbuf, sizeof tsCalibrationTemp);
    const tLCD_PANEL  *panel = LCDTftInfoGet();
    LCDFbClear(C_BLACK);
    const  TEXTCHAR *disstr =  T("Calibrate touch pad,press any key to skip");
    unsigned int disstrwigth = getStringMetricWidth(disstr);
    drawStringEx(disstr, panel->width / 2 - disstrwigth/2, panel->height / 2 + 50, FONT_ASCII_16, C_WHITE, C_TRANSPARENT);
    tsCalibrationTemp.xfb[0] = 0 + CALIBRATION_POINT_OFFSET;
    tsCalibrationTemp.xfb[1] = panel->width - CALIBRATION_POINT_OFFSET;
    tsCalibrationTemp.xfb[2] = panel->width / 2;
    tsCalibrationTemp.xfb[3] = panel->width / 2;


    tsCalibrationTemp.yfb[0] = 0 + CALIBRATION_POINT_OFFSET;
    tsCalibrationTemp.yfb[1] = panel->height / 2;
    tsCalibrationTemp.yfb[2] = panel->height - CALIBRATION_POINT_OFFSET;
    tsCalibrationTemp.yfb[3] = panel->height / 2;
    do {
        for (int i = 0; i < 4; i++) {
            LCDDrawMask(calIcon, tsCalibrationTemp.xfb[i] - 8, tsCalibrationTemp.yfb[i] - 8, 16, 16, C_WHITE, C_TRANSPARENT);
            if (0 == i) {
                delay(200);
                atomicClear(&g_touched);
                atomicClear(&g_keyPushed);
                while (1) {
                    if (atomicTest(&g_touched)) {
                        break;
                    }
                    if (atomicTest(&g_keyPushed)) {
                        return false;
                    }
                }
            } else {
                atomicClear(&g_touched);
                atomicClear(&g_keyPushed);
                while (1) {
                    if (atomicTest(&g_touched)
                        && ((ABS(tsCalibrationTemp.x[i - 1] - g_tsRaw.x) > 800)
                        || (ABS(tsCalibrationTemp.y[i - 1] - g_tsRaw.y) > 800))) {
                        break;
                    }
                    if (atomicTest(&g_keyPushed)) {
                        return false;
                    }
                }
            }
            tsCalibrationTemp.x[i] = g_tsRaw.x;
            tsCalibrationTemp.y[i] = g_tsRaw.y;
            drawRectFillEx(tsCalibrationTemp.xfb[i] - 8, tsCalibrationTemp.yfb[i] - 8, 16, 16, C_BLACK);
        }

        x1 = tsCalibrationTemp.x[0];
        x2 = tsCalibrationTemp.x[1];
        x3 = tsCalibrationTemp.x[2];
        y1 = tsCalibrationTemp.y[0];
        y2 = tsCalibrationTemp.y[1];
        y3 = tsCalibrationTemp.y[2];
        xL1 = tsCalibrationTemp.xfb[0];
        xL2 = tsCalibrationTemp.xfb[1];
        xL3 = tsCalibrationTemp.xfb[2];
        yL1 = tsCalibrationTemp.yfb[0];
        yL2 = tsCalibrationTemp.yfb[1];
        yL3 = tsCalibrationTemp.yfb[2];
        K = (x1 - x2) * (y2 - y3) - (x2 - x3) * (y1 - y2);
        m = (int)((xL1 - xL3) * (y2 - y3));
        n = (int)((xL2 - xL3) * (y1 - y3));
        m = m - n;
        A = ((int)((xL1 - xL2) * (y2 - y3)) - (int)((xL2 - xL3) * (y1 - y2))) * 1000 / K;
        B = ((xL2 - xL3) * (x1 - x2) - (x2 - x3) * (xL1 - xL2)) * 1000 / K;
        C = xL1 - (A * x1 + B * y1) / 1000;
        D = ((yL1 - yL2) * (y2 - y3) - (yL2 - yL3) * (y1 - y2)) * 1000 / K;
        E = ((yL2 - yL3) * (x1 - x2) - (x2 - x3) * (yL1 - yL2)) * 1000 / K;
        F = yL1 - (D * x1 + E * y1) / 1000;
        tsCalibrationTemp.matrix.An = A;
        tsCalibrationTemp.matrix.Bn = B;
        tsCalibrationTemp.matrix.Cn = C;
        tsCalibrationTemp.matrix.Dn = D;
        tsCalibrationTemp.matrix.En = E;
        tsCalibrationTemp.matrix.Fn = F;

        int tempx = tsCalibrationTemp.x[3];
        int tempy = tsCalibrationTemp.y[3];
        ts_linear(&tsCalibrationTemp, &tempx, &tempy);
        int xtol = tempx - tsCalibrationTemp.xfb[3];
        int ytol = tempy - tsCalibrationTemp.yfb[3];
        TEXTCHAR disstrvar[400];
        if ((ABS(xtol) < TS_CALIBRATION_X_TOLERANCE) && (ABS(ytol) < TS_CALIBRATION_Y_TOLERANCE)) {
            //Save_touchData(&Tch_ctrs);
            #if ((CHARACTER_DIS_CODEC == UTF8_CODEC)||(CHARACTER_DIS_CODEC == ASCII_CODEC))
            sprintf(disstrvar, "calibrate success! Xoff:%d  Yoff:%d", xtol, ytol);
            #else
            swprintf(disstrvar,400,T("calibrate success! Xoff:%d  Yoff:%d"), xtol, ytol);
            #endif
            disstrwigth = getStringMetricWidth(disstrvar);
            drawRectFillEx(0,panel->height / 2 + 80,panel->width ,getFontYSize(FONT_ASCII_16),C_BLACK);
            drawStringEx(disstrvar, panel->width / 2 - disstrwigth/2, panel->height / 2 + 80, FONT_ASCII_16, C_WHITE, C_TRANSPARENT);
            delay(1000);
            atomicClear(&g_touched);
            tsCalibrationTemp.magic = CALIBRATION_SUCCESS;
            memcpy(inandsecterbuf, &tsCalibrationTemp, sizeof tsCalibrationTemp);
            MMCSDP_Write(mmcsdctr, inandsecterbuf, TOUCH_CAL_PARAM_SECTOR, 1);
            memcpy(&tsCalibration, &tsCalibrationTemp, sizeof tsCalibrationTemp);
            return TRUE;
        } else {
            #if ((CHARACTER_DIS_CODEC == UTF8_CODEC)||(CHARACTER_DIS_CODEC == ASCII_CODEC))
            sprintf(disstrvar,"calibrate fail! Xoff:%d  Yoff:%d",xtol,ytol);
            #else
            swprintf(disstrvar,400,T("calibrate fail! Xoff:%d  Yoff:%d"),xtol,ytol);
            #endif
            disstrwigth = getStringMetricWidth(disstrvar);
            drawRectFillEx(0,panel->height / 2 + 80,panel->width ,getFontYSize(FONT_ASCII_16),C_BLACK);
            drawStringEx(disstrvar, panel->width / 2 - disstrwigth/2, panel->height / 2 + 80, FONT_ASCII_16, C_WHITE, C_TRANSPARENT);
            delay(2000);
            atomicClear(&g_touched);
        }
    }while (1);
}



/**
 * @brief 
 * @param [in] unsigned short x 
 * @param [in] unsigned short y 
 * @return none 
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void simulateTouch(unsigned short x,unsigned short y){
   g_ts.x = x;
   g_ts.y = y;
   atomicSet(&g_touched);
}


void keyLedSet(unsigned short val){
    unsigned char buf[] = {0xbb,0x08,0x00,0x00,0x00,0x00,0x00,0x0d};
    buf[2] = (unsigned char)val;
    buf[3] = (unsigned char)(val>>8);
    while(!UARTSendNoBlock(UART_LCDBACKLIGHT_MODULE,buf,sizeof buf));
}

void keyLedSet1(unsigned char iled,unsigned char func,unsigned short ontime_ms,unsigned short offtime_ms){
    if(iled>=16) return;
    unsigned char buf[8] = {0xbb,0x09, 0,0,0,0,0,0x0d};
    buf[2] = func<<4|iled;
    buf[3] = (unsigned char )ontime_ms;
    buf[4] = (unsigned char )(ontime_ms>>8);
    buf[5] = (unsigned char )offtime_ms;
    buf[6] = (unsigned char )(offtime_ms>>8);
    while(!UARTSendNoBlock(UART_LCDBACKLIGHT_MODULE,buf,sizeof buf));
}


//! @}



