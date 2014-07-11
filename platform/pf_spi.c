/**
 *  \file   pf_spi.c
 *
 *  \brief
 *  \author  lfl 
 *  \addtogroup SPI
 *  \# include "pf_spi.h"
 *  @{
 *   
 */


#include "mcspi.h"
#include "type.h"
#include "debug.h"
#include "module.h"
#include "pf_spi.h"

volatile SPI_TRANSFER g_spitransfer= {.finish = 1};


void isr_mcspi(unsigned int intnum) {
   unsigned int intCode = 0;
   unsigned int addr = modulelist[intnum].baseAddr;
   intCode = McSPIIntStatusGet(addr);
   McSPIIntStatusClear(addr, intCode);
   switch (g_spitransfer.state) {
   case STATE_OUTPUT_PREFIX:
      if (MCSPI_INT_TX_EMPTY(0) == (intCode & MCSPI_INT_TX_EMPTY(0))) {
         unsigned int count;
         void *buf;
         if (g_spitransfer.inWrCheck) {
            count = g_spitransfer.checkprefixWrbufLen;
            buf = g_spitransfer.checkPrefixWrbuf;
         } else {
            count = g_spitransfer.prefixWrbufLen;
            buf = g_spitransfer.prefixWrbuf;
         }
         for (int i = 0; i < count; i++) {
            McSPITransmitData(addr, *((unsigned char *)buf + i), 0);
         }
         if (0 == g_spitransfer.dir) {
            g_spitransfer.state = STATE_OUTPUT_BUF;
            goto OUTPUTBUF;
         } else {
            g_spitransfer.state = STATE_INPUT;         
            goto INPUTBUF;
         }
      } 
      break;
OUTPUTBUF:   
   case STATE_OUTPUT_BUF:
      if (MCSPI_INT_TX_EMPTY(0) == (intCode & MCSPI_INT_TX_EMPTY(0))) {
         for (int i = g_spitransfer.bufpointer; i < g_spitransfer.len; i++) {
            if (McSPITransmitData(addr, *((unsigned char *)(g_spitransfer.buf) + i), 0)) {
               g_spitransfer.bufpointer++;
            } else {
               break;
            }
         }
      }
      if (MCSPI_INT_EOWKE & intCode) {
         g_spitransfer.bufpointer = 0;
         if (1 == g_spitransfer.needWrCheck) {
            McSPIChannelDisable(addr, 0);
            McSPICSDeAssert(addr,0);
            g_spitransfer.inWrCheck = 1;
            g_spitransfer.dir = 1;
            g_spitransfer.bufpointer = 0;
            g_spitransfer.txprivate = 0;
            g_spitransfer.rxprivate = 0;
            g_spitransfer.state = STATE_OUTPUT_PREFIX;
            McSPIWordCountSet(addr,g_spitransfer.checkprefixWrbufLen + g_spitransfer.len +1);
            McSPICSAssert(addr,0);
            McSPIChannelEnable(addr, 0);  // restart spi to check pre write data
         } else {
            g_spitransfer.state = STATE_OK;
            g_spitransfer.finish = 1;
            McSPIChannelDisable(addr, 0);
            McSPICSDeAssert(addr,0);
         }
      }
      if (MCSPI_INT_RX_FULL(0) == (intCode & MCSPI_INT_RX_FULL(0))) {
         while (!(MCSPI_CH_RXFFE & McSPIChannelStatusGet(addr, 0))) {
            McSPIReceiveData(addr, 0);
         }
      }
      break;
INPUTBUF:
   case STATE_INPUT:
      if (MCSPI_INT_TX_EMPTY(0) == (intCode & MCSPI_INT_TX_EMPTY(0))) {
         for (int i = g_spitransfer.txprivate; i <= g_spitransfer.len; i++) {//send len  = output len + 1
            if (McSPITransmitData(addr, ((unsigned char *)g_spitransfer.buf)[i], 0)) {
               g_spitransfer.txprivate++;
            } else {
               break;
            }
         }       
      }
      if (MCSPI_INT_RX_FULL(0) == (intCode & MCSPI_INT_RX_FULL(0))) {
         while (!(MCSPI_CH_RXFFE & McSPIChannelStatusGet(addr, 0))) {
            if (g_spitransfer.inWrCheck == 0) {
              if(g_spitransfer.rxprivate++ < g_spitransfer.prefixWrbufLen){
                   McSPIReceiveData(addr, 0);
              }else{
                   if(g_spitransfer.bufpointer == g_spitransfer.len) break;
                   ((char *)(g_spitransfer.buf))[g_spitransfer.bufpointer++] = (char)McSPIReceiveData(addr, 0);
              }
            } else {
              if(g_spitransfer.rxprivate++ < g_spitransfer.prefixWrbufLen){
                   McSPIReceiveData(addr, 0);
              }else{              
                  if(g_spitransfer.bufpointer == g_spitransfer.len) break;                
                  if((char)McSPIReceiveData(addr, 0) != ((char *)(g_spitransfer.buf))[g_spitransfer.bufpointer++]) {
                      g_spitransfer.state = STATE_CHECKERROR;
                      g_spitransfer.finish = 1;
                      McSPIChannelDisable(addr, 0);
                      McSPICSDeAssert(addr,0);
                      McSPIWordCountSet(addr,0);
                      return;
                  }
               }
            }
         }
      }
      if (MCSPI_INT_EOWKE & intCode) {
          while (!(MCSPI_CH_RXFFE & McSPIChannelStatusGet(addr, 0))) {
            if (g_spitransfer.inWrCheck == 0) {
              if(g_spitransfer.rxprivate++ < g_spitransfer.prefixWrbufLen){
                   McSPIReceiveData(addr, 0);
              }else{
                   if(g_spitransfer.bufpointer == g_spitransfer.len) break;
                   ((char *)(g_spitransfer.buf))[g_spitransfer.bufpointer++] = (char)McSPIReceiveData(addr, 0);
              }
            } else {
              if(g_spitransfer.rxprivate++ < g_spitransfer.prefixWrbufLen){
                   McSPIReceiveData(addr, 0);
              }else{
                  if(g_spitransfer.bufpointer == g_spitransfer.len) break;
                  if((char)McSPIReceiveData(addr, 0) != ((char *)(g_spitransfer.buf))[g_spitransfer.bufpointer++]) {
                      g_spitransfer.state = STATE_CHECKERROR;
                      g_spitransfer.finish = 1;
                      McSPIChannelDisable(addr, 0);
                      McSPICSDeAssert(addr,0);
                      return;
                  }
               }
            }
         }
         g_spitransfer.state = STATE_OK;
         g_spitransfer.finish = 1;
         McSPIChannelDisable(addr, 0);
         McSPICSDeAssert(addr,0);
      }
      break;
   default:
      break;
   }
}


