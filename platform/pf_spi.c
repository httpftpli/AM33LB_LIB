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

SPI_TRANSFER g_spitransfer;


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
            g_spitransfer.state = STATE_CHECK;
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
         while (McSPITransmitData(addr, 0, 0));
      }
      if (MCSPI_INT_RX_FULL(0) == (intCode & MCSPI_INT_RX_FULL(0))) {
         while (!(MCSPI_CH_RXFFE & McSPIChannelStatusGet(addr, 0))) {
            if (g_spitransfer.inWrCheck == 0) {
               ((char *)(g_spitransfer.buf))[g_spitransfer.bufpointer++] = (char)McSPIReceiveData(addr, 0);
            } else {
               if ((char)McSPIReceiveData(addr, 0) != ((char *)(g_spitransfer.buf))[g_spitransfer.bufpointer++]) {
                  g_spitransfer.state = STATE_CHECKERROR;
                  g_spitransfer.finish = 1;
                  McSPIChannelDisable(addr, 0);
                  McSPICSDeAssert(addr,0);
                  return;
               }
            }
         }
      }
      if (MCSPI_INT_EOWKE & intCode) {
         while (!(MCSPI_CH_RXFFE & McSPIChannelStatusGet(addr, 0))) {
            if (g_spitransfer.inWrCheck == 0) {
               ((char *)(g_spitransfer.buf))[g_spitransfer.bufpointer++] =  McSPIReceiveData(addr, 0);
            } else {
               if ((char)McSPIReceiveData(addr, 0) != ((char *)(g_spitransfer.buf))[g_spitransfer.bufpointer++]) {
                  g_spitransfer.state = STATE_CHECKERROR;
                  g_spitransfer.finish = 1;
                  McSPIChannelDisable(addr, 0);
                  McSPICSDeAssert(addr,0);
                  return;
               }
            }
         }
         g_spitransfer.state = STATE_OK;
         g_spitransfer.finish = 1;
         McSPIChannelDisable(addr, 0);
         McSPICSDeAssert(addr,0);
      }
      break;
   case STATE_CHECK:
      g_spitransfer.inWrCheck = 1;
      g_spitransfer.dir = 1;
      g_spitransfer.bufpointer = 0;
      g_spitransfer.state = STATE_OUTPUT_PREFIX;
      break;
   default:
      break;
   }
}


BOOL SPIRead(unsigned int moduleId,void *wrBuf, unsigned int wrBufLen, void *buf, unsigned int len) {
   ASSERT(wrBufLen <= 16);
   if (g_spitransfer.finish != 0) {
      return FALSE;
   }
   unsigned int addr = modulelist[moduleId].baseAddr;
   g_spitransfer.prefixWrbuf = wrBuf;
   g_spitransfer.prefixWrbufLen = wrBufLen;
   g_spitransfer.buf = buf;
   g_spitransfer.len = len;
   g_spitransfer.bufpointer = 0;
   g_spitransfer.dir = 1;
   g_spitransfer.needWrCheck = 0;
   g_spitransfer.inWrCheck = 0;
   g_spitransfer.finish = 0;
   g_spitransfer.state = STATE_OUTPUT_PREFIX;
   McSPICSAssert(addr,0);
   McSPIChannelEnable(addr, 0);
   return TRUE;
}


BOOL SPIWrite(unsigned int moduleId,void *wrbuf0, unsigned int lenOfBuf0, void *wrbuf1,unsigned int lenOfBuf1, BOOL needCheck, void *checkPrefixWrbuf, unsigned int checkPrefixWrbufLen) {
   ASSERT(lenOfBuf0 <= 16);
   if (g_spitransfer.finish != 0) {
      return FALSE;
   }
   unsigned int addr = modulelist[moduleId].baseAddr;
   g_spitransfer.prefixWrbuf = wrbuf0;
   g_spitransfer.prefixWrbufLen = lenOfBuf0;
   g_spitransfer.buf = wrbuf1;
   g_spitransfer.len = lenOfBuf1;
   g_spitransfer.bufpointer = 0;
   g_spitransfer.dir = 0;
   g_spitransfer.needWrCheck = needCheck;
   g_spitransfer.checkPrefixWrbuf = checkPrefixWrbuf;
   g_spitransfer.checkprefixWrbufLen = checkPrefixWrbufLen;
   g_spitransfer.inWrCheck = 0;
   g_spitransfer.finish = 0;
   g_spitransfer.state = STATE_OUTPUT_PREFIX;
   McSPIWordCountSet(addr,g_spitransfer.prefixWrbufLen + g_spitransfer.len);
   McSPICSAssert(addr,0);
   McSPIChannelEnable(addr, 0);   
   return TRUE;
}

void SPIMasterInit(unsigned int moduleId, unsigned char csChanel, unsigned int spiClk, unsigned char cpha, unsigned char cpol, unsigned char wordLen) {
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
                         MCSPI_TX_RX_MODE, MCSPI_DATA_LINE_COMM_MODE_6,
                         csChanel);
   /* Configure the McSPI bus clock depending on clock mode. */
   McSPIClkConfig(addr, inclk, spiClk, csChanel,
                  cpol << 1 | cpha);

   /* Configure the word length.*/
   McSPIWordLengthSet(addr, MCSPI_WORD_LENGTH(wordLen), csChanel);

   /* Set polarity of SPIEN to low.*/
   McSPICSPolarityConfig(addr, MCSPI_CS_POL_LOW, csChanel);
   //McSPITurboModeEnable(addr,0);

   /* Enable the transmitter FIFO of McSPI peripheral.*/
   McSPITxFIFOConfig(addr, MCSPI_TX_FIFO_ENABLE, csChanel);
   /* Enable the receiver FIFO of McSPI peripheral.*/
   McSPIRxFIFOConfig(addr, MCSPI_RX_FIFO_ENABLE, csChanel);
   McSPIFIFOTrigLvlSet(addr,24,8,MCSPI_TX_RX_MODE);
   McSPIIntEnable(addr,MCSPI_INT_TX_EMPTY(0)| MCSPI_INT_RX_FULL(0)| MCSPI_INT_EOWKE );
   moduleIntConfigure(moduleId);
}








//! @}
//! 
