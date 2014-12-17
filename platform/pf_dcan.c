/**
 *  \file   pf_dcan.c
 *
 *  \brief CAN控制器API
 *  \author  李飞亮  
 *
 * @addtogroup CANBUS   
 * @{ 
 */



#include "dcan.h"
#include "soc_AM335x.h"
#include "hw_types.h"
#include "interrupt.h"
#include "hw_cm_per.h"
#include "hw_control_AM335x.h"
#include "module.h"
#include "pf_can.h"

#define DCAN_ERROR_OCCURED       (0x8000u)
/* Total No. of message objects available in the DCAN message RAM */
#define CAN_NUM_OF_MSG_OBJS      (64u)



#define DCAN_NUM_OF_MSG_OBJS    (64u)
#define DCAN_MSGOBF_RX_BEGIN    1
#define DCAN_MSGOBF_RX_NUMBER   DCAN_NUM_OF_MSG_OBJS/2
#define DCAN_MSGOBF_TX_BEGIN    DCAN_MSGOBF_RX_BEGIN+DCAN_MSGOBF_RX_NUMBER
#define DCAN_MSGOBF_TX_NUMBER   DCAN_NUM_OF_MSG_OBJS/2



volatile G_CAN g_can[2];

static CAN_FRAME canFrame;

static void (*dcanRcvedHandler)(unsigned int index,CAN_FRAME *frame)
   = (void (*)(unsigned int ,CAN_FRAME *))0;
        
  


/**
 * @brief 注册CAN接受回调函数 
 *  
 * 当CAN控制器收到CAN帧后调用注册的回调函数 
 * ,回调函数在中断线程中执行 
 * @return  void         
 * @date    2013/5/7
 * @note 
 * @code
 * 
 * @endcode
 *
 * @pre
 *
 * @see 
 */

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



__weak void isr_DCANLine0(unsigned intnum)
{
    unsigned int errVal;
    unsigned int msgNum;
    unsigned int int0id;

    unsigned int membase = modulelist[intnum].baseAddr;
    unsigned int index = modulelist[intnum].index;
   
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


BOOL CANSendFinishGetClr(unsigned int moduleId) {
   unsigned int index = modulelist[moduleId].index;
   BOOL val = g_can[index].fgSendFinish;
   g_can[index].fgSendFinish = 0;
   return val;
}



unsigned int CANSend_noblock(unsigned int moduleId,CAN_FRAME *frame){
    unsigned int baseAddr = modulelist[moduleId].baseAddr;
    unsigned int msgNum; 
    while (DCANIFBusyStatusGet(baseAddr,DCAN_IF_WRITE));
    unsigned int arb = *(unsigned int *)frame;
    arb ^= (1<<29);
    HWREG(baseAddr + DCAN_IFARB(DCAN_IF_WRITE)) = arb | 0x80000000;
    DCANIFDataSet(baseAddr,DCAN_IF_WRITE,frame->data,frame->dlc);
    HWREG(baseAddr + DCAN_IFMCTL(DCAN_IF_WRITE)) |= DCAN_TRANSMIT_INT
       |DCAN_IFARB_MSGVAL | DCAN_IFMCTL_EOB | DCAN_IFMCTL_TXRQST 
         | DCAN_IFMCTL_NEWDAT;

    /* Get the transmit request status */
    while((msgNum = DCANFreeMsgObjGet(baseAddr,DCAN_MSGOBF_TX_BEGIN))== -1);
    /* Configure the command register */
    DCANCommandRegSet(baseAddr, (DCAN_DAT_A_ACCESS | DCAN_MSG_WRITE | DCAN_TXRQST_ACCESS | 
                                DCAN_DAT_B_ACCESS | DCAN_ACCESS_CTL_BITS | 
                                DCAN_ACCESS_ARB_BITS), msgNum, DCAN_IF_WRITE);
    return CAN_SEND_OK;
}


bool IsMsgObjFull(unsigned int moduleId){
 unsigned int addr = modulelist[moduleId].baseAddr;
 if(DCANFreeMsgObjGet(addr,DCAN_MSGOBF_TX_BEGIN)==-1){
     return true;
 }else{
     return false;
 } 
}


static void DCANMsgRAMInit(unsigned int instanceNum)
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


/**
 * @brief CAN控制器初始化
 * @param [in] moduleId CAN控制器模块号 \b MODULE_ID_DCANX
 * @param [in] mode 
 * - CAN_MODE_NORMAL \n  - CAN_MODE_TEST_LOOPBACK \n 
 * - CAN_MODE_TEST_LOOPBACK_SILENT
 * @param [in] bitRate CAN总线频率
 * @return   void        
 * @date    2013/5/7
 * @note  
 * 示例代码如下：
 * @code
 * 
 * @endcode
 *
 * @pre
 *
 * @see 
 */
void DCANInit(unsigned int moduleId,unsigned int mode,unsigned int bitRate){
   MODULE *module = modulelist+moduleId; 
   unsigned int index = module->index;
   g_can[index].fgSendFinish = 1;
   unsigned int baseAdd = module->baseAddr;
   unsigned int clkInFreq = module->moduleClk->fClk[0]->clockSpeedHz;
   moduleEnable(moduleId);
   DCANMsgRAMInit(index);
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
   DCANAutoBusOnControl(baseAdd, DCAN_AUTO_BUS_ON_ENABLE);
   DCANConfigRegWriteAccessControl(baseAdd,DCAN_CONF_REG_WR_ACCESS_DISABLE);
   CANRxMsgObjectConfig(baseAdd);
   DCANNormalModeSet(baseAdd);
   moduleIntConfigure(moduleId);
}



/**
 * @}
 */
