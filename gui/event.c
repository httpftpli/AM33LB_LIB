/**
 *  \file   event.c
 *
 *  \brief 
 *  \author  李飞亮  
 *  \addtogroup GUI
 *  @{ 
 *   
 */


#include "event.h"
#include "algorithm.h"
#include "mmath.h"

RINGBUF msgRing;
static MSG msg[100];

void initMessageRing(){
   ringBufInit(&msgRing,msg,sizeof(MSG),NARRAY(msg));
}


BOOL GetMessage (MSG *msg){
   return ringBufPop(&msgRing,msg);

}

BOOL SendMessage (MSG *msg){
   return ringBufPush(&msgRing,msg);
}

void clearMessageRing(){
   ringBufClear(&msgRing);
}


//! @}




