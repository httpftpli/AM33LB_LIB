/***********************************************************
描述：	用c语言写的一个如何从unicode编码格式的点阵字库中读取字符信息（像素宽 +点阵信息）
        至于容错性和效率方面还得使用者自行改善。谢谢您的参阅！
作者：  wujianguo
日期： 	20090516
MSN:    wujianguo19@hotmail.com
qq：    9599598
*************************************************************/

#include "..\font.h"
#include "unicode.h"


        

extern FL_HEADER fl_header;      
extern unsigned int g_dwCharInfo;   
extern unsigned int fl_basemem_addr;
extern FL_SECTION_INF *pfl_section;




// 获取字符的像素宽度和字符图像数据的地址
char getCharInfo_U(unsigned short wCode, FL_CHARINFO *charinfo) {
   int   i;
   for (i = 0; i < fl_header.nSection; i++) {
      if (wCode >= pfl_section[i].First && wCode <= pfl_section[i].Last) break;
   }
   if (i >= fl_header.nSection)	return -1;

   unsigned int offset = pfl_section[i].OffAddr + FONT_INDEX_TAB_SIZE * (wCode - pfl_section[i].First);
   unsigned int charindex = *(unsigned int *)(fl_basemem_addr + offset);
   charinfo->height = fl_header.YSize;
   charinfo->width = GET_FONT_WIDTH(charindex);
   if (0==charinfo->width) {
      return -1;
   }
   charinfo->pixaddr = GET_FONT_OFFADDR(charindex);
   return 0;
}





