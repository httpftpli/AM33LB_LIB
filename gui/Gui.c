
//#include "Head.h"
#include "type.h"
#include "pf_lcd.h"
#include <string.h>
#include "gui.h"
#include "Asc8x16_dot.h"
#include "pf_lcd.h"
#include "font.h"
#include "utf8.h"



inline void LCD_SetPixel(unsigned int x,unsigned int y,COLOR color){
   if ((color & 0xffff0000)==0 ) {
      Pix((x),(y)) = color;
   }
}

static void  memset16(void *s,unsigned short val,unsigned int n){
   for (int i=0;i<n;i++) {
      ((unsigned short *)s)[i] = val;
   }
}





//===================================================================
//����:			��ʾһ������
//����:
//		x:			��ʼ������
//		y:			��ʼ������
//		color_f:		��ʾ��ǰ��ɫ
//		color_b:		��ʾ��ǰ��ɫ
//˵��:
//�޸�ʱ��:		2013-3-15
//===================================================================
void Dis_RectFill(uint32 x, uint32 y, uint32 width, uint32 height, COLOR color) {
   uint32 i;
   for (i = y; i < (y + height); i++) {
      memset16(&Pix(x,i), color,  width);
   }
}


void Dis_DrawMask(const void *buf, unsigned int x, unsigned int y, unsigned int width,
                  unsigned int height, COLOR color_f, COLOR color_b) {
   unsigned int nbyteperline = (width+7)/8;
   for (int i = 0; i < height; i++) {
      for (int j = 0; j < nbyteperline; j++) {
        for(int k=0;k<8;k++){
         unsigned char mask = *((unsigned char *)buf+ i*nbyteperline+j);
         COLOR color = (mask & (1<<(8-k)))?color_f:color_b;
         LCD_SetPixel(x+8*j+k,y+i,color);         
        }
      }
   }
}

//===================================================================
//����:			������ˢ�³�ָ������ɫ
//����:
//˵��:
//�޸�ʱ��:		2013-3-15
//===================================================================
void LcdClear(COLOR color) {
   Dis_RectFill(0,0,lcdCtrl.panel->width,lcdCtrl.panel->height,color);
}


unsigned int  Dis_DrawChar_Ucs2(unsigned short character,unsigned int x,unsigned int y,COLOR color_f,COLOR color_b){
   FL_CHARINFO info;
   if(getCharInfo(character,&info)==FALSE){
      getCharInfo((unsigned short)'?',&info);
   }
   unsigned int addr = info.pixaddr;
   unsigned int nbyteperline = (info.width+7)/8;
   for (int i = 0; i < info.height; i++) {
      for (int j = 0; j < nbyteperline; j++) {
        for(int k=0;k<8;k++){
         unsigned char mask = *((unsigned char *)addr+ i*nbyteperline+j);
         COLOR color = (mask & (1<<(8-k)))?color_f:color_b;
         LCD_SetPixel(x+8*j+k,y+i,color);         
        }
      }
   }
   return info.width;
}



void Dis_SetPixel(unsigned int x,unsigned int y,COLOR color){
   LCD_SetPixel(x,y,color);
}


unsigned int Dis_DrawText(TEXTCHAR *text,unsigned int x,unsigned int y,COLOR color_f,COLOR color_b){
   unsigned int xoffset = 0; 
   unsigned short ucs2;
#if defined(__IAR_SYSTEMS_ICC__)
   unsigned char charoffset = 0;
   unsigned char signelcharlen= 0;
   while (1) {
      signelcharlen = UTF8toUCS2(text+charoffset,&ucs2);
      charoffset += signelcharlen;
      if (0==signelcharlen) {
         break;
      }
      xoffset += Dis_DrawChar_Ucs2(ucs2,x+xoffset,y,color_f,color_b);
   }
   return x+xoffset;
#else
#error "Unsupported Compiler. \r\n"

#endif


}

//===================================================================
//����: ��ʾ����ַ�
//x         : ��ʼ������
//y         : ��ʼ������
//color_f  : ��ʾ��ǰ��ɫ
//color_b : ��ʾ��ǰ��ɫ
//===================================================================
/*void Dis_8x16(uint8 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b) {
   uint32 i, j;
   uint32 panel;
   uint32 addr;
   uint16 *p;
   num = LcdCtrl.ActiveBuffer;

   addr = num * 16;
   for (i = 0; i < 16; i++) {
      for (j = 0; j < 8; j++) {
         if (Asc8x16[addr + i] & (0x80 >> j)) {
            LcdBuffer[panel][LCD_XSize * (y + i) + x + j] = color_f;
         } else {
            LcdBuffer[panel][LCD_XSize * (y + i) + x + j] = color_b;
         }
      }
   }
}*/

