
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
//功能:			显示一个矩形
//参数:
//		x:			起始横坐标
//		y:			起始纵坐标
//		color_f:		显示的前景色
//		color_b:		显示的前景色
//说明:
//修改时间:		2013-3-15
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
//功能:			将整屏刷新成指定的颜色
//参数:
//说明:
//修改时间:		2013-3-15
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
//功能: 显示半角字符
//x         : 起始横坐标
//y         : 起始纵坐标
//color_f  : 显示的前景色
//color_b : 显示的前景色
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
//功能: 显示半角字符
//x         : 起始横坐标
//y         : 起始纵坐标
//color_f  : 显示的前景色
//color_b : 显示的前景色
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
//功能: 显示8*16点阵的ASC字符(ASC码从0开始)
//x         : 起始横坐标
//y         : 起始纵坐标
//color_f  : 显示的前景色
//color_b : 显示的前景色
//===================================================================
/*void Dis_Asc(uint32 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b) {
   if (num < 32) return;
   num -= 32;
   Dis_8x16(num, x, y, color_f, color_b);
}*/

//===================================================================
//画填充的矩形函数
//x:		起始横坐标
//y:		起始纵坐标
//width:	矩形的度度
//heigh:	矩形的高度
//color:	线的颜色
//===================================================================
/*void Dis_SuZi(uint8 num, uint16 x, uint16 y, uint16 font, uint16 color_f, uint16 color_b) {
#define ASC_Num		16		//数字在ASC表中的位置
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
//功能:			显示字符串
//参数:
//		hz:			字符串指针
//		x:			起始x坐标
//		y:			起始y坐标
//		font:			字体点阵大小
//		color_f:		字符串前景色
//		color_b:		字符串背景色
//修改时间:		2010-11-10
//===================================================================
void Dis_Strings(const char *hz, uint16 x, uint16 y, uint16 font, uint16 color_f, uint16 color_b) {
   uint8 dat;
   uint32 qu_ma, wei_ma;
   uint32 hz_addr;
   uint32 full_step;               //全角占用的象素宽度
   uint32 half_step;               //半角占用的象素宽度
   void (*call_full)(uint32 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b);
   void (*call_half)(uint8 num, uint16 x, uint16 y, uint16 color_f, uint16 color_b);
   //字体为24点阵处理----------
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
      if (dat == 0) break;       //到字串结束
                                 //是汉字处理----
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
      //ASC码显示处理----
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
//功能:			显示一个数值
//显示一个数值,左对齐,高位0自动消隐
//weisu:  数值的最大位数
//stt:     0为正常显示,1为反白显示
//修改时间:		2010-11-20
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


