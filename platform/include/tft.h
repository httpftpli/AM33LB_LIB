/**
 * \file   tft.h
 *
 * \brief  This file contains prototype declarations of functions which 
 *         performs EVM configurations.
 */


#ifndef _TFT_H_
#define _TFT_H_

#include "pf_lcd.h"

#ifdef __cplusplus
extern "C" {
#endif

static  const tLCD_PANEL lcd_panels[] = {
	[0]={
		.name = "TFT_AT080TN52",
		.width = 800,
		.height = 600,
		.hfp = 40,
		.hbp = 40,
		.hsw = 48,
		.vfp = 13,
		.vbp = 29,
		.vsw = 3,
		.pxl_clk = 40000000,
		.invert_pxl_clk = 0,
		},
	[1]={
		.name = "TFT_AT070TN92",
		.width = 800,
		.height = 480,
		.hfp = 210,
		.hbp = 46-20,
		.hsw = 20,
		.vfp = 22,
		.vbp = 23-10,
		.vsw = 10,
		.pxl_clk = 33300000,
		.invert_pxl_clk = 0,
		},
    [2] = {
		.name = "LSA40AT9001",
		.width = 800,
		.height = 600,
		.hfp = 210,
		.hbp = 46, //46-20
		.hsw = 20,
		.vfp = 12,
		.vbp = 13, //23-10
		.vsw = 10,
		.pxl_clk = 40000000,
		.invert_pxl_clk = 0,
		}
};

#ifdef __cplusplus
}
#endif

#endif

/******************************** End of file *******************************/
