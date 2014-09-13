

#ifndef __MEM__h__
#define __MEM__h__

#include <wchar.h>



#define MEM_ALIGN_SIZE(size) (((size) + MEM_ALIGNMENT - 1) & ~(MEM_ALIGNMENT-1))



extern void memset16(void *s,unsigned short val,unsigned int n);
extern void memset32(void *s,unsigned int val, unsigned int n);
extern BOOL memis(void *mem,unsigned char dat,unsigned int memlen);
extern BOOL memis_32(void *mem,unsigned int dat,unsigned int memlen32);
extern BOOL strendwith(const char *s,const char *endwith);
extern BOOL wcsendwith(const wchar_t *s ,const wchar_t *endwith);
#endif
   
