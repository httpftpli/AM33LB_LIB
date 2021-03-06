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
#include "type.h"
#include "pf_lcd.h"
#include "mmcsd_proto.h"
#include "string.h"
#include "delay.h"
#include "mmath.h"
#include "lib_gui.h"
#include "atomic.h"


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


#define CALIBRATION_POINT_OFFSET  20
#define TOUCH_CAL_PARAM_SECTOR  769

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
TS_CALIBRATION tsCalibration = {.matrix.An=-238,.matrix.Bn=-4,.matrix.Cn=874,
   .matrix.Dn=1,.matrix.En=173,.matrix.Fn=-56};

void (*keyhandler)(int keycode) = NULL;
void (*touchhandler)(void) = NULL;

unsigned int keyCode(unsigned int scancode) {
   unsigned int key;
   switch (scancode) {
   case KEYSCANCODE_0 :
      key =  KEY_0;
      break;
   case KEYSCANCODE_1 :
      key =   KEY_1;
      break;
   case KEYSCANCODE_2 :
      key =   KEY_2;
      break;
   case KEYSCANCODE_3 :
      key =   KEY_3;
      break;
   case KEYSCANCODE_4  :
      key =   KEY_4;
      break;
   case KEYSCANCODE_5  :
      key =   KEY_5;
      break;
   case KEYSCANCODE_6  :
      key =   KEY_6;
      break;
   case KEYSCANCODE_7 :
      key =   KEY_7;
      break;
   case KEYSCANCODE_8 :
      key =   KEY_8;
      break;
   case KEYSCANCODE_9 :
      key =   KEY_9;
      break;
   case KEYSCANCODE_DOT:
      key =   KEY_POINT;
      break;
   case KEYSCANCODE_ZF :
      key =   KEY_ZF;
      break;
   case KEYSCANCODE_A  :
      key =   KEY_A;
      break;
   case KEYSCANCODE_B :
      key =   KEY_B;
      break;
   case KEYSCANCODE_C  :
      key =   KEY_C;
      break;
   case KEYSCANCODE_D :
      key =   KEY_D;
      break;
   case KEYSCANCODE_E :
      key =   KEY_E;
      break;
   case KEYSCANCODE_F :
      key =  KEY_F;
      break;
   case KEYSCANCODE_F1:
      key =   KEY_F1;
      break;
   case KEYSCANCODE_F2 :
      key =   KEY_F2;
      break;
   case KEYSCANCODE_F3 :
      key =   KEY_F3;
      break;
   case KEYSCANCODE_F4 :
      key =  KEY_F4;
      break;
   case KEYSCANCODE_F5 :
      key = KEY_F5;
      break;      
   case KEYSCANCODE_F6:
      key = KEY_F6;
      break; 
   case KEYSCANCODE_RIGHT:
      key =   KEY_RIGHT;
      break;
   case KEYSCANCODE_LEFT :
      key =   KEY_LEFT;
      break;
   case KEYSCANCODE_UP :
      key = KEY_UP;
      break;
   case KEYSCANCODE_DOWN:
      key =   KEY_DOWN;
      break;
   case KEYSCANCODE_QUK :
      key =   KEY_QUK;
      break;
   case KEYSCANCODE_ESC :
      key =   KEY_ESC;
      break;
   case KEYSCANCODE_ENTER:
      key =   KEY_OK;
      break;
   case KEYSCANCODE_PU :
      key =   KEY_PU;
      break;
   case KEYSCANCODE_PD :
      key =   KEY_PD;
      break;
   case KEYSCANCODE_USB:
      key =   KEY_USB;
      break;
   case KEYSCANCODE_CE :
      key =   KEY_CE;
      break;
   case KEYSCANCODE_MEM :
      key =   KEY_MEM;
      break;
   case KEYSCANCODE_POP :
      key =   KEY_POP;
      break;
   default:
      key =   KEY_NO;
      break;
   }
   return key;
}

#define SAMPLES       8
#define CALIBRATION_POINT_OFFSET  20

extern mmcsdCtrlInfo mmcsdctr[2];

BOOL TouchCalibrate(BOOL force);



BOOL isKeyTouchEvent(KEYTOUCHMSG *msg){
   if (0x55 == msg->magic) {
      return TRUE;
   }
   return FALSE;
}



/**
 * @brief 注册特定按键的中断回调函数
 * @param [in] handler 回调函数
 * @return NONE          
 * @date    2013/7/8
 * @note 
 * 注册的回调函数可用于处理全局快捷键 
 */
void registKeyHandler(void handler(int keycode)){
  keyhandler = handler;
}

/**
 * @brief 注册触摸屏的回调中断回调函数，用于全局快捷触摸功能，在没有键盘的时候当快捷键使用
 * @param [in] handler 回调函数
 * @return NONE          
 * @date    2013/7/8
 * @note 
 * 注册的回调函数可用于处理全局快捷键 
 */
void registTouchHandler(void handler()){
  touchhandler = handler;
}



void  ts_linear(TS_CALIBRATION *cal,  int *x,  int *y) {
   *x  = (cal->matrix.An * (int)(*x)) / 1000 + cal->matrix.Bn * (int)(*y) / 1000 + cal->matrix.Cn;
   *y  = (cal->matrix.Dn * (int)(*x)) / 1000 + cal->matrix.En * (int)(*y) / 1000 + cal->matrix.Fn;
}


