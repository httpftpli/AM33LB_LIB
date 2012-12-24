#include "dcan.h"
#include "soc_AM335x.h"
#include "hw_types.h"
#include "interrupt.h"
#include "platform.h"
#include "hw_cm_per.h"
#include "hw_control_AM335x.h"

#define DCAN_ERROR_OCCURED       (0x8000u)
/* Total No. of message objects available in the DCAN message RAM */
#define CAN_NUM_OF_MSG_OBJS      (64u)



#define DCAN_NUM_OF_MSG_OBJS    (64u)
#define DCAN_MSGOBF_RX_BEGIN    1
#define DCAN_MSGOBF_RX_NUMBER   DCAN_NUM_OF_MSG_OBJS/2
#define DCAN_MSGOBF_TX_BEGIN    DCAN_MSGOBF_RX_BEGIN+DCAN_MSGOBF_RX_NUMBER
#define DCAN_MSGOBF_TX_NUMBER   DCAN_NUM_OF_MSG_OBJS/2




typedef struct {
   unsigned char RxErrCnt;
   unsigned char TxErrCnt;
   unsigned short LastErrCode;
   unsigned short fgErrWarn:1;
   unsigned short fgErrPassive: 1;
   unsigned short fgBusOff:1;
   unsigned short fgSendFinish:1;
   unsigned short fgDataRcved:1;
}G_CAN;



volatile G_CAN g_can[2];

static CAN_FRAME canFrame;

static void (*dcanRcvedHandler)(unsigned int index,CAN_FRAME *frame)
   = (void (*)(unsigned int ,CAN_FRAME *))0;
        
  

void CANRegistRcvedHandler(void (*handler)(unsigned int index,CAN_FRAME *frame)){
   dcanRcvedHandler = handler;
}


/**
 * \brief   This function should be used to clear the interrupt pending status 
 *          of receive objects after a new message is received. This will clear
 *          the IntPnd status of the message object represented by msgNum.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   msgNum        Message object number.
 * \param   ifReg         Interface register set used.
 *
**/
static void CANClrIntPndStat(unsigned int baseAdd, unsigned int msgNum, 
                      unsigned int ifReg)
{
    /* Clear the IntPnd bit of DCAN_IFMCTL register */
    DCANClrIntPnd(baseAdd, ifReg);

    /* Set the ClrIntPnd bit of DCAN_IFCMD register */
    DCANCommandRegSet(baseAdd, DCAN_CLR_INTPND, msgNum, ifReg);
}


static void CANRxMsgObjectConfig(unsigned int baseAdd)
{
    unsigned val;

    while (DCANIFBusyStatusGet(baseAdd,DCAN_IF_READ));
    /* Set the message valid bit */ 
    val = (0 << DCAN_IF2ARB_DIR_SHIFT)|(1<< DCAN_IF2ARB_MSGVAL_SHIFT);
    HWREG(baseAdd + DCAN_IFARB(DCAN_IF_READ)) = val;
    val = 1<<7 | 1<<10 | 1<<12;
    DCANIFMCtrSet(baseAdd,DCAN_IF_READ,val);
    val = 1<<30;
    HWREG(baseAdd + DCAN_IFMSK(DCAN_IF_READ)) = val;
    for (int i=0;i<DCAN_MSGOBF_RX_NUMBER;i++) {
       DCANCommandRegSet(baseAdd, DCAN_ACCESS_CTL_BITS | DCAN_MSG_WRITE
                         | DCAN_ACCESS_MSK_BITS
                      |(1<<21)|(1<<19),i+DCAN_MSGOBF_RX_BEGIN, DCAN_IF2_REG);
       while (DCANIFBusyStatusGet(baseAdd,DCAN_IF_READ));
    }
}

