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

#include "ff.h"
#include "type.h"
#include "pf_platform_cfg.h"
#include "utf8.h"
#include "mmath.h"
#include "lib_gui.h"



extern GUI_FONT GUI_Fontascii_16,GUI_Fontascii_20;
GUI_FONT font_16,font_20;

GUI_FONT *fonts[4] = {
#if (ASCII_FONT16_IN_SOURCE==1)
   [0] = &GUI_Fontascii_16,
   [2] = &GUI_Fontascii_16,
#endif
#if (ASCII_FONT20_IN_SOURCE==1)
   [1] =&GUI_Fontascii_20,
   [3] =&GUI_Fontascii_20,
#endif
};


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
   if(!GUI_SIF_CreateFont(font->p.pFontData,font, GUI_SIF_TYPE_PROP)){
       return -1; 
   }
   f_close(&fontfile);
   return filesize;
}


/**
 * @brief 初始化字体
 * @param [in] addr  存放字库的内存基地址 
 * @return 
 *         位编码类型，从低到高分别表示fonts的一种字体，如果位为1表示相应字体加载成功
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  
 */

unsigned int g_fontFlag = 0x03; 


unsigned int  initFont(unsigned int addr) {
   unsigned int ret = 0x03;
   const TCHAR *fontname[2];
   fontname[0] = _TEXT("0:/1_16.FNT");
   fontname[1] = _TEXT("0:/1_20.FNT");
   font_16.p.pFontData = (void *) addr;
   unsigned int filesize = loadFont(fontname[0], &font_16);
   if(filesize!=-1L){
       addr += filesize+8; 
       fonts[2] = &font_16;
       ret |= 0x04;
   }
   font_20.p.pFontData = (void *) addr;
   filesize = loadFont(fontname[1], &font_20);
   if(filesize!=-1L){
       addr += filesize+8;
       fonts[3] = &font_20; 
       ret |= 0x08;
   }
   g_fontFlag = ret; 
   return ret;
}

/**
 * @brief 获取当前字体高度 
 * @return 字体高度          
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  
 */
char  getCurFontYSize() { 
   return GUI_Context.pAFont->YSize;
}

/**
 * @brief 获取字体高度 
 * @param  font 字体结构体指针
 * @return 字体高度          
 * @date    2013/5/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 *  
 */
char getFontYSize(GUI_FONT *font){
   return font->YSize;
}


int getStringMetricWidthEx_ucs2(const wchar *wcs,unsigned int len){
   unsigned int width,widthtotle = 0;
   while (1) {
      if ((*wcs == 0)||(len==0)) {
         break;
      }
      len--;
      width = GUI_Context.pAFont->pfGetCharDistX(*wcs++);
      if (0 == width) {
         width = GUI_Context.pAFont->pfGetCharDistX('?');
      }      
      widthtotle += width;
   }
   return widthtotle;
}

int getStringMetricWidthEx(const TEXTCHAR *string,unsigned int len){
   const TEXTCHAR *index = string;
   unsigned int widthtotle = 0,width, w = 0,w1 = 0;
   unsigned short ucs2;
   while (1) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
      w = UTF8toUCS2(index + w1, &ucs2);
      w1 += w;
      if ((0 == w)||(len==0)) {
         break;
      }
      len--;
      width = GUI_Context.pAFont->pfGetCharDistX(ucs2);
      if (0==width) {
         width = GUI_Context.pAFont->pfGetCharDistX('?');
      }
#elif (CHARACTER_DIS_CODEC==UCS16_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 2);
      if ((*string == 0)||(len==0)) {
         break;
      }
      len--;
      width = GUI_Context.pAFont->pfGetCharDistX(*string++);
      if (0 == width) {
         width = GUI_Context.pAFont->pfGetCharDistX('?');
      }
#elif (CHARACTER_DIS_CODEC==ASCII_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      if ((*string == 0)||(len==0))  {
         break;
      }
      len--;
      width = GUI_Context.pAFont->pfGetCharDistX(*string++);
      if (0 == width) {
         width = GUI_Context.pAFont->pfGetCharDistX('?');
      }
#endif
      
      widthtotle += width;
   }
   return widthtotle;
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
   unsigned int str_len;
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
   str_len = strLen_UTF8(string);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
   str_len = strlen(string);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
   str_len = wcslen(string);
#endif
   return getStringMetricWidthEx(string,str_len);
}

//! @}
//! 


