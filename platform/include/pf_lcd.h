/**
 * \file   pl_lcd.h
 *
 * \brief  This file contains prototype declarations of functions which 
 *         performs EVM configurations.
 */

/*
* Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
*/
/*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions
*  are met:
*
*    Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


#ifndef _PL_LCD_H_
#define _PL_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FOR_TEST

#define LCD_XSize				800
#define LCD_YSize				480

//for display
//#define	LCD_24BIT_888RGB
#define	LCD_16BIT_565RGB

/*
 * Graphic Data Format (GDF) bits
 */
#define GDF__8BIT_INDEX          0
#define GDF_15BIT_555RGB        1
#define GDF_16BIT_565RGB        2
#define GDF_32BIT_X888RGB      3
#define GDF_24BIT_888RGB        4
#define GDF__8BIT_332RGB        5


#define	FRAME_WIDTH		1024
#define	FRAME_HEIGHT		720

#define	LCD_WIDTH		800
#define	LCD_HEIGHT		480

#define PALETTE_SIZE 	32
#define PALETTE_OFFSET 	4


#define FRAME_BUFFER_SIZE	(FRAME_WIDTH*FRAME_HEIGHT*4 + PALETTE_SIZE + PALETTE_OFFSET)



typedef struct __lcdc_panel {
	char	name[24];	/* Full name <vendor>_<model> */
	unsigned short		width;
	unsigned short		height;
	int		hfp;				/* Horizontal front porch */
	int		hbp;			/* Horizontal back porch */
	int		hsw;			/* Horizontal Sync Pulse Width */
	int		vfp;				/* Vertical front porch */
	int		vbp;			/* Vertical back porch */
	int		vsw;			/* Vertical Sync Pulse Width */
	unsigned int	pxl_clk;		/* Pixel clock */
	unsigned char	invert_pxl_clk;	/* Invert Pixel clock */
}tLCD_PANEL,*pLCD_PANEL;


//像素结构
typedef struct __fb_bitfield{
   unsigned int offset;
   unsigned int length;
   unsigned int msb_right;
}tFB_BITFIELD,*pFB_BTFILD;



//LCD模块设置数据结构定义----------------------
typedef struct _S_LcdCtrl
{
    unsigned int     baseAddr;
    const tLCD_PANEL    *panel;
	unsigned int	 lcd_clk;			//for hardware configure to input LCD controler
    void             *palette[2];        //palette address
    unsigned int     palettesize[2];                                        //
    void             *frameaddr[2];		//显存首地址指针
	uint32           framesize[2];		//显存字节数
	unsigned int 	 frame_num;			//display buffer number
	uint32           activeframe;	    //当前显示的显存	
    uint32           contexFrame;   //当前操作的显存
    unsigned int     pixsize;           //sizeof pix
	tFB_BITFIELD     red;
    tFB_BITFIELD     green;
    tFB_BITFIELD     blue;
   	tFB_BITFIELD     stransp;			     
} tLCDCTRL;

extern tLCDCTRL lcdCtrl;
extern void LCDBackLightON(void);
extern void LCDBackLightOFF(void);
extern void LCDModuleClkConfig(void);
extern unsigned int LCDVersionGet(void);
extern void LCDRasterEOFIntEnable(void);
extern void LCDRasterEOFIntDisable(void);
extern void LCDRasterStart(void);
extern void LCDRasterInit(int panelIndex);
extern void * LCDFrameBufferAddrGet(int num);
extern unsigned int  LCDFrameBufferCurGet(void);
extern void LCDFrameBufferCurSet(unsigned int num);
extern void LCDSwapFb(void);
extern void LCDSwapContex(void);
extern const  tLCD_PANEL *LCDTftInfoGet(void);

#define FrameBuffer(X)  ((unsigned short *)(lcdCtrl.frameaddr[lcdCtrl.contexFrame]))[(X)]
#define FrameBuffer2D(X,Y) ((unsigned short *)(lcdCtrl.frameaddr[lcdCtrl.contexFrame]))[(Y)*lcdCtrl.panel->width+(X)]
#define Pix(X,Y) FrameBuffer2D(X,Y)


#ifdef __cplusplus
#endif

#endif

/******************************** End of file *******************************/