/**
 * \brief   Read data from a message object.
 *
 * \param   baseAdd       Base Address of the DCAN Module Registers.
 * \param   msgNum        Message object number.
 * \param   data          Pointer to an unsigned integer. Used to fetch data 
 *                        bytes from data registers.
 * \param   ifReg         Interface register set used.
 *
*/
static void CANFrameRead(unsigned int baseAdd, unsigned int msgNum, 
                       CAN_FRAME * frame)
{
    /* Read a message object from CAN message RAM to Interface register */
    DCANCommandRegSet(baseAdd, (DCAN_DAT_A_ACCESS | DCAN_DAT_B_ACCESS | 
                                DCAN_TXRQST_ACCESS | DCAN_CLR_INTPND | 
                                DCAN_ACCESS_CTL_BITS | DCAN_ACCESS_ARB_BITS | 
                                DCAN_ACCESS_MSK_BITS | DCAN_MSG_READ 
                                |DCAN_CLR_INTPND ), 
                                msgNum, DCAN_IF_READ);

    /* Clear the NewData bit */
    //DCANNewDataControl(baseAdd, DCAN_NEW_DAT_CLR, DCAN_IF_READ);

    /* Read data bytes from interface register */
    
    DCANIFDataRead(baseAdd, DCAN_IF_READ, frame->data);
    *(unsigned int*)frame  = DCANIFArbRead(baseAdd,DCAN_IF_READ);
    frame->dlc = DCANIFDlcRead(baseAdd,DCAN_IF_READ);
}



void isr_DCANLine0(unsigned intnum)
{
    unsigned int errVal;
    unsigned int msgNum;
    unsigned int int0id;

    unsigned int membase = (SYS_INT_DCAN0_INT0 == intnum)?SOC_DCAN_0_REGS:SOC_DCAN_1_REGS;
    unsigned int index = (SYS_INT_DCAN0_INT0 == intnum)?0:1;
   
   while (1) {
      int0id = DCANIntRegStatusGet(membase, DCAN_INT_LINE0_STAT);
      if (DCAN_ERROR_OCCURED == int0id) {
         /* Check the status of DCAN Status and error register */
         errVal = DCANErrAndStatusRegInfoGet(membase);

         if (errVal & DCAN_MOD_IN_BUS_OFF_STATE) {
            g_can[index].fgBusOff = 1;
         }

         if (errVal & DCAN_ERR_WARN_STATE_RCHD) {
            g_can[index].fgErrWarn = 1;
         }
         if (errVal & DCAN_CORE_IN_ERR_PASSIVE) {
            g_can[index].fgErrPassive = 1;
         }
         g_can[index].LastErrCode = errVal & 0x03;
      }
      msgNum =  int0id & DCAN_INT_LINE0_STAT;
      if((msgNum>=0x01) && (msgNum<=0x80)){
         /* Interrupt handling for transmit objects */
        
        if((msgNum >= (DCAN_MSGOBF_TX_BEGIN)) 
             && (msgNum < (DCAN_MSGOBF_TX_BEGIN+DCAN_MSGOBF_TX_NUMBER))){
            /* Clear the Interrupt pending status */
            CANClrIntPndStat(membase, msgNum, DCAN_IF1_REG);
            g_can[index].fgSendFinish = 1;
         }

         if ((msgNum >= (DCAN_MSGOBF_RX_BEGIN)) 
             &&msgNum < (DCAN_MSGOBF_RX_BEGIN+DCAN_MSGOBF_RX_NUMBER)){
            /* Read a received message from message RAM to interface register */
            CANFrameRead(membase, msgNum, &canFrame);
            *(unsigned int*)&canFrame ^= 1<<29;
            /* Clear the Interrupt pending status */
            //CANClrIntPndStat(membase, msgNum, DCAN_IF2_REG);
            if (NULL != dcanRcvedHandler) {
               dcanRcvedHandler(index, &canFrame);
            }
         }
      }
      if (DCANIntRegStatusGet(membase, DCAN_INT_LINE0_STAT)==0) {
         return ;
      }
   }
}

BOOL CANSendFinishGetClr(unsigned int baseAddr) {
   unsigned int index = (SOC_DCAN_0_REGS == baseAddr) ? 0 : 1;
   BOOL val = g_can[index].fgSendFinish;
   g_can[index].fgSendFinish = 0;
   return val;
}



