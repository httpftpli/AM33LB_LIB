/*
 * \file   hs_mmcsdlib.h
 *
 * \brief  MMC/SD Library API prototypes
 *
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

#ifndef __MMCSD_LIB_H__
#define __MMCSD_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif


#define HSMMCSD_BUSWIDTH_DDR        (1<<15)


/* Prototype declarations */

extern unsigned char mmcsdCtrlInfoInit(mmcsdCtrlInfo *ctrl,unsigned int memBase,unsigned int ipClk,
                       unsigned int opClk,unsigned short busWidthSupport,
                       unsigned short busDdrSupport,                 
                       unsigned int intrMask, mmcsdCardInfo *card,
                       void preXferHook(mmcsdCtrlInfo *ctrl,unsigned char rwFlag,
                                      void *buf,unsigned int blksize,unsigned int nBlks),
                       unsigned int cmdstatusget(mmcsdCtrlInfo *ctrl),
                       unsigned int xferstatusget(mmcsdCtrlInfo *ctrl)
                       );

extern int HSMMCSDBusFreqConfig(mmcsdCtrlInfo *ctrl, unsigned int busFreq);
extern unsigned int HSMMCSDCmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c);
extern unsigned int HSMMCSDControllerInit(mmcsdCtrlInfo *ctrl);
#ifdef __cplusplus
}
#endif

#endif

