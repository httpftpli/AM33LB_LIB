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
#include "type.h"

#include "tsc_adc.h"
#include "pf_lcd.h"
#include "algorithm.h"
#include "hw_types.h"
#include "hw_cm_wkup.h"
#include "module.h"
#include "mmath.h"
#include "atomic.h"
#include "lib_gui.h"
#include "pf_tsc.h"
#include "mmcsd_proto.h"
#include "type.h"
#include "string.h"
#include "pf_key_touchpad.h"


#define SAMPLES       8

extern mmcsdCtrlInfo mmcsdctr[2];


BOOL TouchCalibrate(BOOL force);



extern volatile TS_SAMPLE g_tsRaw;
volatile static unsigned int tsenable = 1;

static void StepEnable(void);

//static void ts_linear_scale(int *x, int *y, int swap_xy);


static void tscadcdataclear(void) {
    unsigned int wordsLeftX = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
    unsigned int wordsLeftY = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
    for (int i = 0; i < wordsLeftX; i++) {
         TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
    }
    for (int i = 0; i < wordsLeftY; i++) {
         TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
    }
}

void isr_tsc(unsigned int intnum) {
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
        tscadcdataclear();
        //UARTprintf("pen up \n\r");
    }
    if (status & TSCADC_FIFO1_THRESHOLD_INT) {
        samplefinish = 1;
    }
    if (status & TSCADC_SYNC_PEN_EVENT_INT) {
        //preTsSampleRaw.x = 0;
        //UARTprintf("pen down \n\r");
    }
    if (status & TSCADC_FIFO1_OVER_RUN_INT) {
    }
    if (status & TSCADC_END_OF_SEQUENCE_INT) {
        samplefinish = 1;
    }
    if (1 == samplefinish) {
        wordsLeftX = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
        wordsLeftY = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
        if ((wordsLeftX != wordsLeftY) || (wordsLeftY != SAMPLES)) {
            error = 1;
            tscadcdataclear();
        } else {
            for (int i = 0; i < wordsLeftX; i++) {
                arr_x[i] = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
            }
            for (int i = 0; i < wordsLeftY; i++) {
                arr_y[i] = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_1);
            }
        }
    }
    if (1 == samplefinish){
        //StepEnable();
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
#ifdef TS_SWAP_XY
            g_ts.y = g_tsRaw.y = preTsSampleRaw.x;
            g_ts.x = g_tsRaw.x = preTsSampleRaw.y;
#else
            g_ts.x = g_tsRaw.x = preTsSampleRaw.x;
            g_ts.y = g_tsRaw.y = preTsSampleRaw.y;
#endif
            preTsSampleRaw.x = sum(arr_x + 2, SAMPLES - 4) / (SAMPLES - 4);
            preTsSampleRaw.y = sum(arr_y + 2, SAMPLES - 4) / (SAMPLES - 4);
            ts_linear(&tsCalibration, (int *)&(g_ts.x), (int *)&(g_ts.y));
            atomicSet(&g_touched);
            //UARTprintf("x: %d  ; y: %d  ;", tsSampleRaw.x, tsSampleRaw.y);
            //UARTprintf("fbx: %d  ; fby: %d \n\r", msg.xpt, msg.ypt);
        }
    }
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
    TSCADCTSChargeStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, 0x400);

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
    TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_CONTINIOUS_HARDWARE_SYNC);

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
    TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_CONTINIOUS_HARDWARE_SYNC);

    TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_SIXTEEN_SAMPLES_AVG);
}

static void StepEnable(void) {
    for (int i = 0; i < (SAMPLES * 2 + 1); i++) {
        TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i, 1);
    }
}


void tsEnalbe(void) {
    atomicClear(&g_touched);
    tsenable = 1;
}

void tsDisable(void) {
    tsenable = 0;
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
        TSCADCTSStepSampleDelayConfig(baseaddr, i, 10);  
    }

    for (int i = SAMPLES; i < (2 * SAMPLES); i++) {
        StepConfigY(i);
        TSCADCTSStepOpenDelayConfig(baseaddr, i, 0x90);
        TSCADCTSStepSampleDelayConfig(baseaddr, i, 10);
    }

    //TSCADCFIFOIRQThresholdLevelConfig(baseaddr, TSCADC_FIFO_1, SAMPLES);
    //TSCADCFIFOIRQThresholdLevelConfig(baseaddr, TSCADC_FIFO_0, SAMPLES);
    TSCADCModuleStateSet(baseaddr, TSCADC_MODULE_ENABLE);
    StepEnable();
    TSCADCEventInterruptEnable(baseaddr,  TSCADC_END_OF_SEQUENCE_INT | TSCADC_PEN_UP_EVENT_INT | TSCADC_SYNC_PEN_EVENT_INT); // TSCADC_FIFO1_OVER_RUN_INT  | TSCADC_FIFO1_THRESHOLD_INT
    moduleIntConfigure(MODULE_ID_ADCTSC);
}


void  TouchScreenDeInit(){
    moduleDisable(MODULE_ID_ADCTSC);
}



