/**
 *  \file   pf_tsc.c
 *
 *  \brief
 *  \author  李飞亮  
 *  \addtogroup GUI
 *  @{ 
 *   
 */

#include "soc_AM335x.h"
#include "hw_types.h"
#include "platform.h"
#include "type.h"

#include "tsc_adc.h"
#include "pf_lcd.h"
#include "event.h"
#include "algorithm.h"
#include "hw_types.h"
#include "hw_cm_wkup.h"
#include "module.h"
#include "mmath.h"
#include "atomic.h"
#include "GUI.h"
#include "pf_tsc.h"

#define SAMPLES       8
#define CALIBRATION_POINT_OFFSET  20


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


static TS_CALIBRATION tsCalibration = { .magic = 0xaaaaaaaa };


BOOL TouchCalibrate(void);

void loadCalibration() {
   if (tsCalibration.magic != 0x55555555) TouchCalibrate();
}

volatile static TS_SAMPLE tsSampleRaw,ts;
volatile TS_SAMPLE ts;
atomic touched = 0;
volatile static unsigned int tsenable = 1;

static void StepEnable(void);
static void ts_linear(TS_CALIBRATION *cal,  int *x,  int *y);

//static void ts_linear_scale(int *x, int *y, int swap_xy);

void  isr_tsc(unsigned int intnum) {
   unsigned int wordsLeftX, wordsLeftY, error = 0, samplefinish = 0;
   volatile int dump;
   unsigned int status;
   int arr_x[SAMPLES];
   int arr_y[SAMPLES];
   static volatile TS_SAMPLE preTsSampleRaw = { 0, 0 };

   status = TSCADCIntStatus(SOC_ADC_TSC_0_REGS);
   TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS, status);
   if (status & TSCADC_PEN_UP_EVENT_INT) {
      preTsSampleRaw.x = 0;
   }
   if (status & TSCADC_FIFO1_THRESHOLD_INT) {
      samplefinish = 1;
   }
   if (1 == samplefinish) {
      wordsLeftX = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
      wordsLeftY = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
      if ((wordsLeftX != wordsLeftY) || (wordsLeftY != SAMPLES)) {
         error = 1;
         for (int i = 0; i < wordsLeftX; i++) {
            dump = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
         }
         for (int i = 0; i < wordsLeftY; i++) {
            dump = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
         }
      } else {
         for (int i = 0; i < wordsLeftX; i++) {
            arr_x[i] =  TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
         }
         for (int i = 0; i < wordsLeftY; i++) {
            arr_y[i] = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
         }
      }
      StepEnable();
   }
   if ((1 == tsenable) && (0 == error) && (1 == samplefinish)) {
      bubbleSortAscend(arr_x, SAMPLES);
      bubbleSortAscend(arr_y, SAMPLES);
      if (0 == preTsSampleRaw.x) {
         preTsSampleRaw.x = 1; //sum(arr_x+2 , SAMPLES-4 ) / (SAMPLES-4 );
         preTsSampleRaw.y = 1; //sum(arr_y+2 , SAMPLES-4 ) / (SAMPLES-4 );
      } else if (1 == preTsSampleRaw.x) {
         preTsSampleRaw.x = sum(arr_x + 2, SAMPLES - 4) / (SAMPLES - 4);
         preTsSampleRaw.y = sum(arr_y + 2, SAMPLES - 4) / (SAMPLES - 4);
      } else {
         ts.x = tsSampleRaw.x =  preTsSampleRaw.x;
         ts.y = tsSampleRaw.y =  preTsSampleRaw.y;
         preTsSampleRaw.x = sum(arr_x + 2, SAMPLES - 4) / (SAMPLES - 4);
         preTsSampleRaw.y = sum(arr_y + 2, SAMPLES - 4) / (SAMPLES - 4);
         ts_linear(&tsCalibration,(int*)&(ts.x),(int *)&(ts.y));
         atomicSet(&touched);

         //MSG msg;
         //msg.message = MSG_TOUCH;
         //msg.xpt = sampletem.x;
         //msg.ypt = sampletem.y;
         //UARTprintf("x: %d  ; y: %d  ;", tsSampleRaw.x, tsSampleRaw.y);
         //UARTprintf("fbx: %d  ; fby: %d \n\r", msg.xpt, msg.ypt);
         //SendMessage(&msg);*/
      }
   }
   if (status & TSCADC_PEN_UP_EVENT_INT) {
      //UARTprintf("pen up \n\r");
   }
}


