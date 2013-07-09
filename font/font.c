/**
 *  \file   font.c
 *
 *  \brief
 *  \author  lfl 
 *  \addtogroup FONT
 *  @brief 字体
 *  @{
 *   
 */


#include "font.h"
#include "unicode\unicode.h"
#include "ff.h"
#include "type.h"
#include "pf_platform_cfg.h"
#include "utf8.h"




FONT fonts[MAX_FONT_NFONTLIB];
  

FL_SECTION_INF *pfl_section;
unsigned int fl_basemem_addr;
static unsigned int fontmemaddr = 0;



static signed char loadFont(TCHAR * filepath,  FONT  *font) {
   unsigned int re, br;
   FIL fontfile;
   font->available = 0;
   re = f_open(&fontfile, filepath, FA_OPEN_EXISTING | FA_READ);
   if (FR_OK != re) {
      return -1;
   }
   re = f_read(&fontfile, &(font->fl_header), sizeof(FL_HEADER), &br);
   if (FR_OK != re) {
      f_close(&fontfile);
      return -1;
   }
   if (br != sizeof(FL_HEADER)) {
      f_close(&fontfile);
      return -1;
   }
  
   if ((font->fl_header.magic[0] != 'U' && font->fl_header.magic[0] != 'M')
       || font->fl_header.magic[1] != 'F' || font->fl_header.magic[2] != 'L') {
      f_close(&fontfile);
      return -2;
   }
   if (font->fl_header.Size != fontfile.fsize) {
      f_close(&fontfile);
      return -2;
   }
   f_lseek(&fontfile, 0);    /* Move file pointer to 0 */
   re = f_read(&fontfile, (void *)(font->memaddr), font->fl_header.Size, &br);
   if (FR_OK != re) {
      f_close(&fontfile);
      return -1;
   }
   if (br != font->fl_header.Size) {
      f_close(&fontfile);
      return -1;
   }
   
   if ('U' == font->fl_header.magic[0]) {     //unicode 
      unsigned int nsection = font->fl_header.nSection;
      for (int i=0;i<nsection;i++) {
         font->sectionInfoList[i] = (FL_SECTION_INF *)(font->memaddr+i*sizeof(FL_HEADER));
      }
      font->available = 1;
   }
   return 0;
}


/**
 * @brief 初始化字体
 * @param [in] dirpath 
 *        字体文件路径(编码和文件系统的编码一致)
 * @param [in] addr  字体载入地址
 * @return 字体高度          
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  
 */
void  initFont(TCHAR *dirpath, unsigned int addr) {
   TCHAR filepath[200];
   unsigned int fontaddr = addr;
   const TCHAR *fontname[4];
   fontname[0] = _TEXT("1_16.FNT");
   fontname[1] = _TEXT("1_20.FNT");
   fontname[2] = _TEXT("1_24.FNT");
   fontname[3] = _TEXT("1_32.FNT");

   int i;
   for (i = 0; dirpath[i] != 0; i++) {
      filepath[i] = dirpath[i];
   }
   int j;
   for (int k = 0; k < 4; k++) {
      for (j = 0; fontname[k][j] != 0; j++) {
         filepath[i + j] = fontname[k][j];
      }
      filepath[i + j] = 0;
      fonts[k].memaddr = fontaddr;
      loadFont(filepath, &fonts[k]);
      if (fonts[k].available==TRUE) {
         fontaddr+=fonts[k].fl_header.Size;
      }
   }
}


/**
 * @brief 获取字体高度信息
 * @param [in] font 字体索引
 * @return 字体高度          
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  
 */
unsigned char  GetFontYSize(unsigned int font) { 
   if (1==fonts[font].available) {
      return fonts[font].fl_header.YSize;
   }
   if (font==0) {
      return 16;
   }else if(font==1)
      return 20;

   return 0;
}


extern const uint8 Asc8x16[1536];
extern const uint8 Asc10x20[];



/**
 * @brief 获取单个字符信息
 * @param [in] wCode 字符编码(unicode) 
 * @param [in] font 字体索引
 * @param [out] pcharinfo FL_CHARINFO 字体信息结构体
 * @return  BOOL         
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  
 */
BOOL getCharInfo(unsigned short wCode,unsigned int font ,FL_CHARINFO *pcharinfo) {
   BOOL r;
   if (fonts[font].available == 0) {
      goto FONT_IN_SOURCE;
   }
   r = getCharInfo_U(wCode, font,pcharinfo);
   if (FALSE == r) goto FONT_IN_SOURCE;
   pcharinfo->pixaddr += fontmemaddr;
   return TRUE;

FONT_IN_SOURCE:
   if ((unsigned char)wCode & 0x80) {
      return FALSE;
   }
   if (0 == font) {
      pcharinfo->height = 16;
      pcharinfo->width = 8;
      pcharinfo->pixaddr = 16 * (wCode - 32) + (unsigned int)Asc8x16;
   }else if(1==font){
      pcharinfo->height = 20;
      pcharinfo->width = 10;
      pcharinfo->pixaddr = 30 * (wCode - 32) + (unsigned int)Asc10x20;
   }else 
      return FALSE;
   return TRUE; 
}

/**
 * @brief 获取字符串点阵信息
 * @param [in] string 
 *        字符串(utf8或者ascii编码,取决于\b
 *        CHARACTER_DIS_CODEC)
 * @param [in] font 字体索引
 * @param [out] metric METRIC 结构体
 * @return  NONE         
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *    \b CHARACTER_DIS_CODEC
 */
void getStringMetric(const TEXTCHAR *string ,unsigned int font,METRIC *metric){
   const TEXTCHAR *index = string;
   unsigned int width = 0,w = 0,w1 = 0;
   unsigned short ucs2;
   FL_CHARINFO info;
   while (1) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
      w = UTF8toUCS2(index + w1, &ucs2);
      w1 += w;
      if (0 == w) {
         break;
      }
      if (FALSE == getCharInfo(ucs2, font, &info)) {
         getCharInfo('?', font, &info);
      }
#else
      if (*string == 0) {
         break;
      }
      if (FALSE == getCharInfo(*string++, font, &info)) {
         getCharInfo('?', font, &info);
      }
#endif;
      width += info.width;
   }
   metric->width = width;
   metric->height = info.height;
}

//! @}
