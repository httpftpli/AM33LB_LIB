#ifndef __misc__h___
#define __misc__h___


#include "type.h"

#define everydiffdo(TYPE,MARK,val)\
            static TYPE __old ## MARK = 0;\
            for (TYPE MARK = (val); MARK!=__old ## MARK;\
              __old ## MARK=MARK)

extern unsigned char bcd2hex_2(unsigned char byte);

extern unsigned short hex2bcd_byte(unsigned char byte);

extern unsigned short bcd2hex_4(unsigned short bcd);

extern void strToWstr(wchar *wstr,char *str);

extern void wtrToStr(char *str,wchar *wstr);



#endif

