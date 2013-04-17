/***********************************
*	pl_tsc.c
*
* for mind335x touch screen 
*
*
/************************************/


#include "soc_AM335x.h"
#include "hw_types.h"
#include "platform.h"
#include "hw_cm_per.h"
#include "hw_cm_wkup.h"
#include "hw_cm_dpll.h"
#include "hw_control_AM335x.h"
#include "interrupt.h"
#include "type.h"

#include "tsc_adc.h"
#include "pf_lcd.h"

#define SAMPLES       5
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


void loadCalibration(){
   if (tsCalibration.magic!=0x55555555) {
      while(!perform_calibration(&tsCalibration));
   }
}







/************************************/

volatile unsigned int x_data[2];
volatile unsigned int y_data[2];
volatile unsigned int error;
volatile unsigned int IsTSPress = 0;
volatile unsigned int penUp = 1;
volatile unsigned int numOfInt = 0;
unsigned int touchRelease = 0;
unsigned int dbidx = 0;

/************************************/


static TS_SAMPLE tsSampleRaw;
static volatile unsigned int tsenable = 0;

static void isr_TouchScreen()
{
    unsigned int wordsLeft = 0;
    unsigned int status;
    unsigned int arr_x[SAMPLES] = {0,0,0,0,0};
    unsigned int arr_y[SAMPLES] = {0,0,0,0,0};
    unsigned int x_data = 0;
    unsigned int y_data = 0;
    unsigned int i = 0;
    unsigned int sum = 0;

    status = TSCADCIntStatus(SOC_ADC_TSC_0_REGS);

    if(status & TSCADC_FIFO1_THRESHOLD_INT)
    {
         wordsLeft = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);

         while(wordsLeft)
         {
              x_data = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);

              arr_x[i++] = x_data;

              wordsLeft = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
         }
         bubbleSortAscend(arr_x,SAMPLES);
         x_data = sum(arr_x+1,SAMPLES-2)/SAMPLES-2;

         wordsLeft = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
         i = 0;
         while(wordsLeft)
         {
              y_data = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);

              arr_y[i++] = y_data;

              wordsLeft = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
         }
         bubbleSortAscend(arr_y,SAMPLES);
         y_data = sum(arr_y+1,SAMPLES-2)/SAMPLES-2;
    }
    tsSampleRaw.x = x_data;
    tsSampleRaw.y = y_data;
    StepEnable();
}



void TSCADCModuleClkConfig(void)
{
    /* Writing to MODULEMODE field of CM_WKUP_TSC_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) |=
          CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
           CM_WKUP_ADC_TSC_CLKCTRL_MODULEMODE));

    /*
    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_IDLEST));

   while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_ADC_FCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_ADC_TSC_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_ADC_TSC_CLKCTRL) &
           CM_WKUP_ADC_TSC_CLKCTRL_IDLEST));
}

static void IdleStepConfig(void){
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


static void TSchargeStepConfig(void)
{
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

    /* select fifo 0 */
    TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_FIFO_1);

    /* Configure in One short hardware sync mode */
    TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_ONE_SHOT_HARDWARE_SYNC);

    TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_SIXTEEN_SAMPLES_AVG); 
}

static void StepEnable(void)
{
    unsigned int i = 0;
    if (0==tsenable) {
       return
    }
    for(i = 0; i < SAMPLES*2; i++) // 原来11次，现在SAMPLES*2
    {
         TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i, 1);
    }
}


void tsEnalbe(){
   tsenable = 1;
}

void tsDisable(){
   tsenable = 0;
}






