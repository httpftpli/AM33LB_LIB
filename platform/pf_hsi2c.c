/**
 *  \file   pf_hsi2c.c
 *
 *  \brief
 *  \author  lfl
 *  \addtogroup
 *  \# include "pf_hsi2c.h"
 *  @{
 *
 */

//! @}




#include "hw_control_AM335x.h"
#include "hw_cm_wkup.h"
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "hw_types.h"
#include "hsi2c.h"
#include "type.h"
#include "module.h"
#include "mmath.h"


#define I2C_MODE_SLAVE     0
#define I2C_MODE_MASTER    1


/**
 * @brief I2C init
 * @param [in] I2C moduleId \b MODULE_ID_I2CX
 * @param [in] i2cClkFreq   I2C bus frequency 
 * @param [in] slaveAddr slaveAddr buf when I2C work as 
 *        slaveAddr. when work as master ,the value should be
 *        NULL
 * @param [in] szSlave  number of slaveAdd (szSlave<=4 )
 * @return    NONE
 * @date    2013/5/29
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void I2CInit(unsigned int moduleId,unsigned int i2cClkFreq,
             unsigned short *slaveAddr,
             unsigned int szSlave)
{
   moduleEnable(moduleId);
   unsigned int baseAddr = modulelist[moduleId].baseAddr;
   I2CMasterReset(baseAddr);
   /* Put i2c in reset/disabled state */
   I2CMasterDisable(baseAddr);
   /* Disable auto Idle functionality */
   I2CAutoIdleDisable(baseAddr);
   /* Configure i2c bus speed to 100khz */
   I2CMasterInitExpClk(baseAddr, 48000000, 12000000, i2cClkFreq);
   I2CMasterIntEnableEx(baseAddr, I2C_INT_TRANSMIT_READY);
   if (slaveAddr!=NULL) {
      unsigned int n = MIN(4,szSlave);
      for (int i = 0; i < n; i++) {
         I2COwnAddressSet(baseAddr, slaveAddr[i], i);
      }
   }

/* Bring I2C out of reset */
   I2CMasterEnable(baseAddr);
}