void TSCADCModuleClkConfig(void) {
   /* Writing to MODULEMODE field of CM_WKUP_TSC_CLKCTRL register. */
   HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) |=
      CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE;

   /* Waiting for MODULEMODE field to reflect the written value. */
   while (CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE !=
             (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
                 CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE));

   /*
   ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
   ** desired value.
   */
   while ((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
             (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
                 CM_WKUP_CONTROL_CLKCTRL_IDLEST));

   while (CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK !=
             (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
                 CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK));

   /*
   ** Waiting for IDLEST field in CM_WKUP_ADC_TSC_CLKCTRL register to attain
   ** desired value.
   */
   while ((CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_SHIFT) !=
             (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
                 CM_WKUP_ADC_TSC_CLKCTRL_IDLEST));
}

static void IdleStepConfig(void) {
   /* Configure ADC to Single ended operation mode */
   TSCADCIdleStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                      TSCADC_SINGLE_ENDED_OPER_MODE);
   /* Configure reference volatage and input to idlestep */
   TSCADCIdleStepConfig(SOC_ADC_TSC_0_REGS, TSCADC_NEGATIVE_REF_VSSA,
                        TSCADC_POSITIVE_INP_CHANNEL1, TSCADC_NEGATIVE_INP_ADCREFM,
                        TSCADC_POSITIVE_REF_VDDA);
   /* Configure the Analog Supply to Touch screen */
   TSCADCIdleStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                    TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF);
   /* 
   **Configure the Analong Ground of Touch screen.
   */
   TSCADCIdleStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                    TSCADC_YPNSW_PIN_ON, TSCADC_YNNSW_PIN_ON,
                                    TSCADC_WPNSW_PIN_OFF);
}

static void TSchargeStepConfig(void) {
   /* Configure ADC to Single ended operation mode */
   TSCADCChargeStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                        TSCADC_SINGLE_ENDED_OPER_MODE);
   /* Configure reference volatage and input to idlestep */
   TSCADCChargeStepConfig(SOC_ADC_TSC_0_REGS, TSCADC_NEGATIVE_REF_VSSA,
                          TSCADC_POSITIVE_INP_CHANNEL1, TSCADC_NEGATIVE_INP_ADCREFM,
                          TSCADC_POSITIVE_REF_VDDA);
   /* Configure the Analog Supply to Touch screen */
   TSCADCChargeStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                      TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF);
   /* 
   **Configure the Analong Ground of Touch screen.
   */
   TSCADCChargeStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                      TSCADC_YPNSW_PIN_ON, TSCADC_YNNSW_PIN_ON,
                                      TSCADC_WPNSW_PIN_OFF);
   TSCADCTSChargeStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, 0x200);

   /* Configure ADC to Single ended operation mode */
   /*TSCADCChargeStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                        TSCADC_SINGLE_ENDED_OPER_MODE);*/
   /* Configure reference volatage and input to charge step*/
   /*TSCADCChargeStepConfig(SOC_ADC_TSC_0_REGS, TSCADC_NEGATIVE_REF_XNUR,
                          TSCADC_POSITIVE_INP_CHANNEL2, TSCADC_NEGATIVE_INP_CHANNEL2,
                          TSCADC_POSITIVE_REF_XPUL);*/
   /* Configure the Analog Supply to Touch screen */
   /*TSCADCChargeStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_ON,
                                      TSCADC_XNPSW_PIN_OFF, TSCADC_XPPSW_PIN_OFF);*/
   /* Configure the Analong Ground to Touch screen */
   /*TSCADCChargeStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                      TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_ON,
                                      TSCADC_WPNSW_PIN_OFF);*/
   /*TSCADCTSChargeStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, 0x200);*/
}

