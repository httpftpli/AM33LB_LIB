
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


void drawString(const TEXTCHAR *text, unsigned int x, unsigned int y) {
   unsigned short ucs2;
#if defined(__IAR_SYSTEMS_ICC__)
   unsigned char charoffset = 0;
   unsigned char signelcharlen = 0;
   GUI_Context.DispPosX = x;
   while (1) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      signelcharlen = UTF8toUCS2(text + charoffset, &ucs2);
      charoffset += signelcharlen;
      if (0 == signelcharlen) {
         break;
      }
      DrawCharUcs2(ucs2, GUI_Context.DispPosX, y);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 2);
      if (*text == 0) {
         break;
      }
      DrawCharUcs2(*text++,GUI_Context.DispPosX, y);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      if (*text == 0) {
         break;
      }
      DrawCharUcs2(*text++,GUI_Context.DispPosX, y);
#endif
   }
#else
#error "Unsupported Compiler. \r\n"
#endif
}



void drawStringLen(const TEXTCHAR *text, unsigned int x, unsigned int y,unsigned int len) {
   unsigned short ucs2;
#if defined(__IAR_SYSTEMS_ICC__)
   unsigned char charoffset = 0;
   unsigned char signelcharlen = 0;
   GUI_Context.DispPosX = x;
   while (1) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      signelcharlen = UTF8toUCS2(text + charoffset, &ucs2);
      charoffset += signelcharlen;
      if ((0 == signelcharlen)||(0==len)) {
         break;
      }
      len--;
      DrawCharUcs2(ucs2, GUI_Context.DispPosX, y);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 2);      
      if ((*text == 0)||(0==len)) {
         break;
      }
      len--;
      DrawCharUcs2(*text++,GUI_Context.DispPosX, y);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
      STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
      if ((*text == 0)||(0==len)) {
         break;
      }
      len--;
      DrawCharUcs2(*text++,GUI_Context.DispPosX, y);
#endif
   }
#else
#error "Unsupported Compiler. \r\n"
#endif
}


void drawStringEx(const TEXTCHAR *text, unsigned int x, unsigned int y, const GUI_FONT *font, COLOR color_f,COLOR color_b){
   GUI_CONTEXT oldc;
   GUI_SaveContext(&oldc);
   GUI_Context.DispPosX = x;
   GUI_Context.DispPosY = y;
   GUI_Context.Color = color_f;
   GUI_Context.BkColor = color_b;
   drawString(text, x, y);
   GUI_RestoreContext(&oldc);
}

void drawStringAligen(const TEXTCHAR *text, uint32 aligen, uint16 x, uint16 y, uint16 width, uint16 height) {
   unsigned int widthtemp;
   unsigned short len[10];
   const  char * strptr[10];
   unsigned int strlen = strLen_UTF8(text);
   unsigned int strheight = getCurFontYSize();
   unsigned int line =  height / strheight;
   const char *ptr = text;
   unsigned int lineindex = 0;
   for (int i=0,j=0; i < strlen; i++,j++) {
      widthtemp = getStringMetricWidthEx(ptr, j+1);
      if(i==(strlen-1)) j++;
      if ((widthtemp > width)||(i==(strlen-1))) {
         strptr[lineindex] = ptr;
         len[lineindex] = j;
         ptr = strForward_UTF8(ptr, j);
         j = 0;
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

    
         


void drawStringAligenEx(const TEXTCHAR *text, uint32 aligen, uint16 x, uint16 y, uint16 width,uint16 height, const GUI_FONT *font, COLOR color_f, COLOR color_b) {
   GUI_CONTEXT old;
   GUI_SaveContext(&old);
   GUI_SetFont(font);
   GUI_SetBkColor(color_b);
   GUI_SetColor( color_f);
   GUI_Context.DispPosX = x;
   GUI_Context.DispPosY = y;
   drawStringAligen(text,aligen, x,y,width,height);
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
	 	







