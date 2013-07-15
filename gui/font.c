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
#include "ff.h"
#include "type.h"
#include "pf_platform_cfg.h"
#include "utf8.h"
#include "mmath.h"



extern GUI_FONT GUI_Fontascii_16,GUI_Fontascii_20;
GUI_FONT font_16,font24;
   
GUI_FONT *fonts[4] = {
#if (ASCII_FONT16_IN_SOURCE==1)
   [0] = &GUI_Fontascii_16,
#endif
#if (ASCII_FONT20_IN_SOURCE==1)
   [1] =&GUI_Fontascii_20,
#endif
   [2] =&font_16,
   [3] =&font24,
};

FL_SECTION_INF *pfl_section;


static unsigned int loadFont(const TCHAR * filepath,  GUI_FONT  *font) {
   unsigned int re, br;
   FIL fontfile;
   re = f_open(&fontfile, filepath, FA_OPEN_EXISTING | FA_READ);
   if (FR_OK != re) {
      return -1;
   }
   unsigned int filesize = fontfile.fsize;
   re = f_read(&fontfile, (void *)font->p.pFontData,filesize, &br);
   if (FR_OK != re) {
      f_close(&fontfile);
      return -1;
   }
   if (br !=  filesize) {
      f_close(&fontfile);
      return -1;
   }
   GUI_SIF_CreateFont(font->p.pFontData,font, GUI_SIF_TYPE_PROP); 
   f_close(&fontfile);
   return filesize;
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
void  initFont(unsigned int addr) {
   const TCHAR *fontname[2];
   fontname[0] = _TEXT("0:/1_16.FNT");
   fontname[1] = _TEXT("0:/1_20.FNT");
   for(int i= 0;i<NARRAY(fontname);i++){
     fonts[2+i]->p.pFontData = (void *) addr;
     unsigned int filesize = loadFont(fontname[i], fonts[2+i]);
     if(filesize!=-1L){
         addr += filesize+8;
     }
   }
}

/**
 * @brief 获取字体高度 
 * @return 字体高度          
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  
 */
char  GetFontYSize() { 
   return GUI_Context.pAFont->YSize;
}

/**
 * @brief 获取字符串点阵信息
 * @param [in] string 
 *        字符串(utf8或者ascii编码,取决于\b
 *        CHARACTER_DIS_CODEC)
 * @return  字符串在屏幕显示的宽度         
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *    \b CHARACTER_DIS_CODEC
 */
int getStringMetricWidth(const TEXTCHAR *string){
   const TEXTCHAR *index = string;
   unsigned int widthtotle = 0,width, w = 0,w1 = 0;
   unsigned short ucs2;
   while (1) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
      w = UTF8toUCS2(index + w1, &ucs2);
      w1 += w;
      if (0 == w) {
         break;
      }
      width = GUI_Context.pAFont->pfGetCharDistX(w);
      if (0==width) {
         width = GUI_Context.pAFont->pfGetCharDistX('?');
      }
#elif (CHARACTER_DIS_CODEC==UCS16_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 2);
      if (*string == 0) {
         break;
      }
      width = GUI_Context.pAFont->pfGetCharDistX(w++);
      if (0 == width) {
         width = GUI_Context.pAFont->pfGetCharDistX('?');
      }
#endif
      widthtotle += width;
   }
   return widthtotle;
}
//! @}
