
//#include "Head.h"
#include "type.h"
#include "pf_lcd.h"
#include <string.h>
#include "gui.h"
#include "Asc8x16_dot.h"
#include "pf_lcd.h"
#include "font.h"
#include "utf8.h"
#include "mem.h"
#include "debug.h"
#include "pf_platform_cfg.h"

#ifndef CHARACTER_DIS_CODEC
#error "must define CHARACTER_DIS_CODEC"
#endif

//窗体标题栏渐变色------------
static const uint16 FormTitleColor[3][24]={
0xd000,0xfdf7,0xfd14,0xfb8e,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,0xf800,	//红色
  0xf800,0xf800,0xf800,0xf800,0xf924,0xfa8a,0xfb4d,0xfbef,0xfcb2,0xfd34,0xd800,0x7800,
0x0014,0x73bf,0x18df,0x001e,0x001b,0x0016,0x0016,0x0016,0x0016,0x0017,0x0017,0x0017,	//蓝色
  0x0017,0x0017,0x0017,0x0017,0x0019,0x001b,0x001d,0x003f,0x10bf,0x10bf,0x0016,0x0010,
0x4a49,0xd69a,0xa534,0x8430,0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x7bcf,	//灰色
  0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x7bcf,0x8410,0x8c71,0x9cf3,0x8410,0x31a6,0x18e3,  
};




/**
 * @brief 画水平线
 * @param [in] x 始横坐标
 * @param [in] y 起始纵坐标 
 * @param [in] lineLen  线的长度
 * @param [in] color 线的颜色
 * @return NONE 
 */
void Dis_HLine(uint16 x, uint16 y, uint16 lineLen, COLOR color) {
   memset16(&Pix(x, y), color, lineLen);
}


/**
 * @brief 画垂直线
 * @param [in] x 始横坐标
 * @param [in] y 起始纵坐标 
 * @param [in] lineLen  线的长度
 * @param [in] color 线的颜色
 * @return NONE 
 */
void Dis_VLine(uint16 x, uint16 y, uint16 lineLen, COLOR color) {
   for (uint32 i = 0; i < lineLen; i++) {
      LCD_SetPixel(x, y + i, color);
   }
}

/*********************************************************************************************************
** Function name :        lcd_DrawVLine
** Function Description : Draw a vertical line
** Input:                 ix       x side coordinate of the line
**                        iy0      y side coordinate of the first pixel
**                        iy1      y side coordinate of the last pixel
**                        color  the color of the pixel 
** Output:                none
*********************************************************************************************************/
static void lcd_DrawVLine(int ix, int iy0, int iy1, COLOR color) {
   int i;
   for (i = iy0; i < iy1; i++) {
      LCD_SetPixel(ix, i, color);
   }
}

/*********************************************************************************************************
** Function name :        lcd_DrawHLine
** Function Description : Draw a horizontal line
** Input:                 ix0      x side coordinate of the first pixel
**                        ix1      x side coordinate of the last pixel
**                        iy       y side coordinate of the line
**                        color  the color of the pixel 
** Output:                none
*********************************************************************************************************/
static void lcd_DrawHLine(int ix0, int ix1, int iy, COLOR color) {
   int i;
   for (i = ix0; i < ix1; i++) {
      LCD_SetPixel(i, iy, color);
   }
}

/**
 * @brief 线画
 * @param [in] x0  x side coordinate of the first pixel
 * @param [in] y0  y side coordinate of the first pixel         
 * @param [in] x1  x side coordinate of the last pixel
 * @param [in] Y1  y side coordinate of the last pixel 
 * @param [in] color the color of the pixel 
 * @return   NONE 
 */
void Dis_Line(uint16 x0, uint16 y0, uint16 x1, uint16 y1, COLOR color) {
   signed char dx_sym;                                                 /*  increase size of x          */
   signed char dy_sym;                                                 /*  increase size of y          */
   int  dx;                                                            /*  line of x's offset          */
   int  dy;                                                            /*  line of y's offset          */
   int  dx_x2;
   int  dy_x2;
   int  di;
   dx = x1 - x0;                                                     /*  two pixel's offset          */
   dy = y1 - y0;
   if (dx > 0) {
      dx_sym = 1;                                                     /*  decide the increase size    */
   } else {
      if (dx < 0) {
         dx_sym = -1;
      } else {
         if (dy >= 0) {
            lcd_DrawVLine(x0, y0, y1, color);                  /*  dx==0,draw a vertical line  */
         } else {
            lcd_DrawVLine(x0, y1, y0, color);
         }
         return;
      }
   }

   if (dy > 0) {
      dy_sym = 1;
   } else {
      if (dy < 0) {
         dy_sym = -1;
      } else {
         if (dx >= 0) {
            lcd_DrawHLine(x0, x1, y0, color);                  /*  dy==0,Draw a horizontal line*/
         } else {
            lcd_DrawHLine(x1, x0, y0, color);
         }
         return;
      }
   }

   dx = dx_sym * dx;                                                /*  get absolute value          */
   dy = dy_sym * dy;

   dx_x2 = dx * 2;                                                     /*  double dx                   */
   dy_x2 = dy * 2;                                                     /*  double dy                   */

   /*
    *  dx >= dy, use x as benchmark
    */
   if (dx >= dy) {
      di = dy_x2 - dx;

      while (x0 != x1) {
         LCD_SetPixel(x0, y0, color);
         x0 += dx_sym;

         if (di < 0) {
            di  += dy_x2;
         } else {
            di  += dy_x2 - dx_x2;
            y0 += dy_sym;
         }
      }

      LCD_SetPixel(x0, y0, color);                                  /*  display the last pixel      */

   } else {
      /*
       *  dx < dy, use y as benchmark
       */
      di = dx_x2 - dy;

      while (y0 != y1) {
         LCD_SetPixel(x0, y0, color);
         y0 += dy_sym;

         if (di < 0) {
            di  += dx_x2;
         } else {
            di  += dx_x2 - dy_x2;
            x0 += dx_sym;
         }
      }

      LCD_SetPixel(x0, y0, color);                                  /*  display the last pixel      */
   }
}



/**
 * @brief 画划
 * @param [in] x0  x side coordinate of the first pixel
 * @param [in] y0  y side coordinate of the first pixel         
 * @param [in] x1  x side coordinate of the last pixel
 * @param [in] Y1  y side coordinate of the last pixel 
 * @param [in] color the color of the pixel 
 * @return   NONE        

 */


/**
 * @brief  画线
 * @param [in] ix0 
 * @param [in] iy0           
 * @param [in] ix1 
 * @param [in] iy1 
 * @param [in] color 
 * @return none
 */
