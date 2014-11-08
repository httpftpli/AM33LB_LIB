/**
 *  \file   pf_lcd.c
 *
 *  \brief
 *  \author  lfl
 *  \addtogroup LCD
 *  \# include "pf_lcd.h"
 *  @{
 *
 */



#include <string.h>
#include "soc_AM335x.h"
#include "hw_types.h"
#include "hw_cm_per.h"
#include "hw_cm_wkup.h"
#include "hw_cm_dpll.h"
#include "hw_control_AM335x.h"
#include "interrupt.h"
#include "cache.h"
#include "raster.h"
#include "debug.h"
#include "pf_platform_cfg.h"
#include "tft.h"
#include "pf_lcd.h"
#include "pf_edma.h"
#include "gpio_v2.h"
#include "module.h"
#include "mem.h"
#include "lib_gui.h"


#ifdef LCD_16BIT_565RGB
#undef LCD_24BIT_888RGB
#endif

extern void __lcd_back_ligth_ctr(unsigned char val);

//void LCDModuleClkConfig(void);
void LCDRasterEOFIntEnable(void);
void LCDRasterEOFIntDisable(void);
void * LCDFrameBufferAddrGet(int num);
unsigned int  LCDFrameBufferCurGet(void);
void LCDFrameBufferCurSet(unsigned num);
void LCDWriteColorFrameBuffer(unsigned int num, int lX, int lY, int width, int height, unsigned int color);
void LCDColorDisplay(void *pDisplayBuf, int lX, int lY, int width, int height, unsigned int color, int bpp);


// 32 byte Palette.
const static  unsigned int palette_32b[PALETTE_SIZE/4] =
{ 0x4000u, 0x0000u, 0x0000u, 0x0000u, 0x0000u, 0x0000u, 0x0000u, 0x0000u };


static void *framebuffer = (void *)(0x80000000+DDR_RAM_SIZE-FRAMEBUFFER_SIZE);

void * fb;


tLCDCTRL lcdCtrl = {
   .frame_num = 2,
   .pixsize = LCD_PIX_SIZE,
   .red.length = 5,
   .red.offset = 11,
   .red.msb_right = 0,
   .green.length = 6,
   .green.offset = 5,
   .green.msb_right = 0,
   .blue.length = 5,
   .blue.offset = 0,
   .blue.msb_right = 0
};



unsigned int LCDVersionGet(void)
{
    return 2;
}

/*void LCDModuleClkConfig(void)
{
	HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) |=CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;
	while((HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) & CM_PER_L3S_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

	HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) |=CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;
	while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) & CM_PER_L3_CLKSTCTRL_CLKTRCTRL) != CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

	HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
                             CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_INSTR_CLKCTRL) &
                               CM_PER_L3_INSTR_CLKCTRL_MODULEMODE) !=
                                   CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) |=
                             CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKCTRL) &
        CM_PER_L3_CLKCTRL_MODULEMODE) != CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE);

    HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
                             CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
                              CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL) !=
                                CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) |=
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) &
                             CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL) !=
                               CM_PER_L4LS_CLKSTCTRL_CLKTRCTRL_SW_WKUP);

    HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) |=
                             CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKCTRL) &
      CM_PER_L4LS_CLKCTRL_MODULEMODE) != CM_PER_L4LS_CLKCTRL_MODULEMODE_ENABLE);

    // lcd pixel clock is derived from peripheral pll
    HWREG(SOC_CM_DPLL_REGS + CM_DPLL_CLKSEL_LCDC_PIXEL_CLK) =
                             CM_DPLL_CLKSEL_LCDC_PIXEL_CLK_CLKSEL_SEL3;

    HWREG(SOC_PRCM_REGS + CM_PER_LCDC_CLKCTRL) |=
                             CM_PER_LCDC_CLKCTRL_MODULEMODE_ENABLE;
//CM_PER_LCDC_CLKSTCTRL

    HWREG(SOC_PRCM_REGS + CM_PER_LCDC_CLKSTCTRL) &= CM_PER_LCDC_CLKCTRL_MODULEMODE;

    while((HWREG(SOC_PRCM_REGS + CM_PER_LCDC_CLKCTRL) &
      CM_PER_LCDC_CLKCTRL_MODULEMODE) != CM_PER_LCDC_CLKCTRL_MODULEMODE_ENABLE);

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3S_CLKSTCTRL) &
            CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L3_CLKSTCTRL) &
            CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           (CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK |
            CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L4_GCLK)));

    while(!(HWREG(SOC_PRCM_REGS + CM_PER_L4LS_CLKSTCTRL) &
           (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_L4LS_GCLK |
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_LCDC_GCLK)));
}*/



