

#ifndef __MEM__h__
#define __MEM__h__

#include <wchar.h>


extern void memset16(void *s,unsigned short val,unsigned int n);
extern BOOL memis(void *mem,unsigned char dat,unsigned int memlen);
extern BOOL strendwith(const char *s,const char *endwith);
extern BOOL wcsendwith(const wchar_t *s ,const wchar_t *endwith);
#endif
   
