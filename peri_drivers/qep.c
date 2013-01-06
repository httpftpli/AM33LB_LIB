#include "hw_types.h"
#include "hw_eqep.h"
#include "qep.h"
#include "debug.h"


const unsigned int POSORIGIN = 50000;
//unittime us;unitposition um;positionFactor 0.1um,capTimeFactor ns;
unsigned int unitPosition,positionFactor = 1,capTimeFactor;

static unsigned int oldpos[3];

static unsigned int unitTime ,inFreq;

void QEPSetPosFactor(unsigned int factor){
   positionFactor = factor;
   mdAssert(positionFactor!=0);
}

static void QEPSetInputMode(unsigned int baseAddr,unsigned int mode){
   unsigned int val = HWREGH(baseAddr + EQEP_QDECCTL);
   val &= ~(3<<14);
   val |= mode << 14;
   HWREGH(baseAddr + EQEP_QDECCTL) = val;
}


void QEPSwapQuadInput(unsigned int baseAddr){
   HWREGH(baseAddr+EQEP_QDECCTL) |= 1<<10;
}




void QEPInit(unsigned int baseAddr,unsigned int infreq,unsigned int inputmode){
   inFreq = infreq;
   //soft reset Quadrature position counter
   HWREGH(baseAddr+EQEP_QEPCTL)&= ~(1<<3); 
   QEPSetInputMode(baseAddr,inputmode);
   //disable position compare syn output
   HWREGH(baseAddr+EQEP_QDECCTL) |= 0<<13;
   unitPosition = positionFactor * 2;
   capTimeFactor = 1000*1000*8/infreq;
   //Capture Control:enable; clock prescaler:CAPCLK = SYSCLKOUT/128 ;Unit position:QCLK/2 
   HWREGH(baseAddr+EQEP_QCAPCTL) = 1<<15 | 7<<4 | 1<<0; 
   //position counter reset on maximum position;
   HWREGHBITARRAY(baseAddr+EQEP_QEPCTL,12,2,1);
   //maximum position :0xffffffff
   HWREG(baseAddr+EQEP_QPOSMAX) = 0xffffffff;
   //campare position :0xffffffff
   HWREG(baseAddr+EQEP_QPOSCMP) = 0xffffffff;
   //int enable  UTO,PCM,PCO,PCU,PHE,PCE,QDC
   HWREGH(baseAddr+EQEP_QEINT) = 1<<11 | 1<<8 | 1<<6 | 1<<5| 1<<2|1<<1;
   // position compare: enable;  shadow disable;
   HWREGH(baseAddr+EQEP_QPOSCTL)  = 1<<12;
   QEPSetPos(baseAddr,0,QEP_SETPOS_IMMED);
   //enable Quadrature position counter 
   HWREGH(baseAddr+EQEP_QEPCTL)|= 1<<3;
   
}
       
void QEPVelocityDetectStart(unsigned int baseAddr,unsigned int timeResolution_us){
   unsigned index = addr2index(baseAddr);
   unitTime = timeResolution_us;
   unsigned long long tick = inFreq/1000000*timeResolution_us;
   mdAssert(tick<=0xffffffff);
   oldpos[index] = (HWREG(baseAddr+EQEP_QPOSCNT)-POSORIGIN);
   HWREG(baseAddr+EQEP_QUPRD) = (unsigned int)tick;
   //capture latch mode :Latch on unit time out ;eQEP unit timer enable
   HWREGH(baseAddr+EQEP_QEPCTL) |= 1<<2 | 1<<1;      
}

void QEPSetPos(unsigned int baseAddr,unsigned int pos,unsigned int setevent){
   mdAssert(positionFactor!=0);
   HWREG(baseAddr + EQEP_QPOSINIT ) = POSORIGIN+pos/positionFactor;
   unsigned short ctrl = HWREGH(baseAddr + EQEP_QEPCTL );
   if (setevent & QEP_SETPOS_IMMED ) {
      ctrl |= 1<<7;
   }
   unsigned short val =  (setevent >> 1)& 0x03; // INDEX
   if (val>1) {
      ctrl &= ~(0x03<<8);
      ctrl |= (val<<8);
   }
   val = (setevent >> 3)& 0x03; // STROBE
   if (val>1) {
      ctrl &= ~(0x03<<10);
      ctrl |= (val<<10);
   }
   HWREGH(baseAddr + EQEP_QEPCTL )  = ctrl;
}

unsigned int QEPReadPos(unsigned int baseAddr){
   return (HWREG(baseAddr + EQEP_QPOSCNT)-POSORIGIN)*positionFactor;
}

signed int QEPCalcuLatchVelocity(unsigned int baseAddr){
   unsigned index= addr2index(baseAddr);
   unsigned int newpos = (HWREG(baseAddr+EQEP_QPOSLAT)-POSORIGIN);
   signed long long  dis = ((signed long long)newpos-(signed long long)oldpos[index]) 
                      * (signed int)positionFactor;
   oldpos[index] = newpos;
   return dis*1000L*1000L/(signed int)unitTime;// pulse*factor/s
}

void QEPSetPosCompare(unsigned int baseAddr,unsigned int compare){
   mdAssert(positionFactor!=0);
   HWREG(baseAddr + EQEP_QPOSCMP) = compare / positionFactor + POSORIGIN;
}



void QEPSetPosCompareCurrent(unsigned int baseAddr){
   unsigned int val = QEPReadPos(baseAddr);
   QEPSetPos(baseAddr,val,QEP_SETPOS_IMMED);
}




