#ifndef _GUI_H
#define _GUI_H


#include "type.h"

#define LCD_XSize				800
#define LCD_YSize				480

#define ASC_Head 			32				//����ַ���ӦASC����׵�ַ
#define ASC_Num				16				//������ASC�����ƫ��λ��

#define FONT16				1				//����Ϊ16*16����	
#define FONT24				2				//����Ϊ24*24����	


typedef unsigned int COLOR;

#define C_Red				0xf800
#define C_Green				0x07e0
#define C_Blue				0x1f
#define C_Black             0x0
#define C_White             0xffff
#define C_TRANSPARENT       0xffffffff


#define ALIGEN_LEFT		0			//�������ʾ
#define ALIGEN_MIDDLE	1			//������ʾ
#define ALIGEN_RIGHT	2			//�Ҷ�����ʾ

//�����������ɫ���Ͷ���(�н���Ч��)--------
#define MAX_FormTitle		3				//�����������ʽ���Ϊ3
#define FORM_Red			0				//���������Ϊ��ɫ
#define FORM_Blue		1				//���������Ϊ��ɫ
#define FORM_Gray		2				//���������Ϊ��ɫ

//�������ı���ɫ����(�н���Ч��)------------------
#define BTN_Gray			0				
#define BTN_Yellow		1
	

#define FRAME_Hight		24				//�ı���ĸ߶�

#define TITLE_H16		24				//������Ϊ16��������ĸ߶�
#define TITLE_H24		30				//������Ϊ24��������ĸ߶�
#define TITLE_H35		32				//�������߶�Ϊ35������


#define PTEXT	(const char *)
#define GET_ScrCenterX(width)	((LCD_XSize-width)>>1)	//��ʾ����Ļ���ĵ�Xֵ
#define GET_ScrCenterY(height)	((LCD_YSize-height)>>1)	//��ʾ����Ļ���ĵ�Yֵ


//������Ϣ�ṹ����------------------------------
typedef  struct _S_Form{
	uint16 X;					//��ʼ��X����
	uint16 Y;					//��ʼ��Y����
	uint16 Width;				//������
	uint16 Height;			//����߶�
	char *TitleText;		//�������ı�	
	uint16 TitleFont;			//�������ı�����������
	uint16 TitleColor;			//��������ɫ����(�̶�Ϊ����)
	uint16 TitleHeight;		//�������߶�	
	uint16 TextColor;			//������������ɫ
	uint16 BackColor;			//����ɫ(�������ı���ɫ)
	uint16 FillBack;			//��Ϊ0��ʾ��䱳��ɫ
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