/**
 * @brief I2C master send date to slave
 * @param [in] baseAddr I2C baseaddr \b SOC_I2C_X_REGS
 * @param [in] addr  slave addr
 * @param [in] buf   
 * @param [in] szWrite  byte of buf to be send
 * @return  BOOL
 * @date    2013/7/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL I2CMasterWrite(unsigned baseAddr,unsigned short addr,const void *buf,unsigned int szWrite){
   unsigned int datap = 0;
   I2CMasterIntClearEx(baseAddr, 0xffffffff);
   I2CModeSet(baseAddr, I2C_MODE_MASTER_SEND);
   I2CMasterSlaveAddrSet(baseAddr, addr);
   I2CFIFOClear(baseAddr,I2C_TX_MODE);
   if (szWrite<=32) {
      for (datap=0;datap<szWrite;datap++) {
         HWREG(baseAddr+I2C_DATA) = ((unsigned char *)buf)[datap];
      }
   } else {
      I2CFIFOThresholdConfig(baseAddr, 7, I2C_TX_MODE);
      for (datap = 0; datap < 32; datap++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)buf)[datap];
      }
   }
   I2CSetDataCount(baseAddr, szWrite);
   HWREG(baseAddr + I2C_CON) |= 0x03;
   //__asm volatile ("DSB");
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 8)) { //ARDY
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         return SUCCESS;
      } else if (st & 1 << 4) { //transmit data ready
         for (int i = 0; (datap < szWrite) && (i < 16); i++, datap++) {
            HWREG(baseAddr + I2C_DATA) = ((unsigned char *)buf)[datap];
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 4; //clear
      }
   }
}


/**
 * @brief master send data to slave
 * @param [in] baseAddr I2C baseaddr \b SOC_I2C_X_REGS
 * @param [in] slaveAddr  slave address
 * @param [in] w1buf   buf1 addr
 * @param [in] szWrite byte of w1buf
 * @param [in] w2buf  buf2 addr
 * @param [in] szW2  byte of w2buf
 * @return  BOOL
 * @date    2013/7/8
 * @note 
 * this function will block cpu. the funtion write data to slave
 * 2 times continuously .the purpose of the funtion is to 
 * convenient eeprom devide operation that send eeprom address 
 * first then data sencond 
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL I2CMasterWriteEx(unsigned baseAddr,unsigned short slaveAddr,const void *w1buf,unsigned int szW1,
                      const void *w2buf,unsigned int szW2){
   unsigned int datap1=0,datap2=0;
   I2CMasterIntClearEx(baseAddr, 0xffffffff);
   I2CModeSet(baseAddr, I2C_MODE_MASTER_SEND);
   I2CMasterSlaveAddrSet(baseAddr, slaveAddr);
   I2CFIFOClear(baseAddr,I2C_TX_MODE);
   if (szW1<=32) {
      for (datap1=0;datap1<szW1;datap1++) {
         HWREG(baseAddr+I2C_DATA) = ((unsigned char *)w1buf)[datap1];
      }
   } else {
      I2CFIFOThresholdConfig(baseAddr, 7, I2C_TX_MODE);
      for (datap1=0; datap1 < 32; datap1++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w1buf)[datap1];
      }
   }
   I2CSetDataCount(baseAddr, szW1);
   HWREG(baseAddr + I2C_CON) |= 0x01; //STAR ,NOSTOP
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 12)) { //ARDY ,BB
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         goto NEXT;
      } else if (st & 1 << 4) { //transmit data ready
         for (int i = 0; (datap1 < szW1) && (i < 16); i++, datap1++) {
            HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w1buf)[datap1];
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 4; //clear
      }
   }
NEXT:
   if (szW2 <= 32) {
      for (datap2 = 0; datap2 < szW2; datap2++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w2buf)[datap2];
      }
   } else {
      I2CFIFOThresholdConfig(baseAddr, 7, I2C_TX_MODE);
      for (datap2 = 0; datap2 < 32; datap2++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w2buf)[datap2];
      }
   }
   I2CSetDataCount(baseAddr, szW2);
   HWREG(baseAddr + I2C_CON) |= 0x3; //STAR STOP
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 8)) { //ARDY
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         return SUCCESS;
      } else if (st & 1 << 4) { //transmit data ready
         for (int i = 0; (datap2 < szW2) && (i < 16); i++, datap2++) {
            HWREG(baseAddr + I2C_DATA) = ((unsigned char *)w1buf)[datap2];
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 4; //clear
      }
   }
}



/**
 * @brief master read date from slave
 * @param [in] baseAddr  I2C base address \b SOC_I2C_X_REGS
 * @param [in] slaveAddr  I2C slave address
 * @param [out] buf 
 * @param [in] szRead 
 * @return  BOOL
 * @date    2013/7/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL I2CMasterRead(unsigned int baseAddr,unsigned short slaveAddr,void *buf,unsigned int szRead)
{
   unsigned int datap = 0;
   I2CMasterIntClearEx(baseAddr, 0xffffffff);
   I2CModeSet(baseAddr, I2C_MODE_MASTER_RCV);
   I2CMasterSlaveAddrSet(baseAddr, slaveAddr);
   I2CFIFOClear(baseAddr,I2C_RX_MODE);
   if (szRead<=32) {
      I2CFIFOThresholdConfig(baseAddr, szRead, I2C_RX_MODE);
   } else {
      I2CFIFOThresholdConfig(baseAddr, 24, I2C_RX_MODE);
   }
   I2CSetDataCount(baseAddr, szRead);
   HWREG(baseAddr + I2C_CON) |= 0x03;
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 8)) { //ARDY
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         return SUCCESS;
      } else if (st & 1 << 3) { //Receive data ready
         for (int i = 0; (i < 24)&&(datap < szRead); i++, datap++) {
           ((unsigned char *)buf)[datap] =  HWREG(baseAddr + I2C_DATA);
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 3; //clear
      }
   }
}

/**
 * @brief 
 * @param [in] baseAddr  \b SOC_I2C_X_REGS
 * @param [in] slaveAddr  
 * @param [in] writeBuf   
 * @param [in] szWrite 
 * @param [out] readBuf 
 * @param [in] szRead 
 * @return  BOOL
 * @date    2013/7/8
 * @note 
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL I2CMasterReadEx(unsigned baseAddr,unsigned short slaveAddr,const void *writeBuf ,unsigned int szWrite,
                     void *readBuf,unsigned int szRead){
   unsigned int datap1=0,datap2 =0;
   I2CMasterIntClearEx(baseAddr, 0xffffffff);
   I2CModeSet(baseAddr, I2C_MODE_MASTER_SEND);
   I2CMasterSlaveAddrSet(baseAddr, slaveAddr);
   I2CFIFOClear(baseAddr,I2C_TX_MODE);
   if (szWrite<=32) {
      for (datap1=0;datap1<szWrite;datap1++) {
         HWREG(baseAddr+I2C_DATA) = ((unsigned char *)writeBuf)[datap1];
      }
   } else {
      I2CFIFOThresholdConfig(baseAddr, 7, I2C_TX_MODE);
      for (datap1=0; datap1 < 32; datap1++) {
         HWREG(baseAddr + I2C_DATA) = ((unsigned char *)writeBuf)[datap1];
      }
   }
   I2CSetDataCount(baseAddr, szWrite);
   HWREG(baseAddr + I2C_CON) |= 0x01; //STAR ,NOSTOP
   //__asm volatile ("DSB");
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 12)) { //ARDY ,BB
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         goto NEXT;
      } else if (st & 1 << 4) { //transmit data ready
         for (int i = 0; (datap1 < szWrite) && (i < 16); i++, datap1++) {
            HWREG(baseAddr + I2C_DATA) = ((unsigned char *)writeBuf)[datap1];
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 4; //clear
      }
   }
NEXT:
   I2CModeSet(baseAddr, I2C_MODE_MASTER_RCV);
   I2CFIFOClear(baseAddr,I2C_RX_MODE);
   if (szRead<=32) {
      I2CFIFOThresholdConfig(baseAddr, szRead, I2C_RX_MODE);
   } else {
      I2CFIFOThresholdConfig(baseAddr, 24, I2C_RX_MODE);
   }
   I2CSetDataCount(baseAddr, szRead);
   HWREG(baseAddr + I2C_CON) |= 0x03; //START STOP
   while (1) {
      unsigned int st = I2CMasterIntRawStatus(baseAddr);
      if (st & 1 << 1) { //NACK
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 1;
         return FAIL;
      } else if ((st & 1 << 2) && (st & 1 << 8)) { //ARDY
         HWREG(baseAddr + I2C_IRQSTATUS) |= (1 << 2) | (1 << 8);
         return SUCCESS;
      } else if (st & 1 << 3) { //Receive data ready
         for (int i = 0; (i < 24)&&(datap2 < szRead); i++, datap2++) {
           ((unsigned char *)readBuf)[datap2] =  HWREG(baseAddr + I2C_DATA);
         }
         HWREG(baseAddr + I2C_IRQSTATUS) |= 1 << 3; //clear
      }
   }

}

//! @}