/*
**  A wrapper function which enables the End-Of-Frame interrupt of Raster.
*/
void LCDRasterEOFIntEnable(void)
{
    /* Enable End of frame0/frame1 interrupt */
    RasterIntEnable(SOC_LCDC_0_REGS, RASTER_END_OF_FRAME0_INT |
                                     RASTER_END_OF_FRAME1_INT);

}

/*
** A wrapper function which disables the End-Of-Frame interrupt of Raster.
*/
void LCDRasterEOFIntDisable(void)
{
    /* Enable End of frame0/frame1 interrupt */
    RasterIntDisable(SOC_LCDC_0_REGS, RASTER_END_OF_FRAME0_INT |
                                     RASTER_END_OF_FRAME1_INT);

}

/*
** For each end of frame interrupt base and ceiling is reconfigured
*/
void isr_lcd(unsigned int num) {
   unsigned int  status;
   status = RasterIntStatus(SOC_LCDC_0_REGS, RASTER_END_OF_FRAME0_INT_STAT | RASTER_END_OF_FRAME1_INT_STAT);
   status = RasterClearGetIntStatus(SOC_LCDC_0_REGS, status);

   if (status & RASTER_END_OF_FRAME0_INT_STAT) {
      RasterDMAFBConfig(SOC_LCDC_0_REGS, (unsigned int)lcdCtrl.frameaddr[lcdCtrl.activeframe],
                        (unsigned int)lcdCtrl.frameaddr[lcdCtrl.activeframe] + lcdCtrl.framesize[lcdCtrl.activeframe] - 1, 0);
   }
   if (status & RASTER_END_OF_FRAME1_INT_STAT) {
      RasterDMAFBConfig(SOC_LCDC_0_REGS, (unsigned int)lcdCtrl.frameaddr[lcdCtrl.activeframe],
                        (unsigned int)lcdCtrl.frameaddr[lcdCtrl.activeframe] + lcdCtrl.framesize[lcdCtrl.activeframe] - 1, 1);
   }
}

/**
 * @brief LCD控制
 * @return   none
 * @param [in] light
 *  LCD light pwm
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void LCDBackLightCtr(unsigned char lightpwm) {
   __lcd_back_ligth_ctr(lightpwm);
}

/**
 * @brief 开启LCD背光
 * @return   none
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void LCDBackLightON(unsigned char lightpwm) {
  /*unsigned int addr = modulelist[GPIO_LCDBACKLIGHT_MODULE].baseAddr;
   if (GPIO_DIR_INPUT == GPIODirModeGet(addr,GPIO_LCDBACKLIGHT_PIN)){
       GPIODirModeSet(addr, GPIO_LCDBACKLIGHT_PIN, GPIO_DIR_OUTPUT);
   }
       GPIOPinWrite(addr, GPIO_LCDBACKLIGHT_PIN, 0);*/

   LCDBackLightCtr(lightpwm);
}


/**
 * @brief 关闭LCD背光
 * @return   none
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void LCDBackLightOFF(void)
{
   /*if (GPIO_DIR_INPUT == GPIODirModeGet(modulelist[GPIO_LCDBACKLIGHT_MODULE].baseAddr,
                                        GPIO_LCDBACKLIGHT_PIN)){
      GPIODirModeSet(modulelist[GPIO_LCDBACKLIGHT_MODULE].baseAddr,GPIO_LCDBACKLIGHT_PIN,GPIO_DIR_OUTPUT);
   }
   GPIOPinWrite(modulelist[GPIO_LCDBACKLIGHT_MODULE].baseAddr, GPIO_LCDBACKLIGHT_PIN,1);*/

   LCDBackLightCtr(0);
}

