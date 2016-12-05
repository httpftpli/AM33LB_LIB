#include "pf_tsc2007.h"
#include "pf_hsi2c.h"
#include "pf_gpio.h"
#include "module.h"
#include "pf_hsi2c.h"
#include "hsi2c.h"
#include "pf_timertick.h"
#include "pf_key_touchpad.h"
#include "debug.h"




#define TSC2007_SLAVE_ADDR   0x48


#define TSC2007_CMD_SAMPLE_X   0xc8
#define TSC2007_CMD_SAMPLE_Y   0xd8
#define TSC2007_CMD_SAMPLE_Z1   0xe8
#define TSC2007_CMD_SAMPLE_Z2   0xf8
#define TSC2007_PENIRQ_GPIO    MODULE_ID_GPIO3
#define TSC2007_PENIRQ_GPIO_PIN 10



static unsigned int tsc2007I2cModule;

static unsigned char tsc2007SlaveAddr;
static unsigned int tsc2007Readfinish = 0;

void tsc2007i2cHandler(unsigned int index, unsigned int flag);

void tsc2007TouchProcess(void);

bool tsc2007Init(unsigned int i2cModule, unsigned char addrA0A1) {
    unsigned int addr = modulelist[i2cModule].baseAddr;
    tsc2007SlaveAddr = TSC2007_SLAVE_ADDR + addrA0A1;
    unsigned char data;
    if (I2CMasterRead(addr, tsc2007SlaveAddr, &data, 1)) {
        data = 0xb0;
        tsc2007I2cModule = i2cModule;
        GPIOInit(TSC2007_PENIRQ_GPIO, 0, 0);
        I2CRegistHandler(i2cModule, tsc2007i2cHandler);
        touchRegistProcess(tsc2007TouchProcess);
        moduleIntConfigure(i2cModule);
        return true;
    }
    tsc2007SlaveAddr = 0;
    return false;
}


static unsigned int tsc2007samplefinish = 0;
static bool tsc2007Sample(unsigned char xyz1z2) {
    tsc2007samplefinish = 0;
    unsigned char data = xyz1z2;
    return I2CMasterWriteNoBlock(tsc2007I2cModule, tsc2007SlaveAddr, &data, 1);
}



static unsigned short *sampleData = NULL;

static bool tsc2007Read(unsigned short *val) {
    tsc2007Readfinish = 0;
    sampleData = val;
    return I2CMasterReadNoBlock(tsc2007I2cModule, tsc2007SlaveAddr, 2);
}


static bool isTsc2007Pushed(void) {
    return GPIOPinRd(TSC2007_PENIRQ_GPIO, TSC2007_PENIRQ_GPIO_PIN) == 0;
}


static void tsc2007i2cHandler(unsigned int index, unsigned int flag) {
    unsigned int addr = modulelist[tsc2007I2cModule].baseAddr;
    if (flag & I2C_IRQ_TRIG_STOP) {
        if (I2CMasterIntRawStatusEx(addr, I2C_INT_RECV_READY)) {
            unsigned char data0 = I2CMasterDataGet(addr);
            unsigned char data1 = I2CMasterDataGet(addr);
            if (sampleData) {
                *sampleData = (unsigned short)data0 << 4 | data1 >> 4;
                tsc2007Readfinish = 1;
            }
        }

    }
    if (I2CMasterIntRawStatusEx(addr, I2C_INT_TRANSMIT_READY)) {
        tsc2007samplefinish = 1;
    }
}



#define NO_TOUCH             0
#define SAMPLE_X             1
#define SAMPLE_Y             2
#define SAMPLE_Z1            3
#define SAMPLE_Z2            4
#define READ_X               5
#define READ_Y               6
#define READ_Z1              7
#define READ_Z2              8
#define READ_X_FINISH        9
#define READ_Y_FINISH        10
#define READ_Z1_FINISH       11
#define READ_Z2_FINISH       12
#define TOUCH_DEBOUNCE       13
#define COMM_ERR             14


static unsigned short x,y,z1,z2;

void tsc2007TouchProcess(void) {
    if (tsc2007SlaveAddr == 0) {
        return;
    }
    static unsigned long long timemark;
    static int stat = NO_TOUCH;
    NOT_IN_IRQ();
    everytimedo(2){
    switch (stat) {
    case NO_TOUCH:
        if (isTsc2007Pushed()) {
            stat = TOUCH_DEBOUNCE;
            timemark = TimerTickGet64();
        }
        break;
    case TOUCH_DEBOUNCE:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (TimerTickGet64() - timemark > 10) {
                timemark = TimerTickGet64();
                stat = SAMPLE_X;
            }
        }
        break;
    case SAMPLE_Z1:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (!tsc2007Sample(TSC2007_CMD_SAMPLE_Z1)) {
                stat = COMM_ERR;
            }else{
                stat = READ_Z1;
            }
        }
        break;
    case READ_Z1:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (tsc2007samplefinish == 1) {
                tsc2007Read(&z1);
                stat = READ_Z1_FINISH;
            }
        }
        break;
    case READ_Z1_FINISH:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (tsc2007Readfinish == 1) {
                stat = SAMPLE_Z2;
            }
        }
        break;
    case SAMPLE_Z2:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (!tsc2007Sample(TSC2007_CMD_SAMPLE_Z2)) {
               stat = COMM_ERR;
            }else{
               stat = READ_Z2;
            }
        }
        break;
    case READ_Z2:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (tsc2007samplefinish == 1) {
                tsc2007Read(&z2);
                stat = READ_Z2_FINISH;
            }
        }
        break;
    case READ_Z2_FINISH:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (tsc2007Readfinish == 1) {
                stat = SAMPLE_X;
            }
        }
        break;
    case SAMPLE_X:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (!tsc2007Sample(TSC2007_CMD_SAMPLE_X)) {
                stat = COMM_ERR;
            }else{
                stat = READ_X;
            }
        }
        break;
    case READ_X:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (tsc2007samplefinish == 1) {
                tsc2007Read(&x);
                stat = READ_X_FINISH;
            }
        }
        break;
    case READ_X_FINISH:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (tsc2007Readfinish == 1) {
                stat = SAMPLE_Y;
            }
        }
        break;
    case SAMPLE_Y:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (!tsc2007Sample(TSC2007_CMD_SAMPLE_Y)) {
                stat = COMM_ERR;
            }else{
                stat = READ_Y;
            }
        }
        break;
    case READ_Y:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (tsc2007samplefinish == 1) {
                tsc2007Read(&y);
                stat = READ_Y_FINISH;
            }
        }
        break;
    case READ_Y_FINISH:
        if (!isTsc2007Pushed()) {
            stat = NO_TOUCH;
        } else {
            if (tsc2007Readfinish == 1) {
                g_ts.x = g_tsRaw.x = x;
                g_ts.y = g_tsRaw.y = y;
                ts_linear(&tsCalibration, (int *)&(g_ts.x), (int *)&(g_ts.y));
                *(unsigned int *)&g_touched = 1;
                stat = SAMPLE_X;
            }
        }
        break;
    case COMM_ERR:
        //return i2cModule
        I2CInit(tsc2007I2cModule, 100000, NULL, 0); 
        stat = NO_TOUCH;
    default:
        break;
    }
    }
}