void lcd_DrawLine(int ix0, int iy0, int ix1, int iy1, COLOR color) {
   signed char dx_sym;                                                 /*  increase size of x          */
   signed char dy_sym;                                                 /*  increase size of y          */
   int  dx;                                                            /*  line of x's offset          */
   int  dy;                                                            /*  line of y's offset          */
   int  dx_x2;
   int  dy_x2;
   int  di;

   dx = ix1 - ix0;                                                     /*  two pixel's offset          */
   dy = iy1 - iy0;

   if (dx > 0) {
      dx_sym = 1;                                                     /*  decide the increase size    */
   } else {
      if (dx < 0) {
         dx_sym = -1;
      } else {
         if (dy >= 0) {
            lcd_DrawVLine(ix0, iy0, iy1, color);                  /*  dx==0,draw a vertical line  */
         } else {
            lcd_DrawVLine(ix0, iy1, iy0, color);
         }
         return;
      }
   }

   if (dy > 0) {
      dy_sym = 1;
   } else {
      if (dy < 0) {
         dy_sym = -1;
      } else {
         if (dx >= 0) {
            lcd_DrawHLine(ix0, ix1, iy0, color);                  /*  dy==0,Draw a horizontal line*/
         } else {
            lcd_DrawHLine(ix1, ix0, iy0, color);
         }
         return;
      }
   }

   dx    = dx_sym * dx;                                                /*  get absolute value          */
   dy    = dy_sym * dy;

   dx_x2 = dx * 2;                                                     /*  double dx                   */
   dy_x2 = dy * 2;                                                     /*  double dy                   */

   /*
    *  dx >= dy, use x as benchmark
    */
   if (dx >= dy) {
      di = dy_x2 - dx;

      while (ix0 != ix1) {
         LCD_SetPixel(ix0, iy0, color);
         ix0 += dx_sym;

         if (di < 0) {
            di  += dy_x2;
         } else {
            di  += dy_x2 - dx_x2;
            iy0 += dy_sym;
         }
      }

      LCD_SetPixel(ix0, iy0, color);                                /*  display the last pixel      */

   } else {
      /*
       *  dx < dy, use y as benchmark
       */
      di = dx_x2 - dy;

      while (iy0 != iy1) {
         LCD_SetPixel(ix0, iy0, color);
         iy0 += dy_sym;

         if (di < 0) {
            di  += dx_x2;
         } else {
            di  += dx_x2 - dy_x2;
            ix0 += dx_sym;
         }
      }

      LCD_SetPixel(ix0, iy0, color);                                /*  display the last pixel      */
   }
}





/**
 * @brief  画矩形外框
 * @param [in] x 
 * @param [in] y
 * @param [in] width  
 * @param [in] heigh 
 * @param [in] color 
 * @return    none 
 */
void Dis_Rect(uint16 x, uint16 y, uint16 width, uint16 heigh, COLOR color) {
   Dis_HLine(x, y, width, color);
   Dis_HLine(x, y + heigh - 1, width, color);
   Dis_VLine(x, y, heigh, color);
   Dis_VLine(x + width - 1, y, heigh, color);
}


/**
 * @brief 画矩形框函数,线宽可设定
 * @param [in] x 起始横坐标 
 * @param [in] y 起始纵坐标         
 * @param [in] width 矩形框的宽度
 * @param [in] heigh 矩形框的高度
 * @param [in] lineWidth 线的宽度
 * @param [in] color 颜色

 */
void Dis_RectSome(uint32 x, uint32 y, uint32 width, uint32 heigh, uint32 lineWidth, COLOR color) {
   uint32 i;
   for (i = 0; i < lineWidth; i++) {
      Dis_HLine(x + i, y + i, width - i * 2, color);             //上面的横线
      Dis_HLine(x + i, y + heigh - 1 - i, width - i * 2, color);     //下面的横线
      Dis_VLine(x + i, y + i, heigh - i * 2, color);             //左边的竖线
      Dis_VLine(x + i + width - 1 - i * 2, y + i, heigh - i * 2, color); //右边的竖线
   }
}

/**
 * @brief 显示一个矩形
 * @param [in] x
 * @param [in] y           
 * @param [in] width 
 * @param [in] height 
 * @param [in] color 
 * @return none
 */
void Dis_RectFill(uint32 x, uint32 y, uint32 width, uint32 height, COLOR color) {
   uint32 i;
   for (i = y; i <(y + height); i++) {
      Dis_HLine(x, i, width, color);
   }
}

//=============================================================
//功能:			画实心圆角矩形
//参数:
//		x :			圆角矩形的起始x坐标
//		y :			圆角矩形的起始y坐标
//		width:		圆角矩形的宽度
//		height:		圆角矩形的高度
//		r :			圆角的半径
//		color:		颜色
//返回:
//说明:
//修改时间:		2012-10-17
//=============================================================


/**
 * @brief 画实心圆角矩形
 * @param [in] x 圆角矩形的起始x坐标
 * @param [in] y 圆角矩形的起始y坐标           
 * @param [in] width  圆角矩形的宽度 
 * @param [in] height 圆角矩形的高度 
 * @param [in] r 圆角的半径 
 * @param [in] color 颜色  
 * @return  none 
 */
void Dis_CircleRectFill(int16 x, int16 y, uint32 width, uint32 height, uint32 r, COLOR color) {
   int a, b;
   int di;
   uint32 qua2_y0, qua3_y0;
   uint16 qua3_x0, qua4_x0;
   x = x + r;
   y = y + r;
   width = width - 2 * r - 1;
   height = height - 2 * r - 1;
   a = 0;
   b = r;
   di = 3 - 2 * r;
   while (a <= b) {
      //第一象限
      LCD_SetPixel(x - a, y - b, color);
      LCD_SetPixel(x - b, y - a, color);             //3

      //第二象限
      qua2_y0 = y + height;
      LCD_SetPixel(x - a, qua2_y0 + b, color);
      LCD_SetPixel(x - b, qua2_y0 + a, color);

      //第三象限
      qua3_y0 = y + height;
      qua3_x0 = x + width;
      LCD_SetPixel(qua3_x0 + a, qua3_y0 + b, color);
      LCD_SetPixel(qua3_x0 + b, qua3_y0 + a, color);             //4

      //第四象限
      qua4_x0 = x + width;
      LCD_SetPixel(qua4_x0 + a, y - b, color);
      LCD_SetPixel(qua4_x0 + b, y - a, color);

      Dis_HLine(x - a, y - b, (qua4_x0 + a) - (x - a), color);
      Dis_HLine(x - b, y - a, (qua4_x0 + b) - (x - b), color);

      Dis_HLine(x - a, qua2_y0 + b, (qua3_x0 + a) - (x - a), color);
      Dis_HLine(x - b, qua2_y0 + a, (qua4_x0 + b) - (x - b), color);

      a++;
      if (di < 0) {
         di += 4 * a + 6;
      } else {
         di += 10 + 4 * (a - b);
         b--;
      }
   }

   Dis_VLine(x - r, y, height, color);
   Dis_VLine(x + r + width, y, height, color);
   Dis_HLine(x, y - r, width, color);
   Dis_HLine(x, y + height + r, width, color);
   Dis_RectFill(x - r + 1, y, width + 2 * r - 1, height, color);
}



/**
 * @brief  画有实心圆角矩形
 * @param [in] x 			圆角矩形的起始x坐标
 * @param [in] y 			圆角矩形的起始y坐标           
 * @param [in] width		圆角矩形的宽度
 * @param [in] height   圆角矩形的高度
 * @param [in] r 			圆角的半径
 * @param [in] color1		颜色 1 
 * @param [in] color2		颜色 2 
 * @return  none 
 */
