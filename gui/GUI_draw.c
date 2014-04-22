
#include "type.h"
#include "pf_lcd.h"
#include <string.h>
#include "lib_gui.h"
#include "pf_lcd.h"
#include "utf8.h"
#include "mem.h"
#include "debug.h"
#include "pf_platform_cfg.h"

#ifndef CHARACTER_DIS_CODEC
#error "must define CHARACTER_DIS_CODEC"
#endif





extern  GUI_FONT GUI_Fontascii_16;

GUI_CONTEXT GUI_Context = {
  .ClipRect = {0,0,800,600},
  .BkColor = C_BLUE,
  .Color = C_WHITE,
  .pAFont = &GUI_Fontascii_16,
};

 
void drawHLine(uint16 x, uint16 y, uint16 lineLen) {
   memset16(&Pix(x, y), GUI_Context.Color, lineLen);
}


void drawHLineEx(uint16 x, uint16 y, uint16 lineLen, COLOR color) {
   memset16(&Pix(x, y), color, lineLen);
}


void drawVLine(uint16 x, uint16 y, uint16 lineLen) {
   for (uint32 i = 0; i < lineLen; i++) {
      LCD_SetPixel(x, y + i, GUI_Context.Color);
   }
}

void drawVLineEx(uint16 x, uint16 y, uint16 lineLen,COLOR color) {
   for (uint32 i = 0; i < lineLen; i++) {
      LCD_SetPixel(x, y + i,color);
   }
} 


void drawRect(uint16 x, uint16 y, uint16 width, uint16 heigh) {
   drawHLine(x, y, width);
   drawHLine(x, y + heigh - 1, width);
   drawVLine(x, y, heigh);
   drawVLine(x + width - 1, y, heigh);
}


void drawRectEx(uint16 x, uint16 y, uint16 width, uint16 heigh, COLOR color) {   
   drawHLineEx(x, y, width,color);
   drawHLineEx(x, y + heigh - 1, width,color);
   drawVLineEx(x, y, heigh,color);
   drawVLineEx(x + width - 1, y, heigh,color);
}


void drawRectFill(uint32 x, uint32 y, uint32 width, uint32 height) {
   for (unsigned int i = y; i <(y + height); i++) {
      drawHLine(x, i, width);
   }
}


void drawRectFillEx(uint32 x, uint32 y, uint32 width, uint32 height, COLOR color) {
   for (unsigned int i = y; i <(y + height); i++) {
      drawHLineEx(x, i, width, color);
   }
}


void  DrawCharUcs2(unsigned short character, unsigned int x, unsigned int y) {
   GUI_Context.DispPosX = x;
   GUI_Context.DispPosY = y;
   GUI_Context.pAFont->pfDispChar(character);
}

void  DrawCharUcs2Ex(unsigned short character, unsigned int x, unsigned int y, GUI_FONT *font, COLOR color_f, COLOR color_b) {
   GUI_CONTEXT oldcontex;
   GUI_SaveContext(&oldcontex);
   GUI_Context.Color = color_f;
   GUI_Context.BkColor = color_b;
   GUI_Context.DispPosX = x;
   GUI_Context.DispPosY = y;
   GUI_Context.pAFont = font;
   GUI_Context.pAFont->pfDispChar(character);
   GUI_RestoreContext(&oldcontex);
}

void drawString_ucs2(const wchar *wcs,unsigned int x,unsigned int y){
  GUI_Context.DispPosX = x;
  while (*wcs) {
     DrawCharUcs2(*wcs++,GUI_Context.DispPosX, y);
  }
}

void drawString_utf8(const char *utf8string,unsigned x ,unsigned int y){
  unsigned short ucs2;
  unsigned char charoffset = 0;
  unsigned char signelcharlen = 0;
  GUI_Context.DispPosX = x;
  while (1) {
      signelcharlen = UTF8toUCS2(utf8string + charoffset, &ucs2);
      charoffset += signelcharlen;
      if (0 == signelcharlen) {
         break;
      }
      DrawCharUcs2(ucs2, GUI_Context.DispPosX, y);
  }
}

void drawString_ascii(const char *string,unsigned x ,unsigned int y){
  GUI_Context.DispPosX = x;
  while (*string) {
     DrawCharUcs2(*string++,GUI_Context.DispPosX, y);
  }
}


