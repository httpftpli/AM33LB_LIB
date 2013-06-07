
#include "..\font.h"
#include "unicode.h"


extern unsigned int fl_basemem_addr;
extern FONT fonts[MAX_FONT_NFONTLIB];





// 获取字符的像素宽度和字符图像数据的地址
BOOL getCharInfo_U(unsigned short wCode,unsigned int font ,FL_CHARINFO *charinfo) {
   int   i;
   unsigned int nsection = fonts[font].fl_header.nSection;
   for (i = 0; i < nsection; i++) {
      if (wCode >= fonts[font].sectionInfoList[i]->First && wCode <= fonts[font].sectionInfoList[i]->Last) break;
   }
   if (i >= nsection)	return FALSE;

   unsigned int offset = fonts[font].sectionInfoList[i]->OffAddr + FONT_INDEX_TAB_SIZE * (wCode - fonts[font].sectionInfoList[i]->First);
   unsigned int charindex = *(unsigned int *)(fl_basemem_addr + offset);
   charinfo->height = fonts[font].fl_header.YSize;
   charinfo->width = GET_FONT_WIDTH(charindex);
   if (0==charinfo->width) {
      return FALSE;
   }
   charinfo->pixaddr = GET_FONT_OFFADDR(charindex);
   return TRUE;
}