void Dis_CircleRectFillShade(int16 x, int16 y, uint32 width, uint32 height, uint32 r, COLOR color1, COLOR color2) {
   int a, b;
   int di;
   uint32 qua2_y0, qua3_y0;
   uint16 qua3_x0, qua4_x0;
   x = x + r;
   y = y + r;
   width = width - 2 * r - 1;
   height = height - 2 * r - 1;
   a = 0;
   b = r;
   di = 3 - 2 * r;
   while (a <= b) {
      //第一象限
      LCD_SetPixel(x - a, y - b, color1);
      LCD_SetPixel(x - b, y - a, color1);             //3

      //第二象限
      qua2_y0 = y + height;
      LCD_SetPixel(x - a, qua2_y0 + b, color1);
      LCD_SetPixel(x - b, qua2_y0 + a, color1);

      //第三象限
      qua3_y0 = y + height;
      qua3_x0 = x + width;
      LCD_SetPixel(qua3_x0 + a, qua3_y0 + b, color2);
      LCD_SetPixel(qua3_x0 + b, qua3_y0 + a, color2);             //4

      //第四象限
      qua4_x0 = x + width;
      LCD_SetPixel(qua4_x0 + a, y - b, color2);
      LCD_SetPixel(qua4_x0 + b, y - a, color2);

      Dis_HLine(x - a, y - b, (qua4_x0 + a) - (x - a), color1);
      Dis_HLine(x - b, y - a, (qua4_x0 + b) - (x - b), color1);

      Dis_HLine(x - a, qua2_y0 + b, (qua3_x0 + a) - (x - a), color2);
      Dis_HLine(x - b, qua2_y0 + a, (qua4_x0 + b) - (x - b), color2);

      a++;
      if (di < 0) {
         di += 4 * a + 6;
      } else {
         di += 10 + 4 * (a - b);
         b--;
      }
   }

   Dis_VLine(x - r, y, height, color1);
   Dis_VLine(x + r + width, y, height, color2);
   Dis_HLine(x, y - r, width, color1);
   Dis_HLine(x, y + height + r, width, color2);
//	Dis_RectFill(x-r+1,y,width+2*r-1,height,color1);

   Dis_RectFill(x - r + 1, y, width + 2 * r - 1, height / 2, color1);
   Dis_RectFill(x - r + 1, y + height / 2, width + 2 * r - 1, height / 2 + 1, color2);
}



/**
 * @brief 显示一幅图片
 * @param [in] x      
 * @param [in] y           
 * @param [in] width  图片的宽度
 * @param [in] height 图片的高度
 * @param [in] bmp 图片数据指针
 * @return 
 */
void Dis_Picture(uint32 x, uint32 y, uint32 width, uint32 height, uint16 *bmp) {
   uint32 i;
   for (i = y; i < height + y; i++) {
      memcpy(&Pix(x, i), (uint8 *)bmp, width * 2);
      bmp += width;
   }
}


/**
 * @brief 将整屏刷新成指定的颜色
 * @param [in] color color 
 * @return none
 */
void Dis_Clear(COLOR color) {
   Dis_RectFill(0, 0, lcdCtrl.panel->width, lcdCtrl.panel->height, color);
}




/**
 * @brief 显示一个unicode(UCS2)编码的字符
 * @param [in] character
 * @param [in] x           
 * @param [in] y 
 * @param [in] font 字体句柄
 * @param [in] color_f
 * @param [in] color_b 
 * @return   none 
 */
unsigned int  Dis_DrawChar_Ucs2(unsigned short character, unsigned int x, unsigned int y, unsigned int font, COLOR color_f, COLOR color_b) {
   FL_CHARINFO info;
   if (getCharInfo(character, font, &info) == FALSE) {
      getCharInfo((unsigned short)'?', font, &info);
   }
   unsigned int addr = info.pixaddr;
   unsigned int nbyteperline = (info.width + 7) / 8;
   for (int i = 0; i < info.height; i++) {
      for (int j = 0; j < nbyteperline; j++) {
         for (int k = 0; k < 8; k++) {
            unsigned char mask = *((unsigned char *)addr + i * nbyteperline + j);
            COLOR color = (mask & (1 << (8 - k))) ? color_f : color_b;
            LCD_SetPixel(x + 8 * j + k, y + i, color);
         }
      }
   }
   return info.width;
}



void Dis_SetPixel(unsigned int x, unsigned int y, COLOR color) {
   LCD_SetPixel(x, y, color);
}



void Dis_DrawMask(const void *buf, unsigned int x, unsigned int y, unsigned int width,
                  unsigned int height, COLOR color_f, COLOR color_b) {
   unsigned int nbyteperline = (width + 7) / 8;
   for (int i = 0; i < height; i++) {
      for (int j = 0; j < nbyteperline; j++) {
         for (int k = 0; k < 8; k++) {
            unsigned char mask = *((unsigned char *)buf + i * nbyteperline + j);
            COLOR color = (mask & (1 << (8 - k))) ? color_f : color_b;
            LCD_SetPixel(x + 8 * j + k, y + i, color);
         }
      }
   }
}



/**
 * @brief 显示字符串
 * @param [text] 
 * @param [in] x           
 * @param [in] y 
 * @param [in] font 字体句柄
 * @param [in] color_f
 * @param [in] color_b 
 * @return   最后一个字符显示结束的坐标       

 */
unsigned int Dis_String(const TEXTCHAR *text, unsigned int x, unsigned int y, uint16 font, COLOR color_f, COLOR color_b) {
   unsigned int xoffset = 0;
   unsigned short ucs2;
#if defined(__IAR_SYSTEMS_ICC__)
   unsigned char charoffset = 0;
   unsigned char signelcharlen = 0;
   while (1) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
      signelcharlen = UTF8toUCS2(text + charoffset, &ucs2);
      charoffset += signelcharlen;
      if (0 == signelcharlen) {
         break;
      }
      xoffset += Dis_DrawChar_Ucs2(ucs2, x + xoffset, y, font, color_f, color_b);
#else
      if (*text == 0) {
         break;
      }
      xoffset += Dis_DrawChar_Ucs2(*text++, x + xoffset, y, font, color_f, color_b);
#endif
   }
   return x + xoffset;
#else
#error "Unsupported Compiler. \r\n"
#endif

}



/**
 * @brief 显示字符串,可设置对齐方式
 * @param [in] text
 * @param [in]  aligen 
 * - ALIGEN_LEFT--左对齐显示
 * - ALIGEN_MIDDLE--居中显示
 * - ALIGEN_RIGHT--右对齐显示 
 * @param [in] x
 * @param [in] y
 * @param [in] width
 * @param [in] font 
 * @param [in] color_f 
 * @param [in] color_b 
 * @return none
 */
/*void Dis_StringAligen(const TEXTCHAR *text, uint32 aligen, uint16 x, uint16 y, uint32 width, uint16 font, COLOR color_f, COLOR color_b) {
   uint32 xtemp;
   unsigned int stringwidth = getStringMetric(text, font);
   if (ALIGEN_LEFT == aligen) {
      xtemp = x;
   } else if (ALIGEN_MIDDLE == aligen) {
      xtemp = x - stringwidth / 2;
   } else {
      xtemp = x - stringwidth;
   }
   Dis_String(text, xtemp, y, font, color_f, color_b);
}*/


/**
 * @brief 
 *        在方框内显示字符串，横向可以设置对齐方式，纵向居中显示
 * @param [in] text
 * @param [in]  aligen 
 * - ALIGEN_LEFT--左对齐显示
 * - ALIGEN_MIDDLE--居中显示
 * - ALIGEN_RIGHT--右对齐显示 
 * @param [in] x
 * @param [in] y
 * @param [in] width 
 * @param [in] height 
 * @param [in] font 
 * @param [in] color_f 
 * @param [in] color_b 
 * @return none
 */
void Dis_StringAligen1(const TEXTCHAR *text, uint32 aligen, uint16 x, uint16 y, uint32 width,uint32 height, uint16 font, COLOR color_f, COLOR color_b) {
   uint32 xtemp,ytemp;
   METRIC metric;
   getStringMetric(text, font,&metric);
   if (ALIGEN_LEFT == aligen) {
      xtemp = x;
   } else if (ALIGEN_MIDDLE == aligen) {
      xtemp = x+width/2 - metric.width / 2;
   } else {
      xtemp = x+width - metric.width;
   } 
   ytemp = y+height/2- metric.height/2;
   Dis_String(text, xtemp,ytemp, font, color_f, color_b);
}


