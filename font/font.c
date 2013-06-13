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
#include "utf8.h"




FONT fonts[MAX_FONT_NFONTLIB];
  

static unsigned int is_font_loaded = 0;
FL_SECTION_INF *pfl_section;
unsigned int fl_basemem_addr;
static unsigned int fontmemaddr = 0;



/**
 * @brief 载入字体
 * @param [in] filepath 字体文件路径
 * @param [inout] font 
 *        字体结构体，必须先初始化font->memaddr成员
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
signed char loadFont(TCHAR * filepath, FONT  *font) {
   unsigned int re, br;
   FIL fontfile;
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
   //检测标识头
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
   
   if ('U' == font->fl_header.magic[0]) {     //unicode 编码
      unsigned int nsection = font->fl_header.nSection;
      for (int i=0;i<nsection;i++) {
         font->sectionInfoList[i] = (FL_SECTION_INF *)(font->memaddr+i*sizeof(FL_HEADER));
      }
   }
   return 0;
}

signed char  initFont(TCHAR * dirpath,unsigned int addr){
   TCHAR filepath [200];
   static const TCHAR *fontname0 = _TEXT("1_16.FONT");
   static const TCHAR *fontname1 = _TEXT("1_24.FONT");
   static const TCHAR *fontname2 = _TEXT("1_32.FONT");
   
   int i;
   for(i=0;dirpath[i]!=0;i++){
     filepath[i] = dirpath[i]; 
   }
   int j;
   for(j=0;fontname0[j]!=0;j++){
     filepath[i+j] = fontname0[j];
   }
   filepath[i+j] = 0;
   fonts[0].memaddr = addr;
   return loadFont(filepath,fonts);
   
   //return loadFont(filepath, fonts+1);
   //return loadFont(filepath, fonts+2);
   //return loadFont(filepath, fonts+3);
   
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
unsigned char  GetFontYSize(unsigned int font)
{
    return fonts[font].fl_header.YSize;
}


extern const uint8 Asc8x16[1536];

/**
 * @brief 获取字体信息
 * @param [in] wCode 字体编码,unicode方式 
 * @param [in] font 字体
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
BOOL getCharInfo(unsigned short wCode,unsigned int font ,FL_CHARINFO *pcharinfo) {
   BOOL r;
   if (is_font_loaded == 0) {
      goto FONT_IN_SOURCE;
   }
   r = getCharInfo_U(wCode, font,pcharinfo);
   if (FALSE == r) goto FONT_IN_SOURCE;
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
void getStringMetric(const TEXTCHAR *string ,unsigned int font,METRIC *metric){
   const TEXTCHAR *index = string;
   unsigned int width = 0,w = 0,w1 = 0;
   unsigned short ucs2;
   FL_CHARINFO info;
   while (1) {
       w = UTF8toUCS2(index+w1,&ucs2);
       w1+=w;
       if (0==w) {
          break;
       }
       getCharInfo(ucs2,font,&info);
       width += info.width;
   }
   metric->width = width;
   metric->height = info.height;
}

//! @}
