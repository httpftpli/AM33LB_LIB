#ifndef __PF__ECAP__H__
#define __PF__ECAP__H__


extern void ECAPregisterHandler(unsigned int ecapindex,void (*ecaphandler)(unsigned int time));
extern void ECAPInit(unsigned int baseAddr);

#endif