static void StepConfigX(unsigned int stepSelc) {
   /* Configure ADC to Single ended operation mode */
   TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                    TSCADC_SINGLE_ENDED_OPER_MODE, stepSelc);

   /* Configure reference volatage and input to charge step*/
   TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_NEGATIVE_REF_VSSA,
                      TSCADC_POSITIVE_INP_CHANNEL3, TSCADC_NEGATIVE_INP_CHANNEL1,
                      TSCADC_POSITIVE_REF_VDDA);

   /* Configure the Analog Supply to Touch screen */
   TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_ON,
                                  TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF,
                                  stepSelc);

   /* Configure the Analong Ground to Touch screen */
   TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_ON,
                                  TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_OFF,
                                  TSCADC_WPNSW_PIN_OFF, stepSelc);

   /* select fifo 0 */
   TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_FIFO_0);

   /* Configure in One short hardware sync mode */
   TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_ONE_SHOT_HARDWARE_SYNC);

   TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_SIXTEEN_SAMPLES_AVG);
}


static void StepConfigY(unsigned int stepSelc) {

   /* Configure ADC to Single ended operation mode */
   TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                    TSCADC_SINGLE_ENDED_OPER_MODE, stepSelc);

   /* Configure reference volatage and input to charge step*/
   TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_NEGATIVE_REF_VSSA,
                      TSCADC_POSITIVE_INP_CHANNEL1, TSCADC_NEGATIVE_INP_ADCREFM,
                      TSCADC_POSITIVE_REF_VDDA);

   /* Configure the Analog Supply to Touch screen */
   TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                  TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_ON, stepSelc);

   /* Configure the Analong Ground to Touch screen */
   TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                  TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_ON,
                                  TSCADC_WPNSW_PIN_OFF, stepSelc);

   /* select fifo 1 */
   TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_FIFO_1);

   /* Configure in One short hardware sync mode */
   TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_ONE_SHOT_HARDWARE_SYNC);

   TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_SIXTEEN_SAMPLES_AVG);
}

static void StepEnable(void) {
   for (int i = 0; i < (SAMPLES * 2 + 1); i++) {
      TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i, 1);
   }
}


void tsEnalbe(void) {
   tsenable = 1;
   delay(10);
}

void tsDisable(void) {
   tsenable = 0;
}


static void ts_linear(TS_CALIBRATION *cal,  int *x,  int *y) {
   *x  = (cal->matrix.An * (int)(*x)) / 1000 + cal->matrix.Bn * (int)(*y) / 1000 + cal->matrix.Cn;
   *y  = (cal->matrix.Dn * (int)(*x)) / 1000 + cal->matrix.En * (int)(*y) / 1000 + cal->matrix.Fn;
}



/**
 * @brief 触摸屏校准 
 * @return           
 * @date    2013/5/31
 * @note 
 * 该函数会修改显存 
 * @code
 * @endcode
 * @pre
 * @see 
 */
