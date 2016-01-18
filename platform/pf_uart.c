/**
 *  \file   pf_uart.c
 *
 *  \brief
 *  \# include "pf_uart.h" \n \# include "uart_irda_cir.h"
 *  \author  lfl
 *  \addtogroup UART
 *  @{

 */




#include "hw_control_AM335x.h"
#include "soc_AM335x.h"
#include "hw_cm_wkup.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "module.h"
#include "uart_irda_cir.h"
#include "pf_uart.h"
#include "mmath.h"
#include "pf_key_touchpad.h"
#include "pf_timertick.h"
#include <math.h>
#include "pf_edma.h"
#include "debug.h"
#include <string.h>
#include "cache.h"
#include "debug.h"



/*
** This function enables the system L3 and system L4_WKUP clocks.
** This also enables the clocks for UART0 instance.
*/

void UART0ModuleClkConfig(void) {
    /* Configuring L3 Interface Clocks. */

    /* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
    CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while (CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
            CM_PER_L3_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
    CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while (CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
            CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
    CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while (CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
    CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while (CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
    CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while (CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));

    /* Checking fields for necessary values.  */

    /* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
    while ((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
            CM_PER_L3_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
    ** desired value.
    */
    while ((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
            CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
            CM_PER_L3_INSTR_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
    ** attain the desired value.
    */
    while (CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
    ** register to attain the desired value.
    */
    while (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
    ** to attain the desired value.
    */
    while (CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));


    /* Configuring registers related to Wake-Up region. */

    /* Writing to MODULEMODE field of CM_WKUP_CONTROL_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) |=
    CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while (CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
            CM_WKUP_CONTROL_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) |=
    CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while (CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
            CM_WKUP_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_L3_AON_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) |=
    CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while (CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
            CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL));

    /* Writing to MODULEMODE field of CM_WKUP_UART0_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) |=
    CM_WKUP_UART0_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while (CM_WKUP_UART0_CLKCTRL_MODULEMODE_ENABLE !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) &
            CM_WKUP_UART0_CLKCTRL_MODULEMODE));

    /* Verifying if the other bits are set to required settings. */

    /*
    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    ** desired value.
    */
    while ((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
            CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
            CM_WKUP_CONTROL_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_AON_GCLK field in CM_L3_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while (CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
            CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_L4WKUP_CLKCTRL register to attain
    ** desired value.
    */
    while ((CM_WKUP_L4WKUP_CLKCTRL_IDLEST_FUNC <<
            CM_WKUP_L4WKUP_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
            CM_WKUP_L4WKUP_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_GCLK field in CM_WKUP_CLKSTCTRL register
    ** to attain desired value.
    */
    while (CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
            CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_AON_GCLK field in CM_L4_WKUP_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while (CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) &
            CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK));

    /*
    ** Waiting for CLKACTIVITY_UART0_GFCLK field in CM_WKUP_CLKSTCTRL
    ** register to attain desired value.
    */
    while (CM_WKUP_CLKSTCTRL_CLKACTIVITY_UART0_GFCLK !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
            CM_WKUP_CLKSTCTRL_CLKACTIVITY_UART0_GFCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_UART0_CLKCTRL register to attain
    ** desired value.
    */
    while ((CM_WKUP_UART0_CLKCTRL_IDLEST_FUNC <<
            CM_WKUP_UART0_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_UART0_CLKCTRL) &
            CM_WKUP_UART0_CLKCTRL_IDLEST));
}



static UARTRCVHANDLER rcvhandler = NULL;

void UARTRcvRegistHander(UARTRCVHANDLER handler) {
    rcvhandler = NULL;
}



static void uartclearbuff(unsigned int baseaddr) {
    volatile static char dummy;
    unsigned int val = HWREG(baseaddr + 0x64);
    for (int i = 0; i < val; i++) {
        dummy = HWREGB(baseaddr + UART_RHR);
    }
}


extern void (*keyhandler)(int keycode);
void isr_uart_for_touche_keyboard(unsigned int intNum) {
    unsigned int baseaddr = modulelist[intNum].baseAddr;
    if (UARTIntPendingStatusGet(baseaddr) == UART_N0_INT_PENDING) return;
    unsigned int intval = UARTIntIdentityGet(baseaddr);
    if (intval == UART_INTID_RX_THRES_REACH) {
        for (int i = 0; i < 8; i++) {
            volatile unsigned char tempval = HWREGB(baseaddr + UART_RHR);
            ((unsigned char *)&keyTouchpadMsg)[i] = tempval;
            //UARTPutc(tempval);
        }
        if (isKeyTouchEvent(&keyTouchpadMsg)) {
            /*if(keyTouchpadMsg.type & MSG_TYPE_KEY){*/
            if (keyTouchpadMsg.keycode != 0xff && keyscancode2key != NULL) {
                g_keycode = keyscancode2key(keyTouchpadMsg.keycode);
                atomicSet(&g_keyPushed);
                if (keyhandler != NULL) keyhandler(g_keycode);
            }
            if (keyTouchpadMsg.tscval != 0xffffffff) {
#ifdef TS_SWAP_XY
                g_ts.y = g_tsRaw.y = keyTouchpadMsg.tscval & 0xffff;
                g_ts.x = g_tsRaw.x = keyTouchpadMsg.tscval >> 16;
#else
                g_ts.x = g_tsRaw.x = keyTouchpadMsg.tscval & 0xffff;
                g_ts.y = g_tsRaw.y = keyTouchpadMsg.tscval >> 16;
#endif
                ts_linear(&tsCalibration, (int *)&g_ts.x, (int *)&g_ts.y);
                atomicSet(&g_touched);
            }

            //if (keyTouchpadMsg.type & MSG_TYPE_KEYRESET) {
            //  atomicSet(&g_keyRest);
            //}
        }
    }
    if (intval == UART_INTID_CHAR_TIMEOUT) {
        uartclearbuff(baseaddr);
    }
}



void isr_uart(unsigned int intNum) {
    unsigned int baseaddr = modulelist[intNum].baseAddr;
    if (UARTIntPendingStatusGet(baseaddr) == UART_N0_INT_PENDING) return;
    unsigned int intval = UARTIntIdentityGet(baseaddr);
    if (intval == UART_INTID_RX_THRES_REACH) {
    }
    if (intval == UART_INTID_CHAR_TIMEOUT) {
        unsigned int val = HWREG(baseaddr + 0x64);
        for (int i = 0; i < val; i++) {
            volatile unsigned char tempval1 = HWREGB(baseaddr + UART_RHR);
        }
    }
}


void isr_uart_for_keyboard(unsigned int intNum) {
    unsigned int baseaddr = modulelist[intNum].baseAddr;
    if (UARTIntPendingStatusGet(baseaddr) == UART_N0_INT_PENDING) return;
    unsigned int intval = UARTIntIdentityGet(baseaddr);
    if (intval == UART_INTID_RX_THRES_REACH) {
        for (int i = 0; i < 8; i++) {
            volatile unsigned char tempval = HWREGB(baseaddr + UART_RHR);
            ((unsigned char *)&keyTouchpadMsg)[i] = tempval;
            //UARTPutc(tempval);
        }
        if (isKeyTouchEvent(&keyTouchpadMsg)) {
            /*if(keyTouchpadMsg.type & MSG_TYPE_KEY){*/
            if (keyTouchpadMsg.keycode != 0xff && keyscancode2key != NULL) {
                g_keycode = keyscancode2key(keyTouchpadMsg.keycode);
                atomicSet(&g_keyPushed);
                if (keyhandler != NULL) keyhandler(g_keycode);
            }
        }
    }
    if (intval == UART_INTID_CHAR_TIMEOUT) {
        uartclearbuff(baseaddr);
    }
}



void isr_uart_for_9bit(unsigned int intNum) {

}



bool UARTSendNoBlock(unsigned int moduleId, void *buf, size_t len) {
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    unsigned int lcrRegValue = 0;
    unsigned int retVal = FALSE;
    char *buftemp = (char *)buf;
    ASSERT(len <= 64);

    /* Switching to Register Operational Mode of operation. */
    lcrRegValue = UARTRegConfigModeEnable(baseAdd, UART_REG_OPERATIONAL_MODE);

    /*
    ** Checking if either THR alone or both THR and Transmitter Shift Register
    ** are empty.
    */
    if (HWREG(baseAdd + UART_LSR) & (UART_LSR_TX_SR_E | UART_LSR_TX_FIFO_E)) {
        for (int i = 0; i < len; i++) {
            HWREG(baseAdd + UART_THR) = buftemp[i];
            retVal = TRUE;
        }
    }
    /* Restoring the value of LCR. */
    HWREG(baseAdd + UART_LCR) = lcrRegValue;
    return retVal;
}

void UARTSend(unsigned int moduleId, void *buf, size_t len) {
    unsigned int baseAdd = modulelist[moduleId].baseAddr;
    for (int i = 0; i < len; i++) {
        UARTCharPut(baseAdd, ((unsigned char *)buf)[i]);
    }
}


static unsigned int UARTDivisorValCompute1(unsigned int moduleClk, unsigned int baudRate, unsigned int *mode_nX) {
    int divisor, divisor13, divisor16, erate13m, erate13p, erate16m, erate16p, erate13, erate16;
    divisor13 = (int)(moduleClk) / (13 * baudRate);
    divisor16 = (int)(moduleClk) / (16 * baudRate);
    erate13m = ABS((int)baudRate - (int)(moduleClk / divisor13 / 13));
    erate13p = ABS((int)baudRate - (int)(moduleClk / (divisor13 + 1) / 13));
    erate16m = ABS((int)baudRate - (int)(moduleClk / divisor16 / 16));
    erate16p = ABS((int)baudRate - (int)(moduleClk / (divisor16 + 1) / 16));
    if (erate13m < erate13p) {
        erate13 = erate13m;
    } else {
        erate13 = erate13p;
        divisor13++;
    }

    if (erate16m <= erate16p) {
        erate16 = erate16m;
    } else {
        erate16 = erate16p;
        divisor16++;
    }

    if (erate13 <= erate16) {
        divisor = divisor13;
        *mode_nX = 13;
    } else {
        divisor = divisor16;
        *mode_nX = 16;
    }
    return divisor;
}


/**
 * @brief 初始化UART
 * @param [in] moduleId UART模块ID，\b MODULE_ID_UARTX
 * @param [boudRate] 波特率 
 * @param [in]  charLen 字符长度
 * @param [in]  parityFlag 奇偶校验
 * - UART_PARITY_REPR_1 \n - UART_PARITY_REPR_0
 * - UART_ODD_PARITY \n - UART_EVEN_PARITY
 * - UART_PARITY_NONE
 * @param [in]  stopBit 停止位 
 * - UART_FRAME_NUM_STB_1
 * - UART_FRAME_NUM_STB_1_5_2
 * @param [in] intFlag 中断使能标记
 *  - UART_INT_CTS - to enable Clear-To-Send interrupt,
 *  - UART_INT_RTS - to enable Request-To-Send interrupt,
 *  - UART_INT_XOFF - to enable XOFF interrupt,
 *  - UART_INT_SLEEPMODE - to enable Sleep Mode,
 *  - UART_INT_MODEM_STAT - to enable Modem Status interrupt,
 *  - UART_INT_LINE_STAT - to enable Line Status interrupt,
 *  - UART_INT_THR - to enable Transmitter Holding Register Empty interrupt,
 *  - UART_INT_RHR_CTI - to enable Receiver Data available interrupt and
 *                       Character timeout indication interrupt.  *
 *    @param [in] rxFifoLen
 *                          接受缓冲区中�?��发深�?
 *  - UART_FCR_RX_TRIG_LVL_8
 *  - UART_FCR_RX_TRIG_LVL_16
 *  - UART_FCR_RX_TRIG_LVL_56
 *  - UART_FCR_RX_TRIG_LVL_60
 *  @param [out] TxFifoLen 发送缓冲区�?���?��深度
 *  - UART_FCR_RX_TRIG_LVL_8
 *  - UART_FCR_TX_TRIG_LVL_16
 *  - UART_FCR_TX_TRIG_LVL_32
 *  - UART_FCR_TX_TRIG_LVL_56
 * @return  NONE
 * @date    2013/5/23
 * @note
 */

void uartInit(unsigned int moduleId, unsigned int boudRate,
              unsigned int charLen, unsigned int parityFlag,
              unsigned int stopBit, unsigned int intFlag,
              unsigned int rxFifoLen, unsigned int txFiloLen) {
    MODULE *module = &modulelist[moduleId];
    moduleEnable(moduleId);
    unsigned int baseaddr = module->baseAddr;
    /* Performing a module reset. */
    UARTModuleReset(baseaddr);

    /* Setting the TX and RX FIFO Trigger levels */
    if ((rxFifoLen != 1) || (txFiloLen != 1)) {
        unsigned int fifoConfig = UART_FIFO_CONFIG(UART_TRIG_LVL_GRANULARITY_4,
                                                   UART_TRIG_LVL_GRANULARITY_4,
                                                   txFiloLen,
                                                   rxFifoLen,
                                                   1,
                                                   1,
                                                   UART_DMA_EN_PATH_SCR,
                                                   UART_DMA_MODE_0_ENABLE);

        /* Configuring the FIFO settings. */
        UARTFIFOConfig(baseaddr, fifoConfig);
    }

    /* Performing Baud Rate settings. */
    /* Computing the Divisor Value. */
    unsigned int mode_nX;
    unsigned int divisorValue = UARTDivisorValCompute1(module->moduleClk->fClk[0]->clockSpeedHz,
                                                       boudRate, &mode_nX);

    /* Programming the Divisor Latches. */
    UARTDivisorLatchWrite(baseaddr, divisorValue);

    /* Switching to Configuration Mode B. */
    UARTRegConfigModeEnable(baseaddr, UART_REG_CONFIG_MODE_B);

    /* Programming the Line Characteristics. */
    UARTLineCharacConfig(baseaddr, ((charLen - 5) | stopBit), parityFlag);

    /* Disabling write access to Divisor Latches. */
    UARTDivisorLatchDisable(baseaddr);

    /* Disabling Break Control. */
    UARTBreakCtl(baseaddr, UART_BREAK_COND_DISABLE);
    UARTIntEnable(baseaddr, intFlag);
    //UARTFIFORegisterWrite(baseaddr, UART_FCR_PROGRAM(rxFifoLen,txFiloLen,0,1,1,1));

    /* Switching to operating mode. */
    if (13 == mode_nX) {
        UARTOperatingModeSelect(baseaddr, UART13x_OPER_MODE);
    } else {
        UARTOperatingModeSelect(baseaddr, UART16x_OPER_MODE);
    }
    moduleIntConfigure(moduleId);
}
//! @}
//!



static void (*beforsend)();
static void (*aftersend)();

void uartInitFor9Bit(unsigned int moduleId, unsigned int boudRate,
                     unsigned stopBit, unsigned int intFlag, void (*beforSend)(), void (*afterSend)()) {
    uartInit(moduleId, boudRate, 8, UART_PARITY_REPR_1,
             stopBit, intFlag, 1, 1);
    beforsend = beforSend;
    aftersend = afterSend;
    if (aftersend != NULL) aftersend();
}


void uartSend9Bit(unsigned int moduleId, unsigned short data) {
    unsigned int addr = modulelist[moduleId].baseAddr;
    unsigned int bit9 = data & 1 << 8;
    while ((UART_LSR_TX_SR_E | UART_LSR_TX_FIFO_E) !=
           (HWREG(addr + UART_LSR) & (UART_LSR_TX_SR_E | UART_LSR_TX_FIFO_E)));
    UARTParityModeSet(addr, bit9 ? UART_PARITY_REPR_1 : UART_PARITY_REPR_0);
    if (beforsend != NULL) beforsend();
    UARTCharPutNonBlocking(addr, (unsigned short)data);
    if (aftersend != NULL) aftersend();
}


bool uartRcv9bit(unsigned int moduleId, unsigned short *pdat, unsigned int timeout) {
    unsigned int addr = modulelist[moduleId].baseAddr;
    unsigned int timermark = TimerTickGet();
    unsigned short datatmp;
    while (1) {
        if ((timermark + timeout) <= TimerTickGet()) return false;
        if ((HWREG(addr + UART_LSR) & UART_LSR_RX_FIFO_E) == 1) {
            datatmp = (unsigned char)HWREG(addr + UART_RHR);
            datatmp |= (unsigned char)(!!(HWREG(addr + UART_LCR) & 1 << 4) ^
                                       (UARTRxErrorGet(addr) == UART_PARITY_ERROR)) << 8;
            if (pdat != NULL) {
                *pdat = datatmp;
            }
            return true;
        }
    }
}


#pragma data_alignment = 512
static char uartdmaTxbuf[512];
#pragma data_alignment = 512
static char uartdmaRxbuf[512];

static void UartBaudRateSet(unsigned int baudRate) {
    unsigned int divisorValue = 0;

    /* Computing the Divisor Value. */
    divisorValue = UARTDivisorValCompute(48000000,
                                         baudRate,
                                         UART16x_OPER_MODE,
                                         UART_MIR_OVERSAMPLING_RATE_42);

    /* Programming the Divisor Latches. */
    UARTDivisorLatchWrite(SOC_UART_0_REGS, divisorValue);
}

static void UartFIFOConfigure(void) {
    unsigned int fifoConfig = 0;

    /*
    ** Transmitter Trigger Level Granularity is 1.
    ** Receiver Trigger Level Granularity is 1.
    ** Transmit Trigger Space set using 'txTrigSpace'.
    ** Receive Trigger level set using 'rxTrigLevel'.
    ** Clear the Trasnmit FIFO.
    ** Clear the Receive FIFO.
    ** DMA Mode enabling shall happen through SCR register.
    ** DMA Mode 1 is enabled.
    */
    fifoConfig = UART_FIFO_CONFIG(UART_TRIG_LVL_GRANULARITY_1,
                                  UART_TRIG_LVL_GRANULARITY_1,
                                  32,
                                  32,
                                  1,
                                  1,
                                  UART_DMA_EN_PATH_SCR,
                                  UART_DMA_MODE_1_ENABLE);

    /* Configuring the FIFO settings. */
    UARTFIFOConfig(SOC_UART_0_REGS, fifoConfig);
}


static void uartdmarcfinishhandler(unsigned int tcc, unsigned int stat);
static unsigned int uartmoduletorxdmachannel(unsigned int moduleId);

void uartInitForDMA(unsigned int moduleId, unsigned int boudRate,
                    unsigned int parityFlag, unsigned int intFlag) {

    MODULE *module = &modulelist[moduleId];
    unsigned int baseaddr = module->baseAddr;

    moduleEnable(moduleId);
    /* Performing a module reset. */
    UARTModuleReset(baseaddr);

    /* Setting the TX and RX FIFO Trigger levels */

    unsigned int fifoConfig = UART_FIFO_CONFIG(UART_TRIG_LVL_GRANULARITY_1,
                                               UART_TRIG_LVL_GRANULARITY_1,
                                               63,
                                               2,
                                               1,
                                               1,
                                               UART_DMA_EN_PATH_SCR,
                                               UART_DMA_MODE_1_ENABLE);

    /*Configuring the FIFO settings. */
    UARTFIFOConfig(baseaddr, fifoConfig);

    /* Enabling DMA Mode 1. */
    //UARTDMAEnable(baseaddr, UART_DMA_MODE_1_ENABLE);

    UARTTxDMAThresholdControl(baseaddr, UART_TX_DMA_THRESHOLD_REG);

    /* Configuring the Transmit DMA Threshold value. */
    UARTTxDMAThresholdValConfig(baseaddr, 62);

    //UARTDMAEnable(baseaddr, UART_DMA_MODE_1_ENABLE);


    /* Performing Baud Rate settings. */
    /* Computing the Divisor Value. */
    unsigned int mode_nX;
    unsigned int divisorValue = UARTDivisorValCompute1(module->moduleClk->fClk[0]->clockSpeedHz,
                                                       boudRate, &mode_nX);

    /* Programming the Divisor Latches. */
    UARTDivisorLatchWrite(baseaddr, divisorValue);

    /* Switching to Configuration Mode B. */
    UARTRegConfigModeEnable(baseaddr, UART_REG_CONFIG_MODE_B);

    /* Programming the Line Characteristics. */
    UARTLineCharacConfig(baseaddr, UART_FRAME_WORD_LENGTH_8 | UART_FRAME_NUM_STB_1, parityFlag);

    /* Disabling write access to Divisor Latches. */
    UARTDivisorLatchDisable(baseaddr);

    /* Disabling Break Control. */
    UARTBreakCtl(baseaddr, UART_BREAK_COND_DISABLE);

    UARTIntEnable(baseaddr, UART_INT_RHR_CTI);
    //UARTFIFORegisterWrite(baseaddr, UART_FCR_PROGRAM(rxFifoLen,txFiloLen,0,1,1,1));

    /* Switching to operating mode. */
    if (13 == mode_nX) {
        UARTOperatingModeSelect(baseaddr, UART13x_OPER_MODE);
    } else {
        UARTOperatingModeSelect(baseaddr, UART16x_OPER_MODE);
    }
    moduleIntConfigure(moduleId);

    EDMARegisterHandler(uartmoduletorxdmachannel(moduleId), uartdmarcfinishhandler);
}


static unsigned int uartmoduletotxdmachannel(unsigned int moduleId) {
    unsigned int ch = 0;
    switch (moduleId) {
    case MODULE_ID_UART0:
        ch = EDMA3_CHA_UART0_TX;
        break;
    case MODULE_ID_UART1:
        ch = EDMA3_CHA_UART1_TX;
        break;
    case MODULE_ID_UART2:
        ch = EDMA3_CHA_UART2_TX;
        break;
    case MODULE_ID_UART3:
        ch = EDMA3_CHA_UART3_TX;
        break;
    case MODULE_ID_UART4:
        ch = EDMA3_CHA_UART4_TX;
        break;
    default:
        break;
    }
    return ch;
}

static unsigned int uartmoduletorxdmachannel(unsigned int moduleId) {
    unsigned int ch = 0;
    switch (moduleId) {
    case MODULE_ID_UART0:
        ch = EDMA3_CHA_UART0_RX;
        break;
    case MODULE_ID_UART1:
        ch = EDMA3_CHA_UART1_RX;
        break;
    case MODULE_ID_UART2:
        ch = EDMA3_CHA_UART2_RX;
        break;
    case MODULE_ID_UART3:
        ch = EDMA3_CHA_UART3_RX;
        break;
    case MODULE_ID_UART4:
        ch = EDMA3_CHA_UART4_RX;
        break;
    default:
        break;
    }
    return ch;
}


static void *rcvbuf = 0;
static unsigned int rcvlen;
static bool *uartdmarcvfinish;

void uartSendDmaNoBlock(uint32 moduleId, void *buf, uint32 size, bool *flag) {
    MODULE *module = &modulelist[moduleId];
    unsigned int baseaddr = module->baseAddr;
    unsigned int dmabaseaddr = modulelist[MODULE_ID_EDMA].baseAddr;
    unsigned int ch = uartmoduletotxdmachannel(moduleId);
    size = MIN(size, 512);
    memcpy(uartdmaTxbuf, buf, size);
    CacheDataCleanBuff((unsigned int)uartdmaTxbuf, 512);
    if (size > 1) {
        EDMARequestXferWithBufferEntryEx(EDMA3_TRIG_MODE_EVENT,
                                         ch, baseaddr + UART_THR,
                                         (unsigned int)uartdmaTxbuf + 1,
                                         0, 8, 1, size - 1, ch, 0);
    }
    UARTCharPut(baseaddr, uartdmaTxbuf[0]);
    if (size > 1) {
        EDMA3EnableTransfer(dmabaseaddr, ch, EDMA3_TRIG_MODE_EVENT);
    }
    uartdmarcvfinish = flag;
}



static void uartdmarcfinishhandler(unsigned int tcc, unsigned int stat) {
    if (EDMA3_XFER_COMPLETE == stat) {
        CacheDataInvalidateBuff((unsigned int)uartdmaRxbuf, 512);
        if (rcvbuf && rcvlen) {
            memcpy(rcvbuf, uartdmaRxbuf, rcvlen);
        }
        *uartdmarcvfinish = true;
    }
}


void uartRcvDmaNoBlock(unsigned int moduleId, void *buf, unsigned int size) {
    ASSERT(size % 2 == 0);
    MODULE *module = &modulelist[moduleId];
    unsigned int baseaddr = module->baseAddr;
    unsigned int ch = uartmoduletorxdmachannel(moduleId);
    size = MIN(size, 512);
    rcvbuf = buf;
    rcvlen = size;
    *uartdmarcvfinish = false;
    EDMARequestXferWithBufferEntry(EDMA3_TRIG_MODE_EVENT,
                                   ch, baseaddr + UART_RHR,
                                   (unsigned int)uartdmaRxbuf,
                                   1, 8, 2, size / 2, ch);
}



/****************************** End of file *********************************/