//===================================================================
//画填充的矩形函数
//x:		起始横坐标
//y:		起始纵坐标
//width:	矩形的度度
//heigh:	矩形的高度
//color:	线的颜色
//===================================================================

/**
 * @brief 显示数字
 * @param [in] num
 * @param [in] x           
 * @param [in] y 
 * @param [in] font 
 * @param [in] color_f 前景色
 * @param [in] color_b 背景色
 * @return none
 */
void Dis_SuZi(uint8 num, uint16 x, uint16 y, uint16 font, COLOR color_f, COLOR color_b) {
   mdAssert(num < 10);
   Dis_DrawChar_Ucs2(num + '0', x, y, font, color_f, color_b);
}

//===================================================================
//功能:			显示一个数值
//显示一个数值,左对齐,高位0自动消隐
//weisu:  数值的最大位数
//stt:     0为正常显示,1为反白显示
//修改时间:		2010-11-20
//===================================================================

/**
 * @brief 显示一个数值,左对齐,高位0自动消隐
 * @param [in] dat
 * @param [in] x           
 * @param [in] y
 * @param [in] weisu
 * @param [in] font 
 * @param [in] color_f 
 * @param [in] color_b  
 * @return  none 
 */
void Dis_Num(uint32 dat, uint16 x, uint16 y, uint8 weisu, uint16 font, COLOR color_f, COLOR color_b) {
   uint8 i;
   uint8 youxiao;
   uint8 buf[5];
   uint8 num;
   uint32 width;
   if (font == FONT24) width = FONT24 >> 1;
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
         Dis_String(" ", x, y, font, color_b, color_b);
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
            Dis_String(" ", x, y, font, color_b, color_b);
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
            Dis_String(" ", x, y, font, color_b, color_b);
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
            Dis_String(" ", x, y, font, color_b, color_b);
            x += width;
         }
      }
      break;
   default:
      break;
   }
}


/**
 * @brief 显示ASCII字符
 * @param [in] cha
 * @param [in] x           
 * @param [in] y 
 * @param [in] font
 * @param [in] color_f 前景色
 * @param [in] color_b 背景色
 * @return none
 */
void Dis_Asc(uint8 cha, uint16 x, uint16 y, unsigned int font, COLOR color_f, COLOR color_b) {
   Dis_DrawChar_Ucs2(cha, x, y, font, color_f, color_b);
}



//===================================================================
//功能: 显示一个带标题栏的Windows窗体(标题栏颜色有激变效果)
//             标题栏的文字颜色为白色
//x         : 起始横坐标
//y         : 起始纵坐标
//width   : 窗体的宽度
//heigh   : 窗体的高度
//ref_back: 不为0表示要刷新窗体内的背景色
//head_color: 标题栏的背景色样式0~2
//说明: 顶部标题栏高度24 (右侧显示X表示按标题栏关闭)
//		   标题栏文字居中对齐
//修改时间:		2010-11-10
//===================================================================

/**
 * @brief 显示一个带标题栏的Windows窗体(标题栏颜色有渐变效果)
 * @param [in] form S_Form结构体 
 */
void Dis_WindowsForm(S_Form *form) {
   uint32 TOP_H;           //顶部标题栏的高度
   uint16 x, y, w, h;
   uint16 color;
   uint32 i, len;
   uint32 asc_dot;
   x = form->X;
   y = form->Y;
   w = form->Width;
   h = form->Height;
   if (form->TitleColor > MAX_FormTitle) form->TitleColor = FORM_Blue;
   if (form->TitleFont != FONT24) form->TitleFont = FONT16;
   if ((form->TitleHeight < TITLE_H16) || (form->TitleHeight > (TITLE_H24 * 2))) {
      form->TitleHeight = TITLE_H16;
      form->TitleFont = FONT16;
   }

   TOP_H = form->TitleHeight;
   //标题栏背景色-----
   for (i = 0; i < TOP_H; i++) {
      //标题栏高度大于24时的背景色处理------
      if (TOP_H > 24) {
         if (i < 12) {
            color = FormTitleColor[form->TitleColor][i];
         } else if ((TOP_H - i) >= 12) {
            color = FormTitleColor[form->TitleColor][12];
         } else {
            color = FormTitleColor[form->TitleColor][24 - (TOP_H - i)];
         }
      } else {
         color = FormTitleColor[form->TitleColor][i];
      }
      Dis_HLine(x + 1, y + i, w - 2, color);
   }

   //左边两条竖线
   Dis_VLine(x, y, h, FormTitleColor[form->TitleColor][23]);
   Dis_VLine(x + 1, y + 1, h - 2, FormTitleColor[form->TitleColor][12]);
   //右边两条竖线
   Dis_VLine(x + w - 2, y + 1, h - 1, FormTitleColor[form->TitleColor][23]);
   Dis_VLine(x + w - 1, y, h, FormTitleColor[form->TitleColor][23]);
   //底部两条横线(黑色)
   Dis_HLine(x + 2, y + h - 2, w - 4, FormTitleColor[form->TitleColor][12]);
   Dis_HLine(x, y + h - 1, w, FormTitleColor[form->TitleColor][23]);

   len = 0;
   asc_dot = (form->TitleFont >> 1);
   for (i = 0; i < 100; i++) {
      if (*(form->TitleText + i) != 0) len += asc_dot;
      else break;
   }
   i = w / 2 - len / 2;
   len = x + i;
   Dis_String(form->TitleText,len, y + (TOP_H - form->TitleFont) / 2, form->TitleFont, C_White,C_TRANSPARENT);
   //填充窗体内的背景色-----
   if (form->FillBack > 0) {
      Dis_RectFill(x + 2, y + TOP_H, w - 4, h - TOP_H - 2, form->BackColor);
   }
}


/**
 * @brief 显示一个上凸的框体(刷新框体内的颜色)
 * @param [in] x
 * @param [in] y           
 * @param [in] width 
 * @param [in] height 
 * @param [in] color 框体的底色 
 * @see 
 */
void Dis_BoxUp(uint16 x, uint16 y, uint16 width, uint16 height, COLOR color) {
   //底色
   Dis_RectFill(x, y, width, height, color);
   //顶部两条横线(白色)
   Dis_HLine(x, y, width, C_White);
   Dis_HLine(x, y + 1, width - 1, C_White);
   //底部两条横线(黑色)
   Dis_HLine(x + 2, y + height - 2, width - 2, C_Black);
   Dis_HLine(x + 1, y + height - 1, width - 1, C_Black);
   //左边两条竖线(白色)
   Dis_VLine(x, y, height, C_White);
   Dis_VLine(x + 1, y, height - 1, C_White);
   //右边两条竖线(黑色)
   Dis_VLine(x + width - 2, y + 2, height - 2, C_Black);
   Dis_VLine(x + width - 1, y + 1, height - 1, C_Black);
}



/**
 * @brief 显示一个下凹的框体(刷新框体内的颜色)
 * @param [in] x
 * @param [in] y           
 * @param [in] width 
 * @param [in] height
 * @param [in] color
 * @return   none 
 */