/**
 * @brief LCD开始显示
 * @return   none
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void LCDRasterStart(void) {
   /* configuring the base ceiling */
   RasterDMAFBConfig(SOC_LCDC_0_REGS, (uint32)lcdCtrl.palette[0], (uint32)lcdCtrl.frameaddr[0] + lcdCtrl.framesize[0] - 1, 0);
   RasterDMAFBConfig(SOC_LCDC_0_REGS, (uint32)lcdCtrl.palette[1], (uint32)lcdCtrl.frameaddr[1] + lcdCtrl.framesize[1] - 1, 1);
   /* enable raster */
   RasterEnable(SOC_LCDC_0_REGS);
}

/**
 * @brief LCD停止显示
 * @return   none
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void LCDRasterEnd(void){
   RasterDisable(SOC_LCDC_0_REGS);
}



/**
 * @brief 切换LCD显示的显存
 * @return   none
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void LCDSwapFb(void) {
   if (lcdCtrl.activeframe == 0) lcdCtrl.activeframe = 1;
   else lcdCtrl.activeframe = 0;
}


/**
 * @brief 切换LCD读写的显存
 * @return   none
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */

void LCDSwapContex(){
   lcdCtrl.contexFrame = !lcdCtrl.contexFrame;
   fb =  lcdCtrl.frameaddr[lcdCtrl.contexFrame];
}


void renderLocalBegin(void *localfb,bool swapContex){
    ASSERT((unsigned int)localfb%512==0);
    fb = localfb;
    if (swapContex) {
        lcdCtrl.contexFrame = !lcdCtrl.contexFrame;
    }
}


static void __renderocalend_swapfb(unsigned int tcc,unsigned int status){
    if (tcc==1) {
        LCDSwapFb();
    }
}

void renderLocalEnd(void *localfb, bool swapFb){
    ASSERT((unsigned int)localfb%512==0);
    unsigned int dstAddr = (unsigned int)lcdCtrl.frameaddr[lcdCtrl.contexFrame];
    CacheDataCleanBuff((unsigned int)localfb, lcdCtrl.framesize[lcdCtrl.contexFrame]);
    EDMARequestXferArray(EDMA3_TRIG_MODE_QDMA,
                         1,(unsigned int)localfb,dstAddr,
                         lcdCtrl.framesize[lcdCtrl.contexFrame],
                         1);
    fb =  lcdCtrl.frameaddr[lcdCtrl.contexFrame];
    if (swapFb) {
        EDMARegisterHandler(1, __renderocalend_swapfb);
    }
}


