/***********************************************************
描述：	用c语言写的一个如何从点阵字库中读取字符信息（像素宽 +点阵信息）
        至于容错性和效率方面还得使用者自行改善,谢谢您的参阅！

文件格式： 
		Unicode -- 文件头+数据段(section)+索引表+点阵信息
		MBCS_Ladin-文件头+索引表+点阵信息
		MBCS_CJK-- 文件头+点阵信息
		

*************************************************************/

#include "font.h"
#include "unicode\unicode.h"
#include "ff.h"

FL_HEADER fl_header;
FL_SECTION_INF *pfl_section;
unsigned int fl_basemem_addr;
static unsigned int fontmemaddr = 0;

char loadFont(TCHAR * filepath, unsigned int memaddr) {
   unsigned int re, br;
   FIL fontfile;
   re = f_open(&fontfile, filepath, FA_OPEN_EXISTING | FA_READ);
   if (FR_OK != re) {
      return -1;
   }
   re = f_read(&fontfile, &fl_header, sizeof(FL_HEADER), &br);
   if (FR_OK != re) {
      f_close(&fontfile);
      return -1;
   }
   if (br != sizeof(FL_HEADER)) {
      f_close(&fontfile);
      return -1;
   }
   //检测标识头
   if ((fl_header.magic[0] != 'U' && fl_header.magic[0] != 'M')
       || fl_header.magic[1] != 'F' || fl_header.magic[2] != 'L') {
      f_close(&fontfile);
      return -2;
   }
   if (fl_header.Size != fontfile.fsize) {
      f_close(&fontfile);
      return -2;
   }
   f_lseek(&fontfile, 0);    /* Move file pointer to 0 */
   re = f_read(&fontfile, (void *)memaddr, fl_header.Size, &br);
   if (FR_OK != re) {
      f_close(&fontfile);
      return -1;
   }
   if (br != fl_header.Size) {
      f_close(&fontfile);
      return -1;
   }
   fl_basemem_addr = memaddr;
   if ('U' == fl_header.magic[0]) {     //unicode 编码
      pfl_section = (FL_SECTION_INF *)(memaddr+sizeof(fl_header));
   }
   fontmemaddr = memaddr;
   return 0;
}



unsigned char  GetFontYSize()
{
	return fl_header.YSize;
}



char getCharInfo(unsigned short wCode, FL_CHARINFO *pcharinfo) {
   int r = getCharInfo_U(wCode, pcharinfo);
   if (r != 0) return -1;
   pcharinfo->pixaddr += fontmemaddr;
   return 0;
}