void drawString(const TEXTCHAR *text, unsigned int x, unsigned int y) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      drawString_utf8(text,x,y);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 2);
      drawString_ucs2(text,x,y);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      drawString_ascii(text,x,y);
#endif
}

void drawStringLen_ascii(const char *string,unsigned int x,unsigned int y,unsigned int len){
  GUI_Context.DispPosX = x;
  while((*string)&&(len--)){
      DrawCharUcs2(*string++,GUI_Context.DispPosX, y);
  }
}
  
               
void drawStringLen_ucs2(const wchar *wcs,unsigned int x,unsigned int y,unsigned int len){
  GUI_Context.DispPosX = x;
  while((*wcs)&&(len--)){
      DrawCharUcs2(*wcs++,GUI_Context.DispPosX, y);
  }
}


void drawStringLen_utf8(const char *utf8string,unsigned int x,unsigned int y,unsigned int len){
   unsigned short ucs2;
   unsigned char charoffset = 0;
   unsigned char signelcharlen = 0;
   GUI_Context.DispPosX = x;
   while (1) {
      signelcharlen = UTF8toUCS2(utf8string + charoffset, &ucs2);
      charoffset += signelcharlen;
      if ((0 == signelcharlen)||(0==len)) {
         break;
      }
      len--;
      DrawCharUcs2(ucs2, GUI_Context.DispPosX, y);
   }
}

void drawStringLen(const TEXTCHAR *text, unsigned int x, unsigned int y,unsigned int len) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      drawStringLen_utf8(text,x,y,len);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 2);
      drawStringLen_ucs2(text,x,y,len);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      drawStringLen_ascii(text,x,y,len);
#endif
}

void drawStringEx_ucs2(const wchar *wcs, unsigned int x, unsigned int y, const GUI_FONT *font, COLOR color_f,COLOR color_b){
   GUI_CONTEXT oldc;
   GUI_SaveContext(&oldc);
   GUI_Context.DispPosX = x;
   GUI_Context.DispPosY = y;
   GUI_Context.Color = color_f;
   GUI_Context.BkColor = color_b;
   GUI_Context.pAFont = font;
   drawString_ucs2(wcs, x, y);
   GUI_RestoreContext(&oldc);
}

void drawStringEx(const TEXTCHAR *text, unsigned int x, unsigned int y, const GUI_FONT *font, COLOR color_f,COLOR color_b){
   GUI_CONTEXT oldc;
   GUI_SaveContext(&oldc);
   GUI_Context.DispPosX = x;
   GUI_Context.DispPosY = y;
   GUI_Context.Color = color_f;
   GUI_Context.BkColor = color_b;
   GUI_Context.pAFont = font;
   drawString(text, x, y);
   GUI_RestoreContext(&oldc);
}

void drawStringAlign_ucs2(const wchar *wcs,unsigned int align,unsigned short x,unsigned short y,unsigned short width,unsigned short height){
  unsigned int widthtemp;
   unsigned short len[10];
   const  wchar * strptr[10];
   unsigned int str_len = wcslen((wchar_t *)wcs);
   unsigned int strheight = getCurFontYSize();
   unsigned int line =  height / strheight;
   const wchar *ptr = wcs;
   unsigned int lineindex = 0;
   for (int i=0,j=0; i < str_len; i++,j++) {
      widthtemp = getStringMetricWidthEx_ucs2(ptr, j+1);
      if(i==(str_len-1)) j++;
      if ((widthtemp > width)||(i==(str_len-1))) {
         strptr[lineindex] = ptr;
         len[lineindex] = j;
         ptr++;
         j = 0;
         if (++lineindex == line) {
            break;
         }
      }
   } 
   unsigned xtemp,ytemp = y+(height-strheight*lineindex)/2;
   for (int i = 0; i < lineindex; i++) {
      unsigned int strwidth = getStringMetricWidthEx_ucs2(strptr[i],len[i]);
      if (ALIGN_LEFT == align) {
         xtemp = x;
      } else if (ALIGN_MIDDLE == align) {
         xtemp = x + width / 2 - strwidth / 2;
      } else {
         xtemp = x + width - strwidth;
      }
      drawStringLen_ucs2(strptr[i], xtemp,ytemp,len[i]);
      ytemp += strheight;
   }
}


