
#include "type.h"

void  memset16(void *s,unsigned short val, size_t16 n){
   for (int i=0;i<n;i++) {
      ((unsigned short *)s)[i] = val;
   }
}