unsigned int CANSend_noblock(unsigned int baseAddr,CAN_FRAME *frame){
    unsigned int msgNum;
    if(CANSendFinishGetClr(baseAddr)==false)
       return CAN_SEND_PRE_SENDING;  
    while (DCANIFBusyStatusGet(baseAddr,DCAN_IF_WRITE));
    unsigned int arb = *(unsigned int *)frame;
    arb ^= (1<<29);
    HWREG(baseAddr + DCAN_IFARB(DCAN_IF_WRITE)) = arb | 0x80000000;
    DCANIFDataSet(baseAddr,DCAN_IF_WRITE,frame->data,frame->dlc);
    HWREG(baseAddr + DCAN_IFMCTL(DCAN_IF_WRITE)) |= DCAN_TRANSMIT_INT
       |DCAN_IFARB_MSGVAL | DCAN_IFMCTL_EOB | DCAN_IFMCTL_TXRQST 
         | DCAN_IFMCTL_NEWDAT;

    /* Get the transmit request status */
    msgNum = DCANFreeMsgObjGet(baseAddr,DCAN_MSGOBF_TX_BEGIN);

    /* Configure the command register */
    DCANCommandRegSet(baseAddr, (DCAN_DAT_A_ACCESS | DCAN_MSG_WRITE | DCAN_TXRQST_ACCESS | 
                                DCAN_DAT_B_ACCESS | DCAN_ACCESS_CTL_BITS | 
                                DCAN_ACCESS_ARB_BITS), msgNum, DCAN_IF_WRITE);
    return CAN_SEND_OK;
}


void CANInit(unsigned int baseAdd,unsigned int mode,unsigned int clkInFreq,unsigned int bitRate){
   unsigned index = (SOC_DCAN_0_REGS==baseAdd)? 0:1;
   g_can[index].fgSendFinish = 1;
   DCANReset(baseAdd);
   DCANIntLineEnable(baseAdd,DCAN_INT_LINE0);
   DCANIntEnable(baseAdd,DCAN_ERROR_INT); 
   if ((CAN_MODE_TEST_LOOPBACK==mode)){
      DCANTestModeControl(baseAdd,DCAN_TEST_MODE_ENABLE);
      DCANTestModesEnable(baseAdd,DCAN_TST_LPBCK_MD);
   }
   if (CAN_MODE_TEST_LOOPBACK_SILENT==mode) {
      DCANTestModeControl(baseAdd,DCAN_TEST_MODE_ENABLE);
      DCANTestModesEnable(baseAdd,DCAN_TST_LPBCK_SILENT_MD);
   }
   DCANConfigRegWriteAccessControl(baseAdd,DCAN_CONF_REG_WR_ACCESS_ENABLE);
   CANSetBitTiming(baseAdd,clkInFreq,bitRate);
   DCANConfigRegWriteAccessControl(baseAdd,DCAN_CONF_REG_WR_ACCESS_DISABLE);
   CANRxMsgObjectConfig(baseAdd);
   DCANNormalModeSet(baseAdd);
}

void DCANModuleClkConfig(void)
{
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) =
                             CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
     CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) =
                             CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
     CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) =
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) =
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) =
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) =
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) =
                             CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKCTRL) &
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_DCAN1_CLKCTRL) = 
                                  CM_PER_DCAN1_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_DCAN1_CLKCTRL) & 
                         CM_PER_DCAN1_CLKCTRL_MODULEMODE) != 
                         CM_PER_DCAN1_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_CM_PER_REGS + CM_PER_DCAN0_CLKCTRL) = 
                                  CM_PER_DCAN0_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_CM_PER_REGS + CM_PER_DCAN0_CLKCTRL) & 
                         CM_PER_DCAN0_CLKCTRL_MODULEMODE) != 
                         CM_PER_DCAN0_CLKCTRL_MODULEMODE_ENABLE);

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK |
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK))); 

    while(!(HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_CAN_CLK)));
}

void DCANMsgRAMInit(unsigned int instanceNum)
{
    if(0 == instanceNum)
    {
        HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) |= 
              CONTROL_DCAN_RAMINIT_DCAN0_RAMINIT_START;
        while (HWREG(SOC_CONTROL_REGS+CONTROL_DCAN_RAMINIT)&CONTROL_DCAN_RAMINIT_DCAN0_RAMINIT_DONE==0);       
    }
    else if (1==instanceNum) {
       HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) |= 
              CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_START;
        while (HWREG(SOC_CONTROL_REGS+CONTROL_DCAN_RAMINIT)&CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_DONE==0);   
       
    }
} 
