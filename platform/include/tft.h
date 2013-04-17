/**
 * \file   tft.h
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
		.hbp = 46-20,
		.hsw = 20,
		.vfp = 12,
		.vbp = 23-10,
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