void Dis_BoxDown(uint16 x, uint16 y, uint16 width, uint16 height, COLOR color) {
   //底色
   Dis_RectFill(x, y, width, height, color);
   //顶部两条横线(黑色)
   Dis_HLine(x, y, width, C_Black);
   Dis_HLine(x, y + 1, width - 1, C_Black);
   //底部两条横线(白色)
   Dis_HLine(x + 2, y + height - 2, width - 2, C_White);
   Dis_HLine(x + 1, y + height - 1, width - 1, C_White);
   //左边两条竖线(黑色)
   Dis_VLine(x, y, height, C_Black);
   Dis_VLine(x + 1, y, height - 1, C_Black);
   //右边两条竖线(白色)
   Dis_VLine(x + width - 2, y + 2, height - 2, C_White);
   Dis_VLine(x + width - 1, y + 1, height - 1, C_White);
}



/**
 * @brief 显示一个上凸的框体(不刷新框体内的颜色)
 * @param [in] x
 * @param [in] y           
 * @param [in] width 
 * @param [in] height
 * @param [in] color 深色线的颜色
 * @return  none 
 * @see 
 */
void Dis_FrameUp(uint16 x, uint16 y, uint16 width, uint16 height, COLOR color) {
   //顶部两条横线(白色)
   Dis_HLine(x, y, width, C_White);
   Dis_HLine(x, y + 1, width - 1, C_White);
   //底部两条横线(深色)
   Dis_HLine(x + 2, y + height - 2, width - 2, color);
   Dis_HLine(x + 1, y + height - 1, width - 1, color);
   //左边两条竖线(白色)
   Dis_VLine(x, y, height, C_White);
   Dis_VLine(x + 1, y, height - 1, C_White);
   //右边两条竖线(深色)
   Dis_VLine(x + width - 2, y + 2, height - 2, color);
   Dis_VLine(x + width - 1, y + 1, height - 1, color);
}

//===================================================================
//功能: 显示一个下凹的框体(不刷新框体内的颜色)
//color    : 深色线的颜色
//===================================================================

/**
 * @brief 显示一个下凹的框体(不刷新框体内的颜色)
 * @param [in] X
 * @param [in] Y           
 * @param [in] width
 * @param [in] height 
 * @param [in]  color 深色线的颜色 
 * @return none
 */
void Dis_FrameDown(uint16 x, uint16 y, uint16 width, uint16 height, COLOR color) {
   //顶部两条横线(深色)
   Dis_HLine(x, y, width, color);
   Dis_HLine(x, y + 1, width - 1, color);
   //底部两条横线(白色)
   Dis_HLine(x + 2, y + height - 2, width - 2, C_White);
   Dis_HLine(x + 1, y + height - 1, width - 1, C_White);
   //左边两条竖线(深色)
   Dis_VLine(x, y, height, color);
   Dis_VLine(x + 1, y, height - 1, color);
   //右边两条竖线(白色)
   Dis_VLine(x + width - 2, y + 2, height - 2, C_White);
   Dis_VLine(x + width - 1, y + 1, height - 1, C_White);
}


#if 0

/**
 * @brief 显示一个Windows类型的提示框(按任意键后退出)
 * @param [in] 标题栏上的文本信息
 * @param [in] 提示信息文本的内容(居中显示)          
 * @param [in] 提示文本的颜色
 * @param [in] 窗体的背景色
 * @param [in]
 * @param [in]
 * @return           
 * @date    2013/6/4
 * @note 
 * 自动装载和恢复调用前的显示信息
 *  窗体自动居中显示 ,根据提示文本的长度自动调整宽度
 *  显示字体固定为24点阵
 */
void Dis_HintForm(TEXTCHAR *title, const char *text, COLOR color_f, COLOR color_b) {
   uint16 x, y;
   uint32 i;
   uint32 len;
   uint32 tchnum;                          //备份的总触摸键数
   S_TchKey tch_backup;                        //备份的触摸键信息
   S_Form form;
   form.Height = 70;
   form.TitleHeight = TITLE_H24;
   form.TitleFont = FONT24;
   form.Y = GET_ScrCenterY(form.Height);
   form.TextColor = C_White;
   form.TitleText = PTEXT(title);
   form.TitleColor = FORM_Red;
   form.BackColor = color_b;
   form.FillBack = 1;
   //计算字符串的长度(象素值)------
   len = 0;
   for (i = 0; i < 100; i++) {
      if (*(text + i) != 0) len += FONT24 >> 1;
      else break;
   }
   //根据字符长度确定窗体宽度----
   form.Width = len + 60;      //两边各空30个象素点
   form.X = GET_ScrCenterX(form.Width);
   LcdMemPush(form.X, form.Y, form.Width, form.Height);

   Dis_WindowsForm(&form);
   x = (form.Width - len) >> 1;
   x = form.X + x;
   y = form.Y + form.TitleHeight + ((form.Height - form.TitleHeight - FONT24) >> 1);
   Dis_Strings(text, x, y, FONT24, color_f, color_b);   //显示提示文本
                                                        //保存第1个触摸键信息-----
   tch_backup = TchKey[0][0];
   tchnum = TchVal.KeyNum[0];
   Touch_KeyEn(form.X, form.Y, form.Width, form.Height, KEY_Tch, 0);
   TchVal.KeyNum[0] = 1;
   WaitAnyKey();
   //恢复之前的第1个触摸键信息----
   TchKey[0][0] = tch_backup;
   TchVal.KeyNum[0] = tchnum;
   LcdMemPop();
}

//===================================================================
//功能: 			显示一个Windows类型的确认框
//入口参数:
//		title:		标题栏上的文字信息
//		text:		提示信息文本的内容
//返回: 			0xffffffff表示取消(DO_Cancel),其它表示确定
//说明: 			退出时自动恢复被窗体覆盖的显示信息
//					字体为24点阵
//修改时间:		2010-11-29
//===================================================================

/**
 * @brief 显示一个Windows类型的确认框
 * @param [in]  title
 * @param [in]  text          
 * @param [in]  back_color 
 * @return  按键信息  \b KEY_OK ; \b KEY_ESC 
 */
uint32 Dis_SureDialog(TEXTCHAR *title, TEXTCHAR *text, COLOR back_color) {
   uint32 i;
   uint32 x, y;
   uint32 ret;
   uint32 len;
   uint32 btnw;
   S_Form form;
   Touch_KeyPush();
   TchVal.KeyNum[0] = 2;
   //计算提示文本的长度(以半角算)-----
   len = 0;
   for (i = 0; i < 100; i++) {
      if (*(text + i) != 0) len += FONT24 >> 1;
      else break;
   }
   form.Width = len + 30;
   if (form.Width < 150) form.Width = 150;
   btnw = (form.Width - 30 * 2) >> 1;
   form.Height = 120;
   form.X = GET_ScrCenterX(form.Width);
   form.Y = GET_ScrCenterY(form.Height);
   form.TitleText = title;
   form.TitleColor = FORM_Red;
   form.TitleFont = FONT24;
   form.TitleHeight = TITLE_H24;
   form.BackColor = back_color;
   form.FillBack = 1;
   LcdMemPush(form.X, form.Y, form.Width, form.Height);
   Dis_WindowsForm(&form);

   x = (form.Width - len) >> 1;
   x += form.X;
   y = form.Y + form.TitleHeight + 10;
   Dis_Strings(text, x, y, FONT24, C_Black, form.BackColor);

   //显示触摸选择框-------------------------------
   x = form.X + 15;
   y = form.Y + form.Height - 50;
   Dis_TouchButton(HZ_ItemSiFo[0][Lang], x, y, btnw, 35, KEY_ESC, 0, C_Black, FONT24);

   x = form.X + form.Width - 15 - btnw;
   Dis_TouchButton(HZ_ItemSiFo[1][Lang], x, y, btnw, 35, KEY_OK, 1, C_Black, FONT24);

   Key.Value = KEY_No;

   while (1) {
      Read_Key();

      Dis_TouchButtonFlash();

      if (Key.Value == KEY_OK) {
         ret = Key.Value;
         break;
      } else if (Key.Value == KEY_ESC) {
         ret = DO_Cancel;
         break;
      }
   }

   Touch_KeyPop();
   LcdMemPop();
   Key.Value = KEY_No;
   return (ret);
}


