/**
 *  \file   font.c
 *
 *  \brief
 *  \author  李飞亮  
 *  \addtogroup FONT
 *  @brief 字体显示
 *  @{
 *   
 */


#include "font.h"
#include "unicode\unicode.h"
#include "ff.h"
#include "type.h"
#include "pf_platform_cfg.h"

FL_HEADER fl_header;
static unsigned int is_font_loaded = 0;
FL_SECTION_INF *pfl_section;
unsigned int fl_basemem_addr;
static unsigned int fontmemaddr = 0;



/**
 * @brief 载入字体
 * @param [in] filepath 字体文件路径
 * @param [in] memaddr  字体载入地址
 * @return 
 * - -1 字体文件打开或者读取失败 \n 
 * - -2 字体文件格式不正确 \n
 * - 0 字体载入成功 \n
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 * \b FONT_IN_C_SOURCE 
 */
signed char loadFont(TCHAR * filepath, unsigned int memaddr) {
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
	is_font_loaded = 1;
   return 0;
}



/**
 * @brief 获取字体高度
 * @param [in] 
 * @return 字体高度          
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  \b FONT_IN_C_SOURCE
 */
unsigned char  GetFontYSize()
{
	return fl_header.YSize;
}


extern const uint8 Asc8x16[1536];

/**
 * @brief 获取字体信息
 * @param [in] wCode 字体编码,unicode方式
 * @param [out] pcharinfo FL_CHARINFO 结构体
 * @return  BOOL         
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *   \b FONT_IN_C_SOURCE
 */
BOOL getCharInfo(unsigned short wCode, FL_CHARINFO *pcharinfo) {
   if (is_font_loaded == 0) {
      goto FONT_IN_SOURCE;
   }
   BOOL r = getCharInfo_U(wCode, pcharinfo);
   if (r == FALSE) goto FONT_IN_SOURCE;
   pcharinfo->pixaddr += fontmemaddr;
   return TRUE; 

FONT_IN_SOURCE:
   if (wCode > 0xff) {
      return FALSE;
   }
   pcharinfo->height = 16;
   pcharinfo->width = 8;
   pcharinfo->pixaddr = 16 * (wCode - 32) + (unsigned int)Asc8x16;
   return TRUE;

}


//! @}
