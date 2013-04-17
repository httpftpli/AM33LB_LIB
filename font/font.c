/***********************************************************
������	��c����д��һ����δӵ����ֿ��ж�ȡ�ַ���Ϣ�����ؿ� +������Ϣ��
        �����ݴ��Ժ�Ч�ʷ��滹��ʹ�������и���,лл���Ĳ��ģ�

�ļ���ʽ�� 
		Unicode -- �ļ�ͷ+���ݶ�(section)+������+������Ϣ
		MBCS_Ladin-�ļ�ͷ+������+������Ϣ
		MBCS_CJK-- �ļ�ͷ+������Ϣ
		

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
   //����ʶͷ
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
   if ('U' == fl_header.magic[0]) {     //unicode ����
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
