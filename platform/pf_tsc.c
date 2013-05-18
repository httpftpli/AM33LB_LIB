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
#include "gui.h"

#define SAMPLES       8
#define CALIBRATION_POINT_OFFSET  10


typedef struct {
   unsigned int magic ;   //0x55555555 flag  CALIBRATION data valid
   int x[5], xfb[5];
   int y[5], yfb[5];
   int a[7];
} TS_CALIBRATION;


typedef struct ts_sample {
   int       x;
   int       y;
   unsigned int timetick;
} TS_SAMPLE;

TS_CALIBRATION tsCalibration = {.magic = 0xaaaaaaaa};


void TouchCalibrate(void);
void loadCalibration(){
   if (tsCalibration.magic!=0x55555555)
      TouchCalibrate();      
}

static TS_SAMPLE tsSampleRaw;
static volatile unsigned char touched;
static volatile unsigned int tsenable = 1;

static void StepEnable(void);

void ts_linear(TS_SAMPLE *samp);

void  isr_tsc(unsigned int intnum) {
   unsigned int wordsLeftX, wordsLeftY, error = 0,samplefinish = 0;
   volatile int dump;
   unsigned int status;
   int arr_x[SAMPLES];
   int arr_y[SAMPLES];

   status = TSCADCIntStatus(SOC_ADC_TSC_0_REGS);
   if (status & TSCADC_FIFO1_THRESHOLD_INT) {
     samplefinish = 1;
   }
   if(1==samplefinish){
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
   }
   TSCADCIntStatusClear(SOC_ADC_TSC_0_REGS, status); 
   if(1==samplefinish){
      StepEnable();
   }

   if ((1 == tsenable) && (0 == error)&&(1==samplefinish)){
      bubbleSortAscend(arr_x, SAMPLES);
      tsSampleRaw.y = sum(arr_x + 1, SAMPLES - 2) / (SAMPLES - 2);
      bubbleSortAscend(arr_y, SAMPLES);
      tsSampleRaw.y = sum(arr_y + 1, SAMPLES - 2) / SAMPLES - 2;
      touched = 1;
      ts_linear(&tsSampleRaw);
      MSG msg;
      msg.message = MSG_TOUCH;
      msg.xpt = tsSampleRaw.x;
      msg.ypt = tsSampleRaw.y;
      SendMessage(&msg);
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

   /* Configure reference volatage and input to charge step*/
   TSCADCChargeStepConfig(SOC_ADC_TSC_0_REGS, TSCADC_NEGATIVE_REF_XNUR,
                          TSCADC_POSITIVE_INP_CHANNEL2, TSCADC_NEGATIVE_INP_CHANNEL2,
                          TSCADC_POSITIVE_REF_XPUL);

   /* Configure the Analog Supply to Touch screen */
   TSCADCChargeStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_ON,
                                      TSCADC_XNPSW_PIN_OFF, TSCADC_XPPSW_PIN_OFF);

   /* Configure the Analong Ground to Touch screen */
   TSCADCChargeStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                      TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_ON,
                                      TSCADC_WPNSW_PIN_OFF);

   TSCADCTSChargeStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, 0x200);
}

static void StepConfigX(unsigned int stepSelc)
{
    /* Configure ADC to Single ended operation mode */
    TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                    TSCADC_SINGLE_ENDED_OPER_MODE, stepSelc);

    /* Configure reference volatage and input to charge step*/ 
    TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSelc,TSCADC_NEGATIVE_REF_VSSA,
                       TSCADC_POSITIVE_INP_CHANNEL3,TSCADC_NEGATIVE_INP_CHANNEL1,
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


static void StepConfigY(unsigned int stepSelc)
{

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

static void StepEnable(void)
{
    for(int i = 1; i < SAMPLES*2+1; i++) 
    {
         TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i, 1);
    }
}


void tsEnalbe(void){
   tsenable = 1;
}

void tsDisable(void){
   tsenable = 0;
}


int perform_calibration(TS_CALIBRATION *cal) {
	int j;
	float n, x, y, x2, y2, xy, z, zx, zy;
	float det, a, b, c, e, f, i;
	float scaling = 65536.0;

// Get sums for matrix
	n = x = y = x2 = y2 = xy = 0;
	for(j=0;j<5;j++) {
		n += 1.0;
		x += (float)cal->x[j];
		y += (float)cal->y[j];
		x2 += (float)(cal->x[j]*cal->x[j]);
		y2 += (float)(cal->y[j]*cal->y[j]);
		xy += (float)(cal->x[j]*cal->y[j]);
	}

// Get determinant of matrix -- check if determinant is too small
	det = n*(x2*y2 - xy*xy) + x*(xy*y - x*y2) + y*(x*xy - y*x2);
	if(det < 0.1 && det > -0.1) {
        cal->magic = 0xaaaaaaaa;
		return 0;
	}

// Get elements of inverse matrix
	a = (x2*y2 - xy*xy)/det;
	b = (xy*y - x*y2)/det;
	c = (x*xy - y*x2)/det;
	e = (n*y2 - y*y)/det;
	f = (x*y - n*xy)/det;
	i = (n*x2 - x*x)/det;

// Get sums for x calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->xfb[j];
		zx += (float)(cal->xfb[j]*cal->x[j]);
		zy += (float)(cal->xfb[j]*cal->y[j]);
	}