/**
 * @brief LCD模块初始化
 * @return   none
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void LCDRasterInit() {
   MODULE *module = modulelist+MODULE_ID_LCDC;
   unsigned int baseaddr = module->baseAddr;
   lcdCtrl.baseAddr = baseaddr;
   lcdCtrl.lcd_clk = module->moduleClk->fClk[0]->clockSpeedHz;
   const tLCD_PANEL *panel = lcd_panels + TFT_PANEL;
   lcdCtrl.panel = panel;
   lcdCtrl.activeframe = 0;
   lcdCtrl.contexFrame = 0;
   unsigned int pixsize = lcdCtrl.pixsize;
   unsigned int width = lcd_panels[TFT_PANEL].width;
   unsigned int height = lcd_panels[TFT_PANEL].height;
   lcdCtrl.palettesize[0] = 32;
   lcdCtrl.palettesize[1] = 32;
   lcdCtrl.framesize[0] = (unsigned int)(pixsize * width * height);
   lcdCtrl.framesize[1] = (unsigned int)(pixsize * width * height);
   lcdCtrl.palette[0] = framebuffer;
   lcdCtrl.frameaddr[0] = (void *)((unsigned int)framebuffer + 32);
   lcdCtrl.palette[1] = (void *)((unsigned int)(lcdCtrl.frameaddr[0]) +  lcdCtrl.framesize[0]);
   lcdCtrl.frameaddr[1] = (void *)((unsigned int)lcdCtrl.palette[1] + 32);
   lcdCtrl.activeframe = 0;

   //init palette and framebuffer
   memcpy(lcdCtrl.palette[0],palette_32b,lcdCtrl.palettesize[0]);
   memcpy(lcdCtrl.palette[1],palette_32b,lcdCtrl.palettesize[1]);
   memset32(lcdCtrl.frameaddr[0], 0, pixsize * lcdCtrl.panel->height * lcdCtrl.panel->width/4);
   memset32(lcdCtrl.frameaddr[1], 0, pixsize * lcdCtrl.panel->height * lcdCtrl.panel->width/4);
   moduleEnable(MODULE_ID_LCDC);
   RasterClocksEnable(baseaddr);
   RasterAutoUnderFlowEnable(baseaddr);
   RasterIntEnable(baseaddr, RASTER_END_OF_FRAME0_INT | RASTER_END_OF_FRAME1_INT );
   RasterDisable(baseaddr);
   RasterClkConfig(baseaddr, lcdCtrl.panel->pxl_clk, lcdCtrl.lcd_clk);
   RasterDMAConfig(baseaddr, RASTER_DOUBLE_FRAME_BUFFER,
                   RASTER_BURST_SIZE_16, RASTER_FIFO_THRESHOLD_8,
                   RASTER_BIG_ENDIAN_DISABLE);




   /* Configuring modes(ex:tft or stn,color or monochrome etc) for raster controller */
   if (2 == pixsize) {
       RasterModeConfig(baseaddr, RASTER_DISPLAY_MODE_TFT | (LCDC_RASTER_CTRL_PLM_DATA << LCDC_RASTER_CTRL_PLM_SHIFT),
                        RASTER_PALETTE_DATA, RASTER_COLOR, RASTER_RIGHT_ALIGNED);
   } else if (4 == pixsize) {
       RasterModeConfig(baseaddr, RASTER_DISPLAY_MODE_TFT_UNPACKED | (LCDC_RASTER_CTRL_PLM_DATA << LCDC_RASTER_CTRL_PLM_SHIFT),
                        RASTER_PALETTE_DATA, RASTER_COLOR, RASTER_RIGHT_ALIGNED);
   } else {
       mdError("pixsize should equal 2 or 4");
   }


   /* Configuring the polarity of timing parameters of raster controller */

   RasterTiming2Configure(baseaddr, RASTER_FRAME_CLOCK_LOW |
                             RASTER_LINE_CLOCK_HIGH |   //RASTER_LINE_CLOCK_LOW  |
                             RASTER_PIXEL_CLOCK_HIGH |
                             RASTER_SYNC_EDGE_RISING |
                             RASTER_SYNC_CTRL_ACTIVE |
                             RASTER_AC_BIAS_HIGH, 0, 255);

   RasterHparamConfig(baseaddr, panel->width, panel->hsw, panel->hfp, panel->hbp);
   RasterVparamConfig(baseaddr, panel->height, panel->vsw, panel->vfp, panel->vbp);

   RasterFIFODMADelayConfig(baseaddr, 128);

   moduleIntConfigure(MODULE_ID_LCDC);
   fb = lcdCtrl.frameaddr[0];
}



void * LCDFrameBufferAddrGet(int num)
{

  if (num !=0) {
    num = 1;
  }
  return lcdCtrl.frameaddr[num];
}



/**
 * @brief 查询当前显示的显存索引号
 * @return  索引号  0或者1
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
unsigned int  LCDFrameBufferCurGet(void)
{
  return lcdCtrl.activeframe;
}


/**
 * @brief 查询TFT面板参数
 * @return  const tLCD_PANEL *
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
const tLCD_PANEL *LCDTftInfoGet(void){
  return lcdCtrl.panel;
}




/**
 * @brief 填充整体屏幕
 * @param [in] color 颜色 ，16色
 * @return
 * @date    2013/8/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
void LCDFbClear(unsigned int color){
#if LCD_PIX_SIZE==2
    memset16(&Pix(0,0),color, LCD_XSize*LCD_YSize);
#elif LCD_PIX_SIZE==4
    memset32(&Pix(0,0),color, LCD_XSize*LCD_YSize);
#else
#error
#endif
}



extern void drawPix(uint16 x,uint16 y,COLOR color);
void LCDDrawMask(const void *buf, unsigned short x, unsigned short y, unsigned short width,
                  unsigned short height, unsigned int color_f, unsigned int color_b) {
   unsigned int nbyteperline = (width + 7) / 8;
   for (int i = 0; i < height; i++) {
      for (int j = 0; j < nbyteperline; j++) {
         for (int k = 0; k < 8; k++) {
            unsigned char mask = *((unsigned char *)buf + i * nbyteperline + j);
            unsigned int color = (mask & (1 << (8 - k))) ? color_f : color_b;
            drawPix(x + 8 * j + k, y + i, color);
         }
      }
   }
}



//! @}

