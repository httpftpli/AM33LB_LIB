#ifndef _GUI_H
#define _GUI_H


#include "type.h"

#define LCD_XSize				800
#define LCD_YSize				480

#define ASC_Head 			32				//半角字符对应ASC表的首地址
#define ASC_Num				16				//数字在ASC点阵的偏移位置

#define FONT16				1				//字体为16*16点阵	
#define FONT24				2				//字体为24*24点阵	


typedef unsigned int COLOR;

#define C_Red				0xf800
#define C_Green				0x07e0
#define C_Blue				0x1f
#define C_Black             0x0
#define C_White             0xffff
#define C_TRANSPARENT       0xffffffff


#define ALIGEN_LEFT		0			//左对齐显示
#define ALIGEN_MIDDLE	1			//居中显示
#define ALIGEN_RIGHT	2			//右对齐显示

//窗体标题栏颜色类型定义(有渐变效果)--------
#define MAX_FormTitle		3				//窗体标题栏样式最大为3
#define FORM_Red			0				//窗体标题栏为红色
#define FORM_Blue		1				//窗体标题栏为蓝色
#define FORM_Gray		2				//窗体标题栏为灰色

//触摸键的背景色定义(有渐变效果)------------------
#define BTN_Gray			0				
#define BTN_Yellow		1
	

#define FRAME_Hight		24				//文本框的高度

#define TITLE_H16		24				//标题栏为16点阵字体的高度
#define TITLE_H24		30				//标题样为24点阵字体的高度
#define TITLE_H35		32				//标题栏高度为35个象素


#define PTEXT	(const char *)
#define GET_ScrCenterX(width)	((LCD_XSize-width)>>1)	//显示在屏幕中心的X值
#define GET_ScrCenterY(height)	((LCD_YSize-height)>>1)	//显示在屏幕中心的Y值


//窗体信息结构定义------------------------------
typedef  struct _S_Form{
	uint16 X;					//起始的X坐标
	uint16 Y;					//起始的Y坐标
	uint16 Width;				//窗体宽度
	uint16 Height;			//窗体高度
	char *TitleText;		//标题栏文本	
	uint16 TitleFont;			//标题栏文本的字体类型
	uint16 TitleColor;			//标题栏颜色类型(固定为三种)
	uint16 TitleHeight;		//标题栏高度	
	uint16 TextColor;			//标题栏文字颜色
	uint16 BackColor;			//背景色(窗体填充的背景色)
	uint16 FillBack;			//不为0表示填充背景色
}S_Form;



extern void Dis_RectFill(uint32 x,uint32 y,uint32 width,uint32 height,COLOR color);
extern void LcdClear(COLOR color);
extern void LcdChangDisBuffer(void);
extern void Dis_SetPixel(unsigned int x,unsigned int y,COLOR color);
extern void Dis_DrawMask(const void *buf, unsigned int x, unsigned int y, unsigned int width,
                  unsigned int height, COLOR color_f, COLOR color_b);
extern unsigned int  Dis_DrawChar_Ucs2(unsigned short character,unsigned int x,unsigned int y,unsigned int font,COLOR color_f,COLOR color_b);
extern unsigned int  Dis_DrawText(TEXTCHAR *text,unsigned int x,unsigned int y,COLOR color_f,COLOR color_b);



void Dis_HLine(uint16 x, uint16 y, uint16 lineLen, COLOR color) ;
void Dis_VLine(uint16 x, uint16 y, uint16 lineLen, COLOR color) ;
void Dis_Line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, COLOR color); 
void lcd_DrawLine(int ix0, int iy0, int ix1, int iy1, COLOR color); 
void Dis_Rect(uint16 x, uint16 y, uint16 width, uint16 heigh, COLOR color) ;
void Dis_RectSome(uint32 x, uint32 y, uint32 width, uint32 heigh, uint32 lineWidth, COLOR color) ;
void Dis_RectFill(uint32 x, uint32 y, uint32 width, uint32 height, COLOR color) ;
void Dis_CircleRectFill(int16 x, int16 y, uint32 width, uint32 height, uint32 r, COLOR color) ;
void Dis_CircleRectFillShade(int16 x, int16 y, uint32 width, uint32 height, uint32 r, COLOR color1, COLOR color2) ;
void Dis_Picture(uint32 x, uint32 y, uint32 width, uint32 height, uint16 *bmp); 
void Dis_Clear(COLOR color);
unsigned int  Dis_DrawChar_Ucs2(unsigned short character, unsigned int x, unsigned int y, unsigned int font, COLOR color_f, COLOR color_b) ;
void Dis_SetPixel(unsigned int x, unsigned int y, COLOR color) ;
unsigned int Dis_String(const TEXTCHAR *text, unsigned int x, unsigned int y, uint16 font, COLOR color_f, COLOR color_b) ;
void Dis_StringAligen(const TEXTCHAR *text, uint32 aligen, uint16 x, uint16 y, uint32 width, uint16 font, COLOR color_f, COLOR color_b);
void Dis_StringAligen1(const TEXTCHAR *text, uint32 aligen, uint16 x, uint16 y, uint32 width,uint32 height, uint16 font, COLOR color_f, COLOR color_b);
void Dis_SuZi(uint8 num, uint16 x, uint16 y, uint16 font, COLOR color_f, COLOR color_b);
void Dis_Num(uint32 dat, uint16 x, uint16 y, uint8 weisu, uint16 font, COLOR color_f, COLOR color_b) ;
void Dis_Asc(uint8 cha, uint16 x, uint16 y, unsigned int font, COLOR color_f, COLOR color_b);
void Dis_WindowsForm(S_Form *form); 
void Dis_BoxUp(uint16 x, uint16 y, uint16 width, uint16 height, COLOR color);
void Dis_BoxDown(uint16 x, uint16 y, uint16 width, uint16 height, COLOR color);
void Dis_FrameUp(uint16 x, uint16 y, uint16 width, uint16 height, COLOR color);
void Dis_FrameDown(uint16 x, uint16 y, uint16 width, uint16 height, COLOR color);



#endif