// Now multiply out to get the calibration for framebuffer x coord
	cal->a[0] = (int)((a*z + b*zx + c*zy)*(scaling));
	cal->a[1] = (int)((b*z + e*zx + f*zy)*(scaling));
	cal->a[2] = (int)((c*z + f*zx + i*zy)*(scaling));

// Get sums for y calibration
	z = zx = zy = 0;
	for(j=0;j<5;j++) {
		z += (float)cal->yfb[j];
		zx += (float)(cal->yfb[j]*cal->x[j]);
		zy += (float)(cal->yfb[j]*cal->y[j]);
	}

// Now multiply out to get the calibration for framebuffer y coord
	cal->a[3] = (int)((a*z + b*zx + c*zy)*(scaling));
	cal->a[4] = (int)((b*z + e*zx + f*zy)*(scaling));
	cal->a[5] = (int)((c*z + f*zx + i*zy)*(scaling));

// If we got here, we're OK, so assign scaling to a[6] and return
	cal->a[6] = (int)scaling;
    cal->magic = 0x55555555;
	return 1;
}



void ts_linear(TS_SAMPLE *samp) {
   unsigned int xtemp = samp->x; 
   unsigned int ytemp = samp->y;
   samp->x =   (tsCalibration.a[2] +
                tsCalibration.a[0] * xtemp +
                tsCalibration.a[1] * ytemp) / tsCalibration.a[6];
   samp->y =   (tsCalibration.a[5] +
                tsCalibration.a[3] * xtemp +
                tsCalibration.a[4] * ytemp) / tsCalibration.a[6];
}


void TouchCalibrate(void){
   static const unsigned char calIcon[] = {0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
         0x01,0x80,0x01,0x80,0x01,0x80,0xFF,0xFF,
         0xFF,0xFF,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
      0x80,0x01,0x80,0x01,0x80,0x01,0x80};

   const tLCD_PANEL  *panel = LCDTftInfoGet();
   Dis_DrawText("calibrate touch pad",100,400,C_White,C_TRANSPARENT);
   
   tsCalibration.xfb[0] = 0+CALIBRATION_POINT_OFFSET-8;
   tsCalibration.xfb[1] = panel->width-CALIBRATION_POINT_OFFSET-8;
   tsCalibration.xfb[2] = panel->width-CALIBRATION_POINT_OFFSET-8;
   tsCalibration.xfb[3] = 0+CALIBRATION_POINT_OFFSET-8;
   tsCalibration.xfb[4] = panel->width/2-8;
   
   tsCalibration.yfb[0] = panel->height-CALIBRATION_POINT_OFFSET-8; 
   tsCalibration.yfb[1] = panel->height-CALIBRATION_POINT_OFFSET-8;
   tsCalibration.yfb[2] = 0+CALIBRATION_POINT_OFFSET-8;
   tsCalibration.yfb[3] = 0+CALIBRATION_POINT_OFFSET-8;
   tsCalibration.yfb[4] = panel->height/2-8;
 
   Dis_RectFill(0,0,panel->width,panel->height,C_Black);
   for (int i=0;i<5;i++) {
      Dis_DrawMask(calIcon, tsCalibration.xfb[i],tsCalibration.yfb[i],16,16,C_White,C_TRANSPARENT);    
      while (!touched);
      touched = 0;
      tsCalibration.x[i] = tsSampleRaw.x ;
      tsCalibration.y[i] = tsSampleRaw.y ;
      Dis_RectFill(tsCalibration.xfb[i],tsCalibration.yfb[i],16,16,C_Black);
   }
   perform_calibration(&tsCalibration);
}


void TouchScreenInit(void){
   TSCADCConfigureAFEClock(SOC_ADC_TSC_0_REGS, 24000000, 3000000);
   /* Enable Transistor bias */
   TSCADCTSTransistorConfig(SOC_ADC_TSC_0_REGS, TSCADC_TRANSISTOR_ENABLE);
   /* Map hardware event to Pen Touch IRQ */
   TSCADCHWEventMapSet(SOC_ADC_TSC_0_REGS, TSCADC_PEN_TOUCH);
   /* Set 4 Wire touch screen  mode */
   TSCADCTSModeConfig(SOC_ADC_TSC_0_REGS, TSCADC_FOUR_WIRE_MODE);
   TSCADCStepIDTagConfig(SOC_ADC_TSC_0_REGS, 1);
   /* Disable Write Protection of Step Configuration regs*/
   TSCADCStepConfigProtectionDisable(SOC_ADC_TSC_0_REGS);
    /* Touch Screen detection Configuration*/
   IdleStepConfig();
    /* Configure the Charge step */
   TSchargeStepConfig();
   for(int i = 0; i < SAMPLES; i++)
    {
         StepConfigX(i);
         TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, i, 0x98);
    }

    for(int i = SAMPLES; i < (2 * SAMPLES); i++)
    {
         StepConfigY(i);
         TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, i, 0x98);
    }

   TSCADCFIFOIRQThresholdLevelConfig(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1, SAMPLES-1);
   TSCADCEventInterruptEnable(SOC_ADC_TSC_0_REGS,  TSCADC_FIFO1_THRESHOLD_INT);//TSCADC_SYNC_PEN_EVENT_INT |
   TSCADCModuleStateSet(SOC_ADC_TSC_0_REGS, TSCADC_MODULE_ENABLE);
   StepEnable(); 
}


//! @}





/************************************/