BOOL TouchCalibrate(void) {
   static const unsigned char calIcon[] = { 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
      0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0xFF, 0xFF,
      0xFF, 0xFF, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
      0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80 };

   int x1, x2, x3;
   int y1, y2, y3, K;
   int xL1, xL2, xL3;
   int yL1, yL2, yL3;
   int A, B, C, D, E, F;
   int m, n;

   const tLCD_PANEL  *panel = LCDTftInfoGet();
   Dis_Clear(C_Black);
   Dis_String("calibrate touch pad", panel->width / 2 - 100, panel->height / 2 + 50, 0, C_White, C_TRANSPARENT);
   tsCalibration.xfb[0] = 0 + CALIBRATION_POINT_OFFSET;
   tsCalibration.xfb[1] = panel->width - CALIBRATION_POINT_OFFSET;
   tsCalibration.xfb[2] = panel->width / 2;
   tsCalibration.xfb[3] = panel->width / 2;


   tsCalibration.yfb[0] = 0 + CALIBRATION_POINT_OFFSET;
   tsCalibration.yfb[1] = panel->height / 2;
   tsCalibration.yfb[2] = panel->height - CALIBRATION_POINT_OFFSET;
   tsCalibration.yfb[3] = panel->height / 2;
   for (int i = 0; i < 4; i++) {
      atomicClear(&touched);
      Dis_DrawMask(calIcon, tsCalibration.xfb[i] - 8, tsCalibration.yfb[i] - 8, 16, 16, C_White, C_TRANSPARENT);
      if (0 == i) {
         while (!atomicTest(&touched));
      } else {                                      
         while (!(atomicTest(&touched) && ((ABS(tsCalibration.x[i-1] - tsSampleRaw.x) > 800) || ( ABS(tsCalibration.y[i-1] - tsSampleRaw.y) > 800))));
      }
      tsCalibration.x[i] = tsSampleRaw.x;
      tsCalibration.y[i] = tsSampleRaw.y;
      Dis_RectFill(tsCalibration.xfb[i] - 8, tsCalibration.yfb[i] - 8, 16, 16, C_Black);
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
   if ((ABS(tempx - tsCalibration.xfb[3]) < 30) && (ABS(tempy - tsCalibration.yfb[3]) < 30)) {
      //Save_touchData(&Tch_ctrs);
      Dis_String("calibrate success", panel->width / 2 - 100, panel->height / 2 + 75, 0,C_White, C_TRANSPARENT);
      delay(1000);
      return TRUE;
   } else {
      Dis_String("calibrate fail", panel->width / 2 - 100, panel->height / 2 + 75, 0,C_White, C_TRANSPARENT);
      delay(1000);
      return FAIL;
   }

}




void TouchScreenInit() {
   MODULE *module = modulelist + MODULE_ID_ADCTSC;
   unsigned int baseaddr = module->baseAddr;
   moduleEnable(MODULE_ID_ADCTSC);
   TSCADCConfigureAFEClock(baseaddr, module->moduleClk->fClk[0]->clockSpeedHz, 600000);
   /* Enable Transistor bias */
   TSCADCTSTransistorConfig(baseaddr, TSCADC_TRANSISTOR_ENABLE);
   /* Map hardware event to Pen Touch IRQ */
   TSCADCHWEventMapSet(baseaddr, TSCADC_PEN_TOUCH);
   /* Set 4 Wire touch screen  mode */
   TSCADCTSModeConfig(baseaddr, TSCADC_FOUR_WIRE_MODE);
   TSCADCStepIDTagConfig(baseaddr, 1);
   /* Disable Write Protection of Step Configuration regs*/
   TSCADCStepConfigProtectionDisable(baseaddr);
   /* Touch Screen detection Configuration*/
   IdleStepConfig();
   /* Configure the Charge step */
   TSchargeStepConfig();
   for (int i = 0; i < SAMPLES; i++) {
      StepConfigX(i);
      TSCADCTSStepOpenDelayConfig(baseaddr, i, 0x90);
      TSCADCTSStepSampleDelayConfig(baseaddr, i, 0);
   }

   for (int i = SAMPLES; i < (2 * SAMPLES); i++) {
      StepConfigY(i);
      TSCADCTSStepOpenDelayConfig(baseaddr, i, 0x90);
      TSCADCTSStepSampleDelayConfig(baseaddr, i, 0);
   }

   TSCADCFIFOIRQThresholdLevelConfig(baseaddr, TSCADC_FIFO_1, SAMPLES);
   TSCADCModuleStateSet(baseaddr, TSCADC_MODULE_ENABLE);
   StepEnable();
   TSCADCEventInterruptEnable(baseaddr,  TSCADC_FIFO1_THRESHOLD_INT | TSCADC_PEN_UP_EVENT_INT); //|TSCADC_PEN_UP_EVENT_INT|TSCADC_SYNC_PEN_EVENT_INT
   moduleIntConfigure(MODULE_ID_ADCTSC);
}


//! @}





/************************************/