SPI_TRANSFER_STATE SPIGetState(unsigned int moduleId){
   UNUSED(moduleId);
   return g_spitransfer.state;
}
BOOL isSpiTransferFinish(unsigned int moduleId){
   UNUSED(moduleId);
   return g_spitransfer.finish;
}


/**
 * @brief  SPI 读
 * @param [in] moduleId SPI模块号
 * @param [in] wrBuf 
 *        写入的缓冲区指针，此缓冲区用于读的时候预写入，比如SPI接口的FALSH指定读取地址
 *    
 * @param [in] wrBufLen 写缓冲区长度
 * @param [in] buf  读缓冲区指针
 * @param [in] len  指定的读数据长度 
 * @return  BOOL         
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
BOOL SPIRead(unsigned int moduleId,void *wrBuf, unsigned int wrBufLen, void *buf, unsigned int len) {
   ASSERT(wrBufLen <= 16);
   if (g_spitransfer.finish == 0) {
      return FALSE;
   }
   unsigned int addr = modulelist[moduleId].baseAddr;
   g_spitransfer.prefixWrbuf = wrBuf;
   g_spitransfer.prefixWrbufLen = wrBufLen;
   g_spitransfer.buf = buf;
   g_spitransfer.len = len;
   g_spitransfer.bufpointer = 0;
   g_spitransfer.txprivate = 0;
   g_spitransfer.rxprivate = 0;
   g_spitransfer.dir = 1;
   g_spitransfer.needWrCheck = 0;
   g_spitransfer.inWrCheck = 0;
   g_spitransfer.finish = 0;
   g_spitransfer.state = STATE_OUTPUT_PREFIX;
   McSPIWordCountSet(addr,wrBufLen + len +1);
   McSPICSAssert(addr,0);
   McSPIChannelEnable(addr, 0);
   return TRUE;
}


/**
 * @brief SPI写
 * @param [in] moduleId SPI模块号
 * @param [in] wrbuf0  写缓冲区指针0          
 * @param [in] lenOfBuf0 写缓冲区指针0长度
 * @param [in] wrbuf1  写缓冲区指针1 
 * @param [in] lenOfBuf1 写缓冲区指针1长度
 * @param [in] needCheck 是否校验
 * @param [in] checkPrefixWrbuf 校验命令缓冲区
 * @param [in] checkPrefixWrbufLen 校验命令缓冲区长度
 * @return  BOOL         
 * @date    2013/8/8
 * @note 
 *  wrbuf0用于写入写命令，比如写spi接口的FLASH时，指定写命令和地址，checkPrefixWrbuf用于指定校验的命令，比如写spi接口的FLASH时，指定校验的读命令和地址。
 *  当needCheck为FALSE是，checkPrefixWrbuf应为checkPrefixWrbuf，checkPrefixWrbufLen应为0
 * @code
 * @endcode
 * @pre
 * @see 
 */
