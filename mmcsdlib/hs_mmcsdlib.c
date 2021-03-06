/**
 * \file   hs_mmcsdlib.c
 *
 * \brief  MMCSD library API's.
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


#include "hs_mmcsd.h"
#include "edma.h"
#include "debug.h"
#include "mmcsd_proto.h"
#include "string.h"


/**
 * \brief    init mmcsdCtrlInfo struct 
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \return   1: SUCCESS  0:FAIL
 *
 * \note: that this functional may not be available for all instances of the
 * controler. This function, is only useful of the controller has a dedicated
 * card detect pin. If not, the card detection mechanism is application
 * implementation specific 
 **/





/**
 * \brief   Set output bus frequency
 *
 * \param   mmcsdCtrlInfo It holds the mmcsd control information.
 * \param   busFreq       The required output frequency on the bus
 *
 * \return   0  on clock enable success
 *          -1  on clock enable fail
 *
int HSMMCSDBusFreqConfig(mmcsdCtrlInfo *ctrl, unsigned int busFreq)
{
    return HSMMCSDBusFreqSet(ctrl->memBase, ctrl->ipClk, busFreq, 0);
}
*/
