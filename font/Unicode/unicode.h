// unicode.h 
//

#if !defined(AFX_FONT_UNICODE_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_)
#define AFX_FONT_UNICODE_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

//#include "..\..\include\typedef.h"



int ReadFontSection();
void ReleaseSection();
BOOL getCharInfo_U(unsigned short wCode,unsigned int font ,FL_CHARINFO *charinfo);


#endif // !defined(AFX_FONT_UNICODE_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_)