BOOL SPIWrite(unsigned int moduleId,void *wrbuf0, unsigned int lenOfBuf0, void *wrbuf1,unsigned int lenOfBuf1, BOOL needCheck, void *checkPrefixWrbuf, unsigned int checkPrefixWrbufLen) {
   ASSERT(lenOfBuf0 <= 16);
   if (g_spitransfer.finish == 0) {
      return FALSE;
   }else if(g_spitransfer.state == STATE_CHECKERROR){
      return FALSE;
   }
   unsigned int addr = modulelist[moduleId].baseAddr;
   g_spitransfer.prefixWrbuf = wrbuf0;
   g_spitransfer.prefixWrbufLen = lenOfBuf0;
   g_spitransfer.buf = wrbuf1;
   g_spitransfer.len = lenOfBuf1;
   g_spitransfer.bufpointer = 0;
   g_spitransfer.txprivate = 0;
   g_spitransfer.rxprivate = 0;
   g_spitransfer.dir = 0;
   g_spitransfer.needWrCheck = needCheck;
   g_spitransfer.checkPrefixWrbuf = checkPrefixWrbuf;
   g_spitransfer.checkprefixWrbufLen = checkPrefixWrbufLen;
   g_spitransfer.inWrCheck = 0;
   g_spitransfer.finish = 0;
   g_spitransfer.state = STATE_OUTPUT_PREFIX;
   McSPIWordCountSet(addr,lenOfBuf0 + lenOfBuf1);
   McSPICSAssert(addr,0);
   McSPIChannelEnable(addr, 0);   
   return TRUE;
}



/**
 * @brief SPI主机初始化
 * @param [in] moduleId SPI模块号
 * @param [in] csChanel 哪个CS        
 * @param [in] spiClk SPI clk频率 
 * @param [in]  cpha SPI相位
 * @param [in]  cpol SPI时钟极性
 * @param [in] wordLen字长
 * @return           
 * @date    2013/8/8
 * @note 
 * cpha cpol 参数和SPI总线规范的定义一致
 * @code
 * @endcode
 * @pre
 * @see 
 */