/**
 * @brief 触摸屏校准 
 *        ,该函数只校准一次，如果校准不成功函数返回FALSE,需重新调用该函数
 * @param force 
 *         - FALSE --
 *         当前触摸参数有效时读取当前校准参数
 *         \n
 *         - TRUE --
 *           不管当前校准参数有没有效，都强制重新校准，如果校准成功保存当前参数
 *           \n
 * @return 
 * - TRUE 成功 \n 
 * - FALSE 失败 \n 
 *  
 * @date    2013/5/31
 * @note 
 * 该函数会修改显存 
 * @code
 * @endcode
 * @pre
 * @see 
 */
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
   MMCSDP_Read(mmcsdctr,inandsecterbuf,TOUCH_CAL_PARAM_SECTOR,1);
   memcpy(&tsCalibration,inandsecterbuf,sizeof tsCalibration ); 
   if ((CALIBRATION_SUCCESS == tsCalibration.magic)&&(force==0)) {
      return TRUE;
   }
   const tLCD_PANEL  *panel = LCDTftInfoGet();
   LCDFbClear(C_BLACK);
   drawStringEx("calibrate touch pad", panel->width / 2 - 100, panel->height / 2 + 50, FONT_ASCII_16, C_WHITE, C_TRANSPARENT);
   tsCalibration.xfb[0] = 0 + CALIBRATION_POINT_OFFSET;
   tsCalibration.xfb[1] = panel->width - CALIBRATION_POINT_OFFSET;
   tsCalibration.xfb[2] = panel->width / 2;
   tsCalibration.xfb[3] = panel->width / 2;


   tsCalibration.yfb[0] = 0 + CALIBRATION_POINT_OFFSET;
   tsCalibration.yfb[1] = panel->height / 2;
   tsCalibration.yfb[2] = panel->height - CALIBRATION_POINT_OFFSET;
   tsCalibration.yfb[3] = panel->height / 2;
   for (int i = 0; i < 4; i++) {      
      LCDDrawMask(calIcon, tsCalibration.xfb[i] - 8, tsCalibration.yfb[i] - 8, 16, 16, C_WHITE, C_TRANSPARENT);
      if (0 == i) {        
         delay(200);
         atomicClear(&g_touched);
         while (!atomicTest(&g_touched));
      } else {
         atomicClear(&g_touched);
         while (!(atomicTest(&g_touched) && ((ABS(tsCalibration.x[i-1] - g_tsRaw.x) > 800) || ( ABS(tsCalibration.y[i-1] - g_tsRaw.y) > 800))));
      }
      tsCalibration.x[i] = g_tsRaw.x;
      tsCalibration.y[i] = g_tsRaw.y;
      drawRectFillEx(tsCalibration.xfb[i] - 8, tsCalibration.yfb[i] - 8, 16, 16, C_BLACK);
   }

   x1 = tsCalibration.x[0];
   x2 = tsCalibration.x[1];
   x3 = tsCalibration.x[2];
   y1 = tsCalibration.y[0];
   y2 = tsCalibration.y[1];
   y3 = tsCalibration.y[2];
   xL1 = tsCalibration.xfb[0];
   xL2 = tsCalibration.xfb[1];
   xL3 = tsCalibration.xfb[2];
   yL1 = tsCalibration.yfb[0];
   yL2 = tsCalibration.yfb[1];
   yL3 = tsCalibration.yfb[2];
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
   tsCalibration.matrix.An = A;
   tsCalibration.matrix.Bn = B;
   tsCalibration.matrix.Cn = C;
   tsCalibration.matrix.Dn = D;
   tsCalibration.matrix.En = E;
   tsCalibration.matrix.Fn = F;

   int tempx = tsCalibration.x[3];
   int tempy = tsCalibration.x[3];
   ts_linear(&tsCalibration, &tempx, &tempy);
   if ((ABS(tempx - tsCalibration.xfb[3]) < 16) && (ABS(tempy - tsCalibration.yfb[3]) < 10)) {
      //Save_touchData(&Tch_ctrs);
      drawStringEx("calibrate success", panel->width / 2 - 100, panel->height / 2 + 75, FONT_ASCII_16,C_WHITE, C_TRANSPARENT);
      delay(1000);
      atomicClear(&g_touched);
      tsCalibration.magic = CALIBRATION_SUCCESS;
      memcpy(inandsecterbuf,&tsCalibration,sizeof tsCalibration );
      MMCSDP_Write(mmcsdctr,inandsecterbuf,TOUCH_CAL_PARAM_SECTOR,1);
      return TRUE;
   } else {
      drawStringEx("calibrate fail", panel->width / 2 - 100, panel->height / 2 + 75, FONT_ASCII_16,C_WHITE, C_TRANSPARENT);
      delay(1000);
      atomicClear(&g_touched);
      return FAIL;
   }
}




/**
 * @brief 软件触发触摸
 * @param [in] unsigned short x  校准以后的x坐标 
 * @param [in] unsigned short y  校准以后的y坐标 
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


//! @}