/*
//===================================================================
//����: ��ʾ����ַ�
//x         : ��ʼ������
//y         : ��ʼ������
//color_f  : ��ʾ��ǰ��ɫ
//color_b : ��ʾ��ǰ��ɫ
//===================================================================
void Dis_12x24(uint8 num,uint16 x,uint16 y,uint16 color_f,uint16 color_b)
{
uint32 i,j;
uint32 addr;
    addr=num*2*24;
    for(i=0;i<24;i++)
    {
        for(j=0;j<8;j++)
        {
            if(Asc12x24[addr+i*2]&(0x80>>j))
            {
//				LCD_SetPixel(x+j,y+i,color_f);
                LcdBuffer[1][LCD_XSize*(y+i)+x+j]=color_f;
            }
            else
            {
//				LCD_SetPixel(x+j,y+i,color_b);
                LcdBuffer[1][LCD_XSize*(y+i)+x+j]=color_b;
            }
        }

        for(j=0;j<4;j++)
        {
            if(Asc12x24[addr+i*2+1]&(0x80>>j))
            {
//				LCD_SetPixel(x+j+8,y+i,color_f);
                LcdBuffer[1][LCD_XSize*(y+i)+x+j+8]=color_f;
            }
            else
            {
//				LCD_SetPixel(x+j+8,y+i,color_b);
                LcdBuffer[1][LCD_XSize*(y+i)+x+j+8]=color_b;
            }
        }		
    }
}
*/

//===================================================================
//����: ��ʾ8*16�����ASC�ַ�(ASC���0��ʼ)
//x         : ��ʼ������
//y         : ��ʼ������
//color_f  : ��ʾ��ǰ��ɫ
//color_b : ��ʾ��ǰ��ɫ
//===================================================================
/*void Dis_Asc(uint32 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b) {
   if (num < 32) return;
   num -= 32;
   Dis_8x16(num, x, y, color_f, color_b);
}*/

