#include "qep.h"
#include  "pf_qep.h"
#include "interrupt.h"
#include "hw_types.h"
#include "hw_eqep.h"
#include "soc_AM335x.h"

volatile signed int velocity[3];


static void (*qephandler[3])(unsigned int flag);



void QEPRegistHandler(unsigned int baseAddr,void (*handler)(unsigned int flag)){
   unsigned index = addr2index(baseAddr);
   qephandler[index] = handler;
}

signed int QEPGetVelocity(unsigned int baseAddr){
   unsigned index = addr2index(baseAddr);
   return velocity[index];
}


void isr_qep(unsigned intnum){
   unsigned int baseaddr = intnum2addr(intnum);
   unsigned int index = intnum2index(intnum);
   //read interrupt status
   unsigned short stat = HWREGH(baseaddr+EQEP_QFLG);
   if (stat & 1<<11) { //UTO  calculate velocity
      velocity[index] = QEPCalcuLatchVelocity(baseaddr);
   }
   if ((stat & 1<<11)&&(qephandler[index] !=NULL)) {// compare match event
      qephandler[index](QEP_HANDER_FlAG_COMPARE_MATCH);
   }
   if ((stat & 1<<2)&&(qephandler[index] !=NULL)) {//Quadrature phase error
      qephandler[index](QEP_HANDER_FlAG_PHASE_ERROR);
   }
   if ((stat & 1<<1)&&(qephandler[index] !=NULL)) {//Quadrature phase error
      qephandler[index](QEP_HANDER_FlAG_POSCNT_ERROR);
   }
   if ((stat & 1<<6)&&(qephandler[index] !=NULL)) {//Position counter overflow
      qephandler[index](QEP_HANDER_FlAG_POSCNT_OVERFLOW);
   }
   if ((stat & 1<<5)&&(qephandler[index] !=NULL)) {//Position counter underflow
      qephandler[index](QEP_HANDER_FlAG_POSCNT_UNDERFLOW);
   }
   HWREGH(baseaddr+EQEP_QCLR) |= stat;
}