void SPIMasterInit(unsigned int moduleId, unsigned char csChanel, unsigned int spiClk, unsigned char cpha, unsigned char cpol, unsigned char csPolar,unsigned char wordLen) {
   ASSERT(cpha < 2);
   ASSERT(cpol < 2);
   ASSERT((wordLen >= 4) && (wordLen <= 32));
   ASSERT(csChanel <= 3);
   moduleEnable(moduleId);
   unsigned int addr = modulelist[moduleId].baseAddr;
   unsigned int inclk = modulelist[moduleId].moduleClk->fClk[0]->clockSpeedHz;
   McSPIReset(addr);
   McSPICSEnable(addr);
   McSPIMasterModeEnable(addr);
   /* Perform the necessary configuration for master mode.*/
   McSPIMasterModeConfig(addr, MCSPI_SINGLE_CH,
                         MCSPI_TX_RX_MODE, MCSPI_DATA_LINE_COMM_MODE_1,
                         csChanel);
   /* Configure the McSPI bus clock depending on clock mode. */
   McSPIClkConfig(addr, inclk, spiClk, csChanel,
                  cpol << 1 | cpha);

   /* Configure the word length.*/
   McSPIWordLengthSet(addr, MCSPI_WORD_LENGTH(wordLen), csChanel);

   /* Set polarity of SPIEN */
   McSPICSPolarityConfig(addr, (csPolar==0)?MCSPI_CS_POL_LOW:MCSPI_CS_POL_HIGH, csChanel);
   //McSPITurboModeEnable(addr,0);

   /* Enable the transmitter FIFO of McSPI peripheral.*/
   McSPITxFIFOConfig(addr, MCSPI_TX_FIFO_ENABLE, csChanel);
   /* Enable the receiver FIFO of McSPI peripheral.*/
   McSPIRxFIFOConfig(addr, MCSPI_RX_FIFO_ENABLE, csChanel);
   McSPIFIFOTrigLvlSet(addr,24,1,MCSPI_TX_RX_MODE);
   McSPIIntEnable(addr,MCSPI_INT_TX_EMPTY(0)| MCSPI_INT_RX_FULL(0)| MCSPI_INT_EOWKE );
   moduleIntConfigure(moduleId);
}


/**
 * @brief 
 *        SPI主机初始化,只发送，硬件有64字节的fifo全部用于发送，所以关闭了发送中断
 * @param [in] moduleId SPI模块号
 * @param [in] csChanel 哪个CS        
 * @param [in] spiClk SPI clk频率 
 * @param [in]  cpha SPI相位
 * @param [in]  cpol SPI时钟极性
 * @param [in] wordLen字长
 * @return           
 * @date    2013/8/8
 * @note 
 * cpha cpol 参数和SPI总线规范的定义一致
 * @code
 * @endcode
 * @pre
 * @see 
 */
void SPIMasterInit_SendOnly(unsigned int moduleId, unsigned char csChanel, unsigned int spiClk, unsigned char cpha, unsigned char cpol, unsigned char csPolar,unsigned char wordLen) {
   ASSERT(cpha < 2);
   ASSERT(cpol < 2);
   ASSERT((wordLen >= 4) && (wordLen <= 32));
   ASSERT(csChanel <= 3);
   moduleEnable(moduleId);
   unsigned int addr = modulelist[moduleId].baseAddr;
   unsigned int inclk = modulelist[moduleId].moduleClk->fClk[0]->clockSpeedHz;
   McSPIReset(addr);
   McSPICSEnable(addr);
   McSPIMasterModeEnable(addr);
   /* Perform the necessary configuration for master mode.*/
   McSPIMasterModeConfig(addr, MCSPI_MULTI_CH,
                         MCSPI_TX_ONLY_MODE, MCSPI_DATA_LINE_COMM_MODE_1,
                         csChanel);
   /* Configure the McSPI bus clock depending on clock mode. */
   McSPIClkConfig(addr, inclk, spiClk, csChanel,
                  cpol << 1 | cpha);

   /* Configure the word length.*/
   McSPIWordLengthSet(addr, MCSPI_WORD_LENGTH(wordLen), csChanel);

   /* Set polarity of SPIEN */
   McSPICSPolarityConfig(addr, (csPolar==0)?MCSPI_CS_POL_LOW:MCSPI_CS_POL_HIGH, csChanel);
   //McSPITurboModeEnable(addr,0);

   /* Enable the transmitter FIFO of McSPI peripheral.*/
   McSPITxFIFOConfig(addr, MCSPI_TX_FIFO_ENABLE, csChanel);
   moduleIntConfigure(moduleId);
   McSPIChannelEnable(addr, 0);
}

/**
 * @brief SPI写
 * @param [in] moduleId SPI模块号
 * @param [in] data  待发送的数据，8bit 
 * @return  BOOL         
 * @date    2013/8/8
 * @note 
 * @code
 * @endcode
 * @pre
 * @see 
 */
void SPIWrite_SendOnly(unsigned int moduleId, unsigned short data) {
   unsigned int addr  =  modulelist[moduleId].baseAddr;  
   while(McSPITransmitData(addr, (unsigned int)data, 0)==0);
}



//! @}
//! 