/**
 * @brief 显示一个比列条
 * @param [in] dat 当前的数值(从1开始)
 * @param [in] tol  总数值         
 * @param [in]  x  起始的x坐标
 * @param [in]  y 起始的y坐标
 * @param [in]  height 比列条的高度
 * @param [in]  width 比列条的宽度
 * @param [in] color_f  滑动块的颜色
 * @param [in] color_b  信息框的背景色
 * @return  none 
 * @note 
 * 列条占用显示宽度为13 
 */
void Dis_PercentSlider(uint32 dat, uint32 tol, uint32 x, uint32 y, uint32 height, uint32 width, COLOR color_f, COLOR color_b) {
#define SLIDER_H 	30	//15
   uint32 percent;
   uint32 m, n;
   width = width - 1;
   if (dat > 0) dat--;
   if (tol > 0) {
      tol--;
      percent = dat * 100 / tol;
   } else percent = 0;
   if (percent > 100) percent = 100;
   Dis_VLine(x, y, height, C_Gray1);
   m = (height - SLIDER_H) * percent / 100;
   Dis_BoxUp(x + 1, y + m, width, SLIDER_H, color_f);
   //填充滑动块上部的背景色
   if (m > 0) {
      Dis_RectFill(x + 1, y, width, m, color_b);
   }
   //填充滑动块下部的背景色
   n = m + SLIDER_H;
   if (n < height) {
      Dis_RectFill(x + 1, y + n, width, height - n, color_b);
   }
}


/**
 * @brief 示进度信息(进度框可设定)
 * @param [in] title
 * @param [in]  dat          
 * @param [in] tol
 * @param [in]  width
 * @param [in] color 
 * @return   none       
 * @note  窗体高度固定为90
 */
void Dis_SpeedInfo(TEXTCHAR *title, uint32 dat, uint32 tol, uint32 width, COLOR color) {
   enum {BAR_H = 28};
   uint32 i;
   uint32 percent;
   uint32 m;
   static uint32 ago_percent = 0;        //之前的百分比值
   static uint32 ago_width = 0;          //之前的宽度
   static uint32 now_width = 0;          //当前的宽度
   S_Form form;
   form.Width = width;
   form.Height = 90;             //进度窗体高度固定为90
   form.X = GET_ScrCenterX(form.Width);      //窗体的X坐标固定显示在中间
   form.Y = GET_ScrCenterY(form.Height);     //窗体的Y坐标固定在中间
   while (1) {
      if (dat > tol) break;
      percent = dat * 100 / tol;
      //第一次显示时刷新整个框体----------
      if (SysFlag.SpeedDis == 0) {
         SysFlag.SpeedDis = 1;
         ago_percent = 0;
         ago_width = 0;
         //将显示进度条前的画面复制
         LcdMemPush(form.X, form.Y, form.Width, form.Height);
         form.TextColor = C_White;
         form.TitleText = PTEXT(title);
         form.TitleColor = FORM_Red;
         form.TitleHeight = TITLE_H16;
         form.TitleFont = FONT16;
         form.BackColor = color;
         form.FillBack = 1;
         Dis_WindowsForm(&form);
         //进度条的底色---------------
         Dis_BoxDown(form.X + 10, form.Y + 50, form.Width - 20, BAR_H, C_Gray1);
         Dis_Strings("%", form.X + form.Width / 2 + 10, form.Y + 30, FONT16, C_Blue, color);
      }

      if (percent == ago_percent) break; //若百分比不变则不刷新显示(节约时间)
      now_width = (form.Width - 20 - 4) * percent / 100;
      m = now_width - ago_width;
      for (i = 0; i < 24; i++) {
         Dis_HLine(form.X + 12 + ago_width, form.Y + 52 + i, m, FormTitleColor[FORM_Blue][i]);
      }
      Dis_NumHide(percent, form.X + form.Width / 2 - 16, form.Y + 30, 3, FONT16, C_Blue, color);
      ago_percent = percent;
      ago_width = now_width;
      break;
   }

   if (dat >= tol) {
      if (dat == tol) DelayMs(400);
      if (SysFlag.SpeedDis > 0) {
         LcdMemPop();
      }
      SysFlag.SpeedDis = 0;
   }
}



/**
 * @brief 显示文件名(和扩展名)
 * @param [in] fdt 32字节的文件目录项信息
 * @param [in] ext 0表示不显示扩展名,1表示显示扩展名         
 * @param [in] x 起始的x坐标
 * @param [in] y 起始的y坐标
 * @param [in] color_f  文字的颜色
 * @param [in] color_b  文字的背景色
 * @return  void 
 */
void Dis_FileName(uint8 *fdt, uint32 ext, uint16 x, uint16 y, COLOR color_f, COLOR color_b) {
   uint32 i;
   uint32 hz_low;
   uint32 hz = 0;
   uint32 dat;
   uint32 dis_len = 0;
   //文件名
   hz_low = 0;
   for (i = 0; i < 8; i++) {
      dat = *(fdt + i);
      if (dat == 0x20) break;
      else if (dat < 0xa0) {
         Dis_Asc(dat, x, y, color_f, color_b);
         dis_len += 8;
      } else {
         if (hz_low == 0) {
            hz = dat;
            hz *= 256;
            hz_low = 1;
         } else {
            hz += dat;
            hz_low = 0;
            Dis_Chinese(hz, x - 8, y, color_f, color_b);
            dis_len += 16;
         }
      }
      x += 8;
   }
   //不显示扩展名---
   if (ext == 0) {
      if (dis_len < 64) {
         Dis_RectFill(x, y, 64 - dis_len, 16, color_b);
      }
      return;
   }
   //扩展名--------------------------------------
   Dis_Strings(".", x, y, FONT16, color_f, color_b);
   dis_len += 8;
   x += 8;
   for (i = 0; i < 3; i++) {
      dat = *(fdt + 8 + i);
      if (dat == 0x20) break;
      Dis_Asc(dat, x, y, color_f, color_b);
      dis_len += 8;
      x += 8;
   }

   if (dis_len < 96) {
      Dis_RectFill(x, y, 96 - dis_len, 16, color_b);
   }
}


/**
 * @brief 显示文件的大小
 * @param [in]  fdt 32字节的文件目录项信息
 * @param [in]   x   起始的x坐标      
 * @param [in]   y    起始的y坐标
 * @param [in]  color_f 文字的颜色
 * @param [in]  color_b 文字的背景色
 * @return  void 
 */