static void touchDetectStepConfigX(unsigned int stepSelc,unsigned char volt){
    /* Configure ADC to Single ended operation mode */
    TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                     TSCADC_SINGLE_ENDED_OPER_MODE, stepSelc);

    /* Configure reference volatage and input to charge step*/
    TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_NEGATIVE_REF_VSSA,
                       TSCADC_POSITIVE_INP_CHANNEL1, TSCADC_NEGATIVE_INP_ADCREFM,
                       TSCADC_POSITIVE_REF_VDDA);

    if(volt){
        /* Configure the Analog Supply to Touch screen */
        TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                   TSCADC_XNPSW_PIN_ON, TSCADC_YPPSW_PIN_OFF,
                                   stepSelc);

        /* Configure the Analong Ground to Touch screen */
        TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                   TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_OFF,
                                   TSCADC_WPNSW_PIN_OFF, stepSelc);
    }else{
        /* Configure the Analog Supply to Touch screen */
        TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                   TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF,
                                   stepSelc);

        /* Configure the Analong Ground to Touch screen */
        TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_ON,
                                   TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_OFF,
                                   TSCADC_WPNSW_PIN_OFF, stepSelc);
    }

    /* select fifo 0 */
    TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_FIFO_0);

    /* Configure in One short hardware sync mode */
    TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_ONE_SHOT_SOFTWARE_ENABLED);

    TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_SIXTEEN_SAMPLES_AVG);
    TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, stepSelc, 0x90);
    TSCADCTSStepSampleDelayConfig(SOC_ADC_TSC_0_REGS, stepSelc, 0);
}


static void touchDetectStepConfigY(unsigned int stepSelc,unsigned char volt){
    /* Configure ADC to Single ended operation mode */
    TSCADCTSStepOperationModeControl(SOC_ADC_TSC_0_REGS,
                                     TSCADC_SINGLE_ENDED_OPER_MODE, stepSelc);

    /* Configure reference volatage and input to charge step*/
    TSCADCTSStepConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_NEGATIVE_REF_VSSA,
                       TSCADC_POSITIVE_INP_CHANNEL4, TSCADC_NEGATIVE_INP_ADCREFM,
                       TSCADC_POSITIVE_REF_VDDA);

    if(volt){
        /* Configure the Analog Supply to Touch screen */
        TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                   TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_ON,
                                   stepSelc);

        /* Configure the Analong Ground to Touch screen */
        TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                   TSCADC_YPNSW_PIN_OFF, TSCADC_YNNSW_PIN_OFF,
                                   TSCADC_WPNSW_PIN_OFF, stepSelc);
    }else{
        /* Configure the Analog Supply to Touch screen */
        TSCADCTSStepAnalogSupplyConfig(SOC_ADC_TSC_0_REGS, TSCADC_XPPSW_PIN_OFF,
                                   TSCADC_XNPSW_PIN_OFF, TSCADC_YPPSW_PIN_OFF,
                                   stepSelc);

        /* Configure the Analong Ground to Touch screen */
        TSCADCTSStepAnalogGroundConfig(SOC_ADC_TSC_0_REGS, TSCADC_XNNSW_PIN_OFF,
                                   TSCADC_YPNSW_PIN_ON, TSCADC_YNNSW_PIN_OFF,
                                   TSCADC_WPNSW_PIN_OFF, stepSelc);
    }

    /* select fifo 0 */
    TSCADCTSStepFIFOSelConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_FIFO_0);

    /* Configure in One short hardware sync mode */
    TSCADCTSStepModeConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_ONE_SHOT_SOFTWARE_ENABLED);

    TSCADCTSStepAverageConfig(SOC_ADC_TSC_0_REGS, stepSelc, TSCADC_SIXTEEN_SAMPLES_AVG);
    TSCADCTSStepOpenDelayConfig(SOC_ADC_TSC_0_REGS, stepSelc, 0x90);
    TSCADCTSStepSampleDelayConfig(SOC_ADC_TSC_0_REGS, stepSelc, 0);
}



bool TouchScreenTsPadDetect(){
    MODULE *module = modulelist + MODULE_ID_ADCTSC;
    unsigned int baseaddr = module->baseAddr;
    moduleEnable(MODULE_ID_ADCTSC);
    TSCADCModuleStateSet(baseaddr, TSCADC_MODULE_DISABLE);
    TSCADCConfigureAFEClock(baseaddr, module->moduleClk->fClk[0]->clockSpeedHz, 600000);
    /* Enable Transistor bias */
    TSCADCTSTransistorConfig(baseaddr, TSCADC_TRANSISTOR_ENABLE);
    /* Set 4 Wire touch screen  mode */
    TSCADCTSModeConfig(baseaddr, TSCADC_GENERAL_PURPOSE_MODE);
    //TSCADCStepIDTagConfig(baseaddr, 1);
    /* Disable Write Protection of Step Configuration regs*/
    TSCADCStepConfigProtectionDisable(baseaddr);
    /* Touch Screen detection Configuration*/
    //IdleStepConfig();
    /* Configure the Charge step */
    //TSchargeStepConfig();
    //clear fifo;
    unsigned short wordsLeftX = TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
    for(int i=0;i<wordsLeftX;i++){
        TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
    }
    //config step
    touchDetectStepConfigX(0,1);
    touchDetectStepConfigX(1,0);
    touchDetectStepConfigY(2,1);
    touchDetectStepConfigY(3,0);
    touchDetectStepConfigX(4,1);
    touchDetectStepConfigX(5,0);
    touchDetectStepConfigY(6,1);
    touchDetectStepConfigY(7,0);
    touchDetectStepConfigY(8,0);
    for(int i=0;i<9;i++){
        TSCADCConfigureStepEnable(SOC_ADC_TSC_0_REGS, i, 1);
    }
    TSCADCModuleStateSet(baseaddr, TSCADC_MODULE_ENABLE);
    //waite for adc convert finish;
    while(TSCADCFIFOWordCountRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0) < 8);
    unsigned short tsval[8];
    for(int i=0;i<8;i++){
       tsval[i] = TSCADCFIFOADCDataRead(SOC_ADC_TSC_0_REGS, TSCADC_FIFO_0);
    }
    if(tsval[0]>3800 && tsval[2]>3800 && tsval[1]<200 && tsval[3]<200 &&
       tsval[4]>3800 && tsval[6]>3800 && tsval[5]<200 && tsval[7]<200){
       return true;
    }
    return false;
}


//! @}





/************************************/




