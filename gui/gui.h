#ifndef _GUI_H
#define _GUI_H


#include "type.h"

#define LCD_XSize				800
#define LCD_YSize				480

#define ASC_Head 			32				//����ַ���ӦASC����׵�ַ
#define ASC_Num				16				//������ASC�����ƫ��λ��

#define FONT16				16				//����Ϊ16*16����	
#define FONT24				24				//����Ϊ24*24����	


typedef unsigned int COLOR;

#define C_Red				0xf800
#define C_Green				0x07e0
#define C_Blue				0x1f
#define C_Black             0x0
#define C_White             0xffff
#define C_TRANSPARENT       0xffffffff



extern void Dis_RectFill(uint32 x,uint32 y,uint32 width,uint32 height,COLOR color);
extern void LcdClear(COLOR color);
extern void LcdChangDisBuffer(void);
extern void Dis_SetPixel(unsigned int x,unsigned int y,COLOR color);
extern void Dis_DrawMask(void *buf, unsigned int x, unsigned int y, unsigned int width,
                  unsigned int height, COLOR color_f, COLOR color_b);
extern unsigned int  Dis_DrawChar_Ucs2(unsigned short character,unsigned int x,unsigned int y,COLOR color_f,COLOR color_b);
extern void Dis_DrawText(TEXTCHAR *text,unsigned int x,unsigned int y,COLOR color_f,COLOR color_b);

#endif