void Dis_FileSize(uint8 *fdt, uint16 x, uint16 y, COLOR color_f, COLOR color_b) {
   uint32 i;
   uint32 bytes;
   uint32 wei[7];
   uint32 cusu[7] = { 1000000, 100000, 10000, 1000, 100, 10, 1 };
   uint32 start_wei = 0;
   uint32 dis_len = 0;
   bytes = *(fdt + 0x1c) + *(fdt + 0x1d) * 0x100 + *(fdt + 0x1e) * 0x10000 + *(fdt + 0x1f) * 0x10000000;
   if (bytes > 9999999) bytes = 9999999;
   for (i = 0; i < 7; i++) {
      wei[i] = bytes / cusu[i];
      bytes = bytes % cusu[i];
   }
   start_wei = 0;
   for (i = 0; i < 7; i++) {
      if ((wei[i] > 0) || (start_wei > 0) || (i == 6)) {
         start_wei = 1;
         Dis_NumHide(wei[i], x, y, 1, FONT16, color_f, color_b);
         dis_len += 8;
         x += 8;
      }
   }

   if (dis_len < 56) {
      Dis_RectFill(x, y, 56 - dis_len, 16, color_b);
   }
}



/**
 * @brief  显示文件的大小
 * @param [in] fdt 32字节的文件目录项信息
 * @param [in] x  起始的x坐标         
 * @param [in] y  起始的y坐标
 * @param [in]  color_f 文字的颜色
 * @param [in]  color_b 文字的背景色
 */
void Dis_FileTime(uint8 *fdt, uint16 x, uint16 y, COLOR color_f, COLOR color_b) {
   Get_FileTime(fdt, &FatTime);
   Dis_NumAll(FatTime.Year, x, y, 4, FONT16, color_f, color_b);
   Dis_Strings("-", x + 32, y, FONT16, color_f, color_b);
   Dis_NumAll(FatTime.Month, x + 40, y, 2, FONT16, color_f, color_b);
   Dis_Strings("-", x + 56, y, FONT16, color_f, color_b);
   Dis_NumAll(FatTime.Data, x + 64, y, 2, FONT16, color_f, color_b);
   Dis_RectFill(x + 80, y, 10, 16, color_b);
   x += 90;
   Dis_NumAll(FatTime.Hour, x, y, 2, FONT16, color_f, color_b);
   Dis_Strings(":", x + 16, y, FONT16, color_f, color_b);
   Dis_NumAll(FatTime.Minute, x + 24, y, 2, FONT16, color_f, color_b);
   Dis_Strings(":", x + 40, y, FONT16, color_f, color_b);
   Dis_NumAll(FatTime.Second, x + 48, y, 2, FONT16, color_f, color_b);
}


/**
 * @brief 显示文件信息(文件名,扩展名,大小,修改时间)
 * @param [in] fdt 文件目录项数据指针(32字节)
 * @param [in] ext 表示是否要显示扩展名,不为0显示         
 * @param [in] x 显示位置的x坐标值
 * @param [in] y 显示位置的y坐标值
 * @param [in] color_f 显示字符的前景色
 * @param [in] color_b 显示字符的背景色
 * @return    none 
 */
void Dis_FileInfoFlord(uint8 *fdt, uint32 ext, uint16 x, uint16 y, COLOR color_f, COLOR color_b) {
   //当前目录项不是子目录处理-----------------
   if (fdt[FAT_Properties] != FAT_Dir) {
      Dis_FileName(fdt, ext, x, y, color_f, color_b);
      if (ext > 0) {
         Dis_RectFill(x + 12 * 8, y, 115 - 12 * 8, 16, color_b);
         x += 115;
      } else {
         Dis_RectFill(x + 8 * 8, y, 70 - 8 * 8, 16, color_b);
         x += 70;
      }
      Dis_FileSize(fdt, x, y, color_f, color_b);
      Dis_RectFill(x + 8 * 7, y, 80 - 8 * 7, 16, color_b);
      Dis_FileTime(fdt, x + 80, y, color_f, color_b);
   }
   //是文件夹处理----------------------
   else {
      //当前在子目录下处理-----
      if (fdt[0] == '.') {
         Dis_RectFill(x, y, 115 + 80 + 8 * 20, 16, C_Black);
         Dis_Strings(Hz_FatDir[1][Lang], x, y, FONT16, color_f, color_b);
      } else {
         Dis_RectFill(x, y, 115 + 80 + 8 * 20, 16, C_Black);
         Dis_FileName(fdt, 0, x, y, color_f, color_b);
         x += 115;
         Dis_Strings(Hz_FatDir[0][Lang], x, y, FONT16, color_f, C_Black);
//			color_b=C_Black;
//			Dis_RectFill(x+8*7,y,80-8*7,16,color_b);
//			Dis_FileTime(fdt,x+80,y,color_f,color_b);
      }
   }
}



/**
 * @brief 将文件列表进行排序(先显示文件夹)
 * @param [in] tol 文件列表中的总文件数
 * @param [in] file_list  文件列表的目录项信息(返回排序后的列表) 
 * @return none
 */
void File_SortByFloder(uint32 tol, uint8 *file_list) {
   uint32 i, j;
   uint32 m;
   uint8 fdt[MAX_Files][32];
   uint32 floders;                 //列表中的文件夹个数
   uint32 files;                   //列表中的文件个数
   uint32 floder_count;
   uint32 file_count;
   if (tol <= 1) return;          //若不到2个文件则直接返回

   floders = 0;
   files = 0;
   //获取文件夹和文件的个数-------
   m = FAT_Properties;
   for (i = 0; i < tol; i++) {
      if (file_list[m] == FAT_Dir) {
         floders++;
      } else {
         files++;
      }
      m += 32;
   }

   floder_count = 0;
   file_count = floders;

   //将原始文件列表排序-------
   m = 0;
   for (i = 0; i < tol; i++) {
      if (file_list[m + FAT_Properties] == FAT_Dir) {
         for (j = 0; j < 32; j++) {
            fdt[floder_count][j] = file_list[m + j];
         }
         floder_count++;
      } else {
         for (j = 0; j < 32; j++) {
            fdt[file_count][j] = file_list[m + j];
         }
         file_count++;
      }
      m += 32;
   }

   //将排序后的列表返回-----
   m = 0;
   for (i = 0; i < tol; i++) {
      for (j = 0; j < 32; j++) {
         file_list[m + j] = fdt[i][j];
      }
      m += 32;
   }
}



/**
 * @brief 从文件(指定扩展名)列表框中选择一个文件
 * @param [in] title 标题栏显示的文本(居中显示)
 * @param [in] file_fdt 
 *        传递进来的扩展名(前3字节),返回当前选中的文件信息
 * @param [in] device 设备类型 0=SD卡,1=U盘,2=NandFlash 
 * @return  0xffffffff表示取消操作 
 * @note 无删除文件功能 
 */
