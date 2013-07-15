
#include "type.h"
#include "pf_lcd.h"
#include <string.h>
#include "lib_gui.h"
#include "pf_lcd.h"
#include "font.h"
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
  .BkColor = C_Blue,
  .Color = C_White,
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
   drawHLine(x, y, width);
   drawHLine(x, y + heigh - 1, width);
   drawVLine(x, y, heigh);
   drawVLine(x + width - 1, y, heigh);
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
      signelcharlen = UTF8toUCS2(text + charoffset, &ucs2);
      charoffset += signelcharlen;
      if (0 == signelcharlen) {
         break;
      }
      DrawCharUcs2(ucs2, GUI_Context.DispPosX, y);
#else
      if (*text == 0) {
         break;
      }
      GUI_Context.DispPosX = x;
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
	 	







