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


typedef struct __spitrasfer{
   void * prefixWrbuf;
   unsigned int prefixWrbufLen;
   void * buf;
   unsigned int len;
   unsigned int bufpointer;
   unsigned short dir;
   unsigned short needWrCheck:1
   unsigned short writeFinish:1;  
   volatile unsigned short finish:1;
} SPI_TRANSFER;


SPI_TRANSFER transfer;

static void isr_mcspi(unsigned int intnum)
{
    unsigned int intCode = 0;
    unsigned int addr = modulelist[intnum].baseAddr;
    intCode = McSPIIntStatusGet(addr);
    while(intCode)
    {
        if(MCSPI_INT_TX_EMPTY(chNum) == (intCode & MCSPI_INT_TX_EMPTY(chNum)))
        {
            McSPIIntStatusClear(addr, MCSPI_INT_TX_EMPTY(chNum));
            if (0==transfer.dir) { //read
               while (McSPITransmitData(addr,0,chNum)) {
                  transfer.bufpointer++;  
               }
               if (transfer.bufpointer==transfer.len) {
                  transfer.finish = 1;
                  McSPIChannelDisable(addr,0);

               }
            }
        }
        if(MCSPI_INT_RX_FULL(chNum) == (intCode & MCSPI_INT_RX_FULL(chNum)))
        {
            McSPIIntStatusClear(SOC_SPI_0_REGS, MCSPI_INT_RX_FULL(chNum));

            *p_rx++ = (unsigned char) McSPIReceiveData(SOC_SPI_0_REGS, chNum);

            if(!(length))
            {
                McSPIIntDisable(SOC_SPI_0_REGS, MCSPI_INT_RX_FULL(chNum));

                flag = 0;
            }
        }

        intCode = McSPIIntStatusGet(SOC_SPI_0_REGS);
    }
}


void SPIMasterInit(unsigned int moduleId,unsigned char csChanel,unsigned int spiClk,unsigned char cpha,unsigned char cpol,unsigned char wordLen){
   ASSERT(cpha<2);
   ASSERT(cpol<2);
   ASSERT((wordLen>=4)&&(wordLen<=32));
   moduleEnable(moduleID);
   moduleIntConfigure(unsigned int moduleID);
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
                  cpol<<1|cpha );

   /* Configure the word length.*/
   McSPIWordLengthSet(addr, MCSPI_WORD_LENGTH(wordLen), csChanel);

   /* Set polarity of SPIEN to low.*/
   McSPICSPolarityConfig(addr, MCSPI_CS_POL_LOW, csChanel);

   /* Enable the transmitter FIFO of McSPI peripheral.*/
   McSPITxFIFOConfig(addr, MCSPI_TX_FIFO_ENABLE, csChanel);

   /* Enable the receiver FIFO of McSPI peripheral.*/
   McSPIRxFIFOConfig(addr, MCSPI_RX_FIFO_ENABLE, csChanel);

   McSPITransmitData()
}







//! @}
//! 
