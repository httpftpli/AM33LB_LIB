
#include "type.h"
#include <wchar.h>
#include "debug.h"

void  memset16(void *s,unsigned short val, size_t16 n){
    wmemset((wchar_t *) s, val, n);
}

BOOL strendwith(const char *s,const  char *endwith){
   ASSERT((s!=NULL)&&(endwith!=NULL));
   char *temp = (char *)endwith;
   while (*s) s++;
   while (*temp) temp++;
   do {
      if (*--temp !=  *--s)
          return FALSE;
   } while (temp != endwith);
   return TRUE;
}

BOOL wcsendwith(const wchar_t *s,const const wchar_t *endwith){
   ASSERT((s!=NULL)&&(endwith!=NULL));
   wchar_t *temp = (wchar_t *)endwith;
   while (*s) s++;
   while (*temp++) temp++;
   do {
      if (*--temp !=  *--endwith)
          return FALSE;
   }
   while (temp != endwith); 
   return TRUE;
}



