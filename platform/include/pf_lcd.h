/**
 * \file   pl_lcd.h
 *
 * \brief   
 *        
 */

#ifndef _PL_LCD_H_
#define _PL_LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#define FOR_TEST

#define LCD_XSize				800
#define LCD_YSize				600

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

extern tLCDCTRL lcdCtrl;
extern void LCDBackLightON(void);
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
extern void LCDSwapContex(void);
extern const  tLCD_PANEL *LCDTftInfoGet(void);
extern void LCDFbClear(unsigned short color);
extern void LCDDrawMask(const void *buf, unsigned short x, unsigned short y, unsigned short width,
                  unsigned short height, unsigned int color_f, unsigned int color_b);

#define FrameBuffer(X)  ((unsigned short *)(lcdCtrl.frameaddr[lcdCtrl.contexFrame]))[(X)]
#define FrameBuffer2D(X,Y) ((unsigned short *)(lcdCtrl.frameaddr[lcdCtrl.contexFrame]))[(Y)*lcdCtrl.panel->width+(X)]
#define Pix(X,Y) FrameBuffer2D(X,Y)



/**
 * @brief  写一个像素点
 * @param [in] x
 * @param [in] y           
 * @param [in] color 
 * @return none

 */
inline void LCD_SetPixel(unsigned int x, unsigned int y, unsigned int color) {
   if ((color & 0xffff0000) == 0) {
      Pix((x), (y)) = (unsigned short)color;
   }
}


unsigned int inline  LCD_GetPixel(unsigned int x,unsigned int y ){
   return Pix((x), (y));
}

#ifdef __cplusplus
#endif

#endif

/******************************** End of file *******************************/