void drawStringAlign(const TEXTCHAR *text, uint32 aligen, uint16 x, uint16 y, uint16 width, uint16 height) {
   unsigned int widthtemp;
   unsigned short len[10];
   const  char * strptr[10];
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
   unsigned int str_len = strLen_UTF8(text);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
   unsigned int str_len = strlen(text);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
   unsigned int str_len = wcslen(text);
#endif
   unsigned int strheight = getCurFontYSize();
   unsigned int line =  height / strheight;
   const TEXTCHAR *ptr = text;
   unsigned int lineindex = 0;
   unsigned int lineflag = 0;
   unsigned int itemp = 0;
   for (int i=0,j=1; i < str_len; i++,j++) {
      widthtemp = getStringMetricWidthEx(ptr, j);
      if(widthtemp > width){
         if(i-itemp <= 1) break;
         itemp = i ; 
         lineflag = 1;
         j--;i--;
      }else if(i==(str_len-1)){
         lineflag = 1;
      }
      if(1==lineflag){
         lineflag = 0;
         strptr[lineindex] = ptr;
         len[lineindex] = j;
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
         ptr = strForward_UTF8(ptr, j);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
         ptr+= j;
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
         STATIC_ASSERT(sizeof(*ptr)==2);
         ptr+= j;
#endif
         j = 1;
         if (++lineindex == line) {
            break;
         }
      }
   } 
   unsigned xtemp,ytemp = y+(height-strheight*lineindex)/2;
   for (int i = 0; i < lineindex; i++) {
      unsigned int strwidth = getStringMetricWidthEx(strptr[i],len[i]);
      if (ALIGN_LEFT == aligen) {
         xtemp = x;
      } else if (ALIGN_MIDDLE == aligen) {
         xtemp = x + width / 2 - strwidth / 2;
      } else {
         xtemp = x + width - strwidth;
      }
      drawStringLen(strptr[i], xtemp,ytemp,len[i]);
      ytemp += strheight;
   }
}

    
         
void drawStringAlignEx(const TEXTCHAR *text, uint32 align, uint16 x, uint16 y, uint16 width,uint16 height, const GUI_FONT *font, COLOR color_f, COLOR color_b) {
   GUI_CONTEXT old;
   GUI_SaveContext(&old);
   GUI_SetFont(font);
   GUI_SetBkColor(color_b);
   GUI_SetColor( color_f);
   GUI_Context.DispPosX = x;
   GUI_Context.DispPosY = y;
   GUI_Context.pAFont = font;
   drawStringAlign(text,align, x,y,width,height);
   GUI_RestoreContext(&old);
}

void drawStringAlignEx_ucs2(const wchar *wcs, uint32 align, uint16 x, uint16 y, uint16 width,uint16 height, const GUI_FONT *font, COLOR color_f, COLOR color_b) {
   GUI_CONTEXT old;
   GUI_SaveContext(&old);
   GUI_SetFont(font);
   GUI_SetBkColor(color_b);
   GUI_SetColor( color_f);
   GUI_Context.DispPosX = x;
   GUI_Context.DispPosY = y;
   GUI_Context.pAFont = font;
   drawStringAlign_ucs2(wcs,align, x,y,width,height);
   GUI_RestoreContext(&old);
}

/*********************************************************************
*
*       GUI_SetBkColor
*/
void GUI_SetBkColor(GUI_COLOR color) {
   GUI_Context.BkColor = color;
}


void GUI_SetColor( GUI_COLOR color) {
   GUI_Context.Color = color;
}


const GUI_FONT   * GUI_SetFont(const GUI_FONT *font){
   const GUI_FONT *old = GUI_Context.pAFont;
   GUI_Context.pAFont = font;
   return old;
}



void GUI_SaveContext(const GUI_CONTEXT* pContext) {
  memcpy((void *)pContext,&GUI_Context,sizeof GUI_Context);
}

/*********************************************************************
*
*       GUI_RestoreContext
*/
void GUI_RestoreContext(const GUI_CONTEXT* pContext) {
   memcpy(&GUI_Context,(void *)pContext,sizeof GUI_Context);
}

/*************************** End of file ****************************/
	 	