uint32 Dis_FileSelectOne(const char *title, uint8 *file_fdt, uint32 device) {
   const char *Hz_SelDel[][LANG_MAX] =
   {
      "退出", "Esc",
      "确定", "Enter"
   };
   enum {MAX_Num = 10,BOXH = 220,SLIDW = 25};
   uint32 k = 0;                     //当前光标在本页的序号
   uint32 k_start = 0;                   //光标在最顶上的序号
   uint32 i;
   uint32 m;
   uint32 x, y;
   uint8 file_list[32 * MAX_Files];
   uint32 file_tol;
   uint32 color_f, color_b;
   uint32 ret;
   uint32 ref_list;                    // 1表示重新读取文件信息
   S_Form form;
   S_GuiXY guixy;
   Touch_KeyPush();
   ref_list = 1;
   file_tol = 0;
   TchVal.Item[0] = k;

   form.Width = 420;
   form.Height = 320;
   form.X = GET_ScrCenterX(form.Width);
   form.Y = GET_ScrCenterY(form.Height);
   form.TitleColor = FORM_Blue;
   form.TitleHeight = TITLE_H24;
   form.TitleFont = FONT24;
   form.TextColor = C_White;
   form.TitleText = title;
   form.BackColor = C_Back;
   form.FillBack = 1;

   guixy.StartX = form.X + 20;
   guixy.StartY = form.Y + form.TitleHeight + 20;

   Sys.LcdRef = REF_Item;
   Sys.DatChang = 0;
   LcdMemPush(form.X, form.Y, form.Width, form.Height);
   Dis_WindowsForm(&form);
   x = form.X + 10; y = form.Y + form.Height - 40;
   Dis_Strings(HZ_FileTol[0][Lang], x, y, FONT16, C_Black, C_Back);
   Touch_KeyEn(form.X, form.Y, form.Width, form.TitleHeight, KEY_ESC, 10);

   x = form.X + 150;
   y = form.Y + form.Height - 50;
   Dis_TouchButton(Hz_SelDel[0][Lang], x, y, 110, 40, KEY_ESC, 13, C_Black, FONT24);

   x = form.X + 280;
   Dis_TouchButton(Hz_SelDel[1][Lang], x, y, 110, 40, KEY_OK, 14, C_Black, FONT24);
   TchVal.KeyNum[0] = MAX_Num + 5;

   FatDir.SubDir[device] = 0;            //表示在根目录下
   while (1) {
      if (ref_list > 0) {
         ref_list = 0;
         Dis_BoxDown(guixy.StartX - 10, guixy.StartY - 10, form.Width - 20, BOXH, C_Black);
         if (device == NAND) i = MAX_MemFiles;
         else i = MAX_Files;
         file_tol = ListFileNameFloder(file_fdt, i, file_list, device);
         File_SortByFloder(file_tol, file_list);
         Dis_NumHide(file_tol, form.X + 10 + 60, form.Y + form.Height - 40, 4, FONT16, C_Red, C_Back);
      }

      if (Sys.LcdRef > 0) {
         x = form.X + form.Width - 10 - SLIDW - 2;
         Dis_PercentSlider(k + k_start + 1, file_tol, x, guixy.StartY - 10 + 2, BOXH - 4, SLIDW, C_Green, C_Gray1);
         Touch_KeyEn(x, guixy.StartY - 10 + 2, SLIDW, 60, KEY_PU, 11);            //上翻键
         Touch_KeyEn(x, guixy.StartY - 10 + BOXH - 60, SLIDW, 60, KEY_PD, 12);  //下翻键

         x = guixy.StartX;
         y = guixy.StartY;
         for (i = 0; i < 10; i++) {
            if (k == i) {
               color_f = C_Red;
               color_b = C_Yellow;
            } else {
               color_f = C_White;
               color_b = C_Black;
            }

            m = k_start + i;
            if (m >= file_tol) {
               Dis_RectFill(x, y, form.Width - 60, 16, C_Black);
               TouchKeyDis(i);
            } else {
               Dis_FileInfoFlord(&file_list[(i + k_start) * 32], 1, x, y, color_f, color_b);
               Touch_KeyEn(x, y, 320, 16, KEY_Tch, i);
            }
            y += 20;
         }
         Sys.k = k;
         Sys.LcdRef = 0;
      }

      Read_Key();

      Dis_TouchButtonFlash();

      //有效的触摸点按下处理(序号)---------
      if ((TchVal.Down == TCH_KeyDown) && (Key.Value == KEY_Tch)) {
         TchVal.Down = 0;
         if (k != TchVal.Item[0]) {
            k = TchVal.Item[0];
            Sys.LcdRef = REF_Now;
         }
         //目录项双击当确定键处理---------
         else {
            m = k_start + k;
            if ((m + 1) <= file_tol) {
               m *= 32;
               if (file_list[m + FAT_Properties] == FAT_Dir) {
                  Key.Value = KEY_OK;
               }
            }
         }
      }

      //下移一项-----------------
      if (Key.Value == KEY_DOWN) {
         if ((k < (MAX_Num - 1)) && ((k_start + k) < (file_tol - 1))) {
            Sys.LcdRef = REF_Item;
            k++;
         } else if ((k_start + MAX_Num) < file_tol) {
            Sys.LcdRef = REF_Item;
            k_start++;
         }
      }
      //上移一项-----------------
      else if (Key.Value == KEY_UP) {
         if (k > 0) {
            Sys.LcdRef = REF_Item;
            k--;
         } else if (k_start > 0) {
            Sys.LcdRef = REF_Item;
            k_start--;
         }
      }
      //下翻一页----------------
      else if (Key.Value == KEY_PD) {
         if (file_tol > (k_start + MAX_Num)) {
            Sys.LcdRef = REF_Item;
            k_start += MAX_Num;
            if (k >= (file_tol - k_start)) {
               k = file_tol - k_start - 1;       //光标到最后一项
            }
         }
      }
      //上翻一页----------------
      else if (Key.Value == KEY_PU) {
         if (k_start >= MAX_Num) {
            Sys.LcdRef = REF_Item;
            k_start -= MAX_Num;
         } else if (k_start > 0) {
            Sys.LcdRef = REF_Item;
            k_start = 0;
         }
      }	else if (Key.Value == KEY_OK) {
         m = (k + k_start) * 32;
         //文件夹处理------------------------------
         if (file_list[m + FAT_Properties] == FAT_Dir) {
            //返回到上级目录处理------------------
            if (file_list[m] == '.') {
               ref_list = 1;
               FatDir.CurrentDir[26] = FatDir.ParentDir[26];
               FatDir.CurrentDir[27] = FatDir.ParentDir[27];
               FatDir.CurrentDir[20] = FatDir.ParentDir[20];
               FatDir.CurrentDir[21] = FatDir.ParentDir[21];
               if ((FatDir.CurrentDir[26] == 0) && (FatDir.CurrentDir[27] == 0) && (FatDir.CurrentDir[20] == 0) && (FatDir.CurrentDir[21] == 0)) {
                  FatDir.SubDir[device] = 0;
               }
               k = 0;                    //光标移到第1项
               k_start = 0;
               Sys.LcdRef = REF_Item;
            }
            //进入文件夹-----------
            else {
               ref_list = 1;
               FatDir.SubDir[device] = 0x55;
               FatDir.CurrentDir[26] = file_list[m + 26];
               FatDir.CurrentDir[27] = file_list[m + 27];
               FatDir.CurrentDir[20] = file_list[m + 20];
               FatDir.CurrentDir[21] = file_list[m + 21];
               k = 0;                    //光标移到第1项
               k_start = 0;
               Sys.LcdRef = REF_Item;
            }
         } else {
            if (file_tol == 0) ret = DO_Cancel;
            else {
               ret = 0;
               for (i = 0; i < 32; i++) {
                  file_fdt[i] = file_list[m + i];
               }
               //传递当前子目录的簇号--------
               if (FatDir.SubDir[device] == 0x55) {
                  file_fdt[26] = file_list[26];
                  file_fdt[27] = file_list[27];
                  file_fdt[20] = file_list[20];
                  file_fdt[21] = file_list[21];
               } else {
                  file_fdt[26] = 0;
                  file_fdt[27] = 0;
                  file_fdt[20] = 0;
                  file_fdt[21] = 0;
               }
            }
            break;
         }
      }
      if (Key.Value == KEY_ESC) {
         ret = DO_Cancel;
         break;
      }
   }
   Key.Value = KEY_No;
   Touch_KeyPop();
   LcdMemPop();
   return (ret);
}
#endif
