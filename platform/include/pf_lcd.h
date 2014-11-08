/**
 * \file   pl_lcd.h
 *
 * \brief
 *
 */



#ifndef _PL_LCD_H_
#define _PL_LCD_H_

#include "pf_platform_cfg.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FOR_TEST

#define LCD_XSize				(lcdCtrl.panel->width)
#define LCD_YSize				(lcdCtrl.panel->height)

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


#define PALETTE_SIZE 	32
#define PALETTE_OFFSET 	4


#define FRAME_BUFFER_SIZE	(FRAME_WIDTH*FRAME_HEIGHT*4 + PALETTE_SIZE + PALETTE_OFFSET)


typedef struct __lcdc_panel {
	char	name[24];	                /* Full name <vendor>_<model> */
	unsigned short		width;
	unsigned short		height;
	unsigned int		hfp;			/* Horizontal front porch */
	unsigned int		hbp;			/* Horizontal back porch */
	unsigned int		hsw;			/* Horizontal Sync Pulse Width */
	unsigned int		vfp;				/* Vertical front porch */
	unsigned int		vbp;			/* Vertical back porch */
	unsigned int		vsw;			/* Vertical Sync Pulse Width */
	unsigned int	pxl_clk;		/* Pixel clock */
	unsigned char	invert_pxl_clk;	        /* Invert Pixel clock */
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


extern void * fb;
extern tLCDCTRL lcdCtrl;
extern void LCDBackLightON(unsigned char lightpwm);
extern void LCDBackLightOFF(void);
//extern void LCDModuleClkConfig(void);
extern unsigned int LCDVersionGet(void);
extern void LCDRasterEOFIntEnable(void);
extern void LCDRasterEOFIntDisable(void);
extern void LCDRasterStart(void);
extern void LCDRasterEnd(void);
extern void LCDRasterInit();
extern void * LCDFrameBufferAddrGet(int num);
extern unsigned int  LCDFrameBufferCurGet(void);
extern void LCDFrameBufferCurSet(unsigned int num);
extern void LCDSwapFb(void);
extern void LCDSwapContex( );
extern const  tLCD_PANEL *LCDTftInfoGet(void);
extern void LCDFbClear(unsigned int color);
extern void LCDDrawMask(const void *buf, unsigned short x, unsigned short y, unsigned short width,
                  unsigned short height, unsigned int color_f, unsigned int color_b);
extern void renderLocalBegin(void *localfb,bool swapContex);
extern void renderLocalEnd(void *localfb, bool swapFb);



#if LCD_PIX_SIZE==2
#define FrameBuffer(X)  ((unsigned short*)fb)[X]  // ((unsigned short *)(lcdCtrl.frameaddr[lcdCtrl.contexFrame]))[(X)]
#define  FrameBuffer2D(X,Y) ((unsigned short*)fb)[Y*lcdCtrl.panel->width+X] //((unsigned short *)(lcdCtrl.frameaddr[lcdCtrl.contexFrame]))[(Y)*lcdCtrl.panel->width+(X)]
#elif LCD_PIX_SIZE==4
#define FrameBuffer(X)  ((unsigned int*)fb)[X]   //((unsigned int *)(lcdCtrl.frameaddr[lcdCtrl.contexFrame]))[(X)]
#define FrameBuffer2D(X,Y) ((unsigned int*)fb)[Y*lcdCtrl.panel->width+X] //((unsigned int *)(lcdCtrl.frameaddr[lcdCtrl.contexFrame]))[(Y)*lcdCtrl.panel->width+(X)]
#else
#error
#endif
#define Pix(X,Y) FrameBuffer2D(X,Y)



/**
 * @brief  写一个像素点
 * @param [in] x
 * @param [in] y
 * @param [in] color
 * @return none

 */
static inline void LCD_SetPixel(unsigned int x, unsigned int y, unsigned int color) {
#if LCD_PIX_SIZE==2
    Pix((x), (y)) = (unsigned short)color;
#elif LCD_PIX_SIZE==4
    Pix((x), (y)) = color;
#else
#error
#endif
}


inline unsigned int LCD_GetPixel(unsigned int x,unsigned int y ){
   return Pix((x), (y));
}

#ifdef __cplusplus
#endif

#endif

/******************************** End of file *******************************/