//===================================================================
//�����ľ��κ���
//x:		��ʼ������
//y:		��ʼ������
//width:	���εĶȶ�
//heigh:	���εĸ߶�
//color:	�ߵ���ɫ
//===================================================================
/*void Dis_SuZi(uint8 num, uint16 x, uint16 y, uint16 font, uint16 color_f, uint16 color_b) {
#define ASC_Num		16		//������ASC���е�λ��
   uint32 i, j;
   uint32 addr;
   if (font == FONT24) {
      addr = (num + ASC_Num) * 48;
      for (i = 0; i < 24; i++) {
         for (j = 0; j < 8; j++) {
            if (Asc12x24[addr + i * 2] & (0x80 >> j)) {
               LCD_SetPixel(x + j, y + i, color_f);
            } else {
               LCD_SetPixel(x + j, y + i, color_b);
            }
         }
         for (j = 0; j < 4; j++) {
            if (Asc12x24[addr + i * 2 + 1] & (0x80 >> j)) {
               LCD_SetPixel(x + j + 8, y + i, color_f);
            } else {
               LCD_SetPixel(x + j + 8, y + i, color_b);
            }
         }
      }
   } else {
      Dis_8x16(num + ASC_Num, x, y, color_f, color_b);
   }
}

//===================================================================
//����:			��ʾ�ַ���
//����:
//		hz:			�ַ���ָ��
//		x:			��ʼx����
//		y:			��ʼy����
//		font:			��������С
//		color_f:		�ַ���ǰ��ɫ
//		color_b:		�ַ�������ɫ
//�޸�ʱ��:		2010-11-10
//===================================================================
void Dis_Strings(const char *hz, uint16 x, uint16 y, uint16 font, uint16 color_f, uint16 color_b) {
   uint8 dat;
   uint32 qu_ma, wei_ma;
   uint32 hz_addr;
   uint32 full_step;               //ȫ��ռ�õ����ؿ��
   uint32 half_step;               //���ռ�õ����ؿ��
   void (*call_full)(uint32 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b);
   void (*call_half)(uint8 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b);
   //����Ϊ24������----------
   if (font == FONT24) {
//		call_full=Dis_24x24;
//		call_half=Dis_12x24;
      full_step = FONT24;
      half_step = FONT24 >> 1;
   } else {
//		call_full=Dis_16x16;
      call_half = Dis_8x16;
      full_step = FONT16;
      half_step = FONT16 >> 1;
   }

   while (1) {
      dat = *hz;
      if (dat == 0) break;       //���ִ�����
                                 //�Ǻ��ִ���----
      if (dat >= 0x80) {
         qu_ma = dat - 0xa0;
         hz++;
         wei_ma = *hz;
         if (wei_ma == 0) break;
         wei_ma -= 0xa0;
         hz_addr = (qu_ma - 1) * 94 + wei_ma - 1;
         call_full(hz_addr, x, y, color_f, color_b);
         x += full_step;
      }
      //ASC����ʾ����----
      else {
         if (dat < 32) return;
         dat -= ASC_Head;
         call_half(dat, x, y, color_f, color_b);
         x += half_step;
      }
      hz++;
   }
}

//===================================================================
//����:			��ʾһ����ֵ
//��ʾһ����ֵ,�����,��λ0�Զ�����
//weisu:  ��ֵ�����λ��
//stt:     0Ϊ������ʾ,1Ϊ������ʾ
//�޸�ʱ��:		2010-11-20
//===================================================================
void Dis_NumHide(uint16 dat, uint16 x, uint16 y, uint8 weisu, uint16 font, uint16 color_f, uint16 color_b) {
   uint8 i;
   uint8 youxiao;
   uint8 buf[5];
   uint8 num;
   uint32 width;
   if (font == FONT24) width = 12;
   else width = 8;
   num = 0;
   switch (weisu) {
   case 1:
      if (dat >= 10) i = 0;
      else i = (uint8)dat;
      Dis_SuZi(i, x, y, font, color_f, color_b);
      break;
   case 2:
      if (dat >= 100) dat = 0;
      buf[0] = dat / 10;
      buf[1] = dat % 10;
      for (i = 0; i < 2; i++) {
         if ((buf[i] > 0) || (i == 1)) {
            Dis_SuZi(buf[i], x, y, font, color_f, color_b);
            x += width;
            num++;
         }
      }
      if (num != 2) {
         Dis_Strings(" ", x, y, font, color_b, color_b);
      }
      break;
   case 3:
      youxiao = 0;
      if (dat >= 1000) dat = 0;
      buf[0] = dat / 100;
      dat %= 100;
      buf[1] = dat / 10;
      buf[2] = dat % 10;
      for (i = 0; i < 3; i++) {
         if ((buf[i] > 0) || (i == 2) || (youxiao > 0)) {
            Dis_SuZi(buf[i], x, y, font, color_f, color_b);
            x += width;
            youxiao = 1;
            num++;
         }
      }
      if (num != 3) {
         for (i = num; i < 3; i++) {
            Dis_Strings(" ", x, y, font, color_b, color_b);
            x += width;
         }
      }
      break;
   case 4:
      youxiao = 0;
      if (dat >= 10000) dat = 0;
      buf[0] = dat / 1000;
      dat %= 1000;
      buf[1] = dat / 100;
      dat %= 100;
      buf[2] = dat / 10;
      buf[3] = dat % 10;
      for (i = 0; i < 4; i++) {
         if ((buf[i] > 0) || (i == 3) || (youxiao > 0)) {
            Dis_SuZi(buf[i], x, y, font, color_f, color_b);
            x += width;
            youxiao = 1;
            num++;
         }
      }
      if (num != 4) {
         for (i = num; i < 4; i++) {
            Dis_Strings(" ", x, y, font, color_b, color_b);
            x += width;
         }
      }
      break;
   case 5:
      youxiao = 0;
      buf[0] = dat / 10000;
      dat %= 10000;
      buf[1] = dat / 1000;
      dat %= 1000;
      buf[2] = dat / 100;
      dat %= 100;
      buf[3] = dat / 10;
      buf[4] = dat % 10;
      for (i = 0; i < 5; i++) {
         if ((buf[i] > 0) || (i == 4) || (youxiao > 0)) {
            Dis_SuZi(buf[i], x, y, font, color_f, color_b);
            x += width;
            youxiao = 1;
            num++;
         }
      }
      if (num != 5) {
         for (i = num; i < 5; i++) {
            Dis_Strings(" ", x, y, font, color_b, color_b);
            x += width;
         }
      }
      break;
   default:
      break;
   }
}*/


