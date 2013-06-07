#ifndef    __ATOMIC__H__
#define    __ATOMIC__H__

#include "cpu.h"

typedef volatile int atomic ;

static inline void atomicWrite(atomic *atom, int val){
    CPUirqd();
    *atom = val;
    CPUirqe();
}

static inline atomic atomicRead(atomic *atom){
   return *atom;
}

static inline void atomicSet(atomic *atom){
    atomicWrite(atom, 1);
}

static inline void atomicClear(atomic *atom){
    atomicWrite(atom, 0);
}

static inline void atomicAdd(atomic *atom,int val){
    CPUirqd();
    atom += val;
    CPUirqe();
}


static inline void atomicInc(atomic *atom){
   atomicAdd(atom,1);
}

static inline void atomicDec(atomic *atom){
   atomicAdd(atom,-1);
}

static inline unsigned int  atomicTest(atomic *atom){  
    return !! *atom ;
}


static inline unsigned int atomicTestClear(atomic *atom){
   if (*atom) {
      atomicWrite(atom, 0);
      return 1;
   }else
      return 0;
}


#endif
 