static void tssample ( calibration *cal,
			int index, int x, int y)
{
	static int last_x = -1, last_y;

	if (last_x != -1) {
#define NR_STEPS 10
		int dx = ((x - last_x) << 16) / NR_STEPS;
		int dy = ((y - last_y) << 16) / NR_STEPS;
		int i;
		last_x <<= 16;
		last_y <<= 16;
		for (i = 0; i < NR_STEPS; i++) {
			put_cross (last_x >> 16, last_y >> 16, 2 | XORMODE);
			usleep (1000);
			put_cross (last_x >> 16, last_y >> 16, 2 | XORMODE);
			last_x += dx;
			last_y += dy;
		}
	}

	put_cross(x, y, 2 | XORMODE);
	getxy (ts, &cal->x [index], &cal->y [index]);
	put_cross(x, y, 2 | XORMODE);

	last_x = cal->xfb [index] = x;
	last_y = cal->yfb [index] = y;
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






void ts_linear(struct ts_sample *samp) {
   xtemp = samp->x; ytemp = samp->y;
   samp->x =   (lin->a[2] +
                lin->a[0] * xtemp +
                lin->a[1] * ytemp) / lin->a[6];
   samp->y =   (lin->a[5] +
                lin->a[3] * xtemp +
                lin->a[4] * ytemp) / lin->a[6];
}



void TouchCalibrate(void){
   static const unsigned char calIcon[] = {0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
         0x01,0x80,0x01,0x80,0x01,0x80,0xFF,0xFF,
         0xFF,0xFF,0x01,0x80,0x01,0x80,0x01,0x80,0x01,
      0x80,0x01,0x80,0x01,0x80,0x01,0x80};

   const tLCD_PANEL  *panel = LCDTftInfoGet();
   tsCalibration.xfb = {0+CALIBRATION_POINT_OFFSET-8,panel->width-CALIBRATION_POINT_OFFSET-8,
      panel->width-CALIBRATION_POINT_OFFSET-8,0+CALIBRATION_POINT_OFFSET-8,
      panel->width/2-8}
   tsCalibration.yfb = {panel->height-CALIBRATION_POINT_OFFSET-8,panel->height-CALIBRATION_POINT_OFFSET-8,
   0+CALIBRATION_POINT_OFFSET-8,0+CALIBRATION_POINT_OFFSET-8,panel->height/2-8}
   }
   POINT cantouchpoint[4];
   const tLCD_PANEL *panel = LCDTftInfoGet();
   Dis_RectFill(0,0,panel->width,panel->height,C_Black);
   for (int i=0;i<4,i++) {
      Dis_DrawMask(calIcon,caldispoint[i].x,caldispoint[i].y,16,16,C_White,C_TRANSPARENT);
      Dis_RectFill(calIcon,caldispoint[i].x,caldispoint[i].y,16,16,C_Black);
      while (istouch);
      tsCalibration[i].x = 
      

       
   }


   
   

}


void TouchScreenInit(void){
   TSCADCConfigureAFEClock(SOC_ADC_TSC_0_REGS, 24000000, 3000000);
   /* Enable Transistor bias */
   TSCADCTSTransistorConfig(SOC_ADC_TSC_0_REGS, TSCADC_TRANSISTOR_ENABLE);

   /* Map hardware event to Pen Touch IRQ */
   TSCADCHWEventMapSet(SOC_ADC_TSC_0_REGS, TSCADC_PEN_TOUCH);

   /* Set 4 Wire or 5 wire touch screen  mode */
   TSCADCTSModeConfig(SOC_ADC_TSC_0_REGS, TSCADC_FOUR_WIRE_MODE);

   TSCADCStepIDTagConfig(SOC_ADC_TSC_0_REGS, 1);
   /* Disable Write Protection of Step Configuration regs*/
   TSCADCStepConfigProtectionDisable(SOC_ADC_TSC_0_REGS);

    /* Touch Screen detection Configuration*/
   IdleStepConfig();
    /* Configure the Charge step */
   TSchargeStepConfig();
   for(i = 0; i < SAMPLES; i++)
    {
         StepConfigX(i);

         TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, i, 0x98);
    }

    for(i = SAMPLES; i < (2 * SAMPLES); i++)
    {
         StepConfigY(i);

         TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, i, 0x98);
   }

   TSCADCFIFOIRQThresholdLevelConfig(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1, SAMPLES);
   TSCADCEventInterruptEnable(SOC_ADC_TSC_0_REGS, TSCADC_FIFO1_THRESHOLD_INT);
   TSCADCModuleStateSet(SOC_ADC_TSC_0_REGS, TSCADC_MODULE_ENABLE);
   StepEnable();   //注意 11次
}








/************************************/




