/**
 * \file   cpsw.c
 *
 * \brief  This file contains functions which configure CPSW instance
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

#include "soc_AM335x.h"
#include "hw_control_AM335x.h"
#include "hw_types.h"
#include "platform.h"
#include "hw_cm_per.h"

/******************************************************************************
**                       INTERNAL MACRO DEFINITIONS
******************************************************************************/
#define CPSW_MII_SEL_MODE                     (0x00u)
#define CPSW_MDIO_SEL_MODE                    (0x00u)
#define LEN_MAC_ADDR                          (0x06u)
#define OFFSET_MAC_ADDR                       (0x30u)



/**
 * \brief   Enables CPSW clocks
 *
 * \param   None
 *
 * \return  None.
 */
void CPSWClkEnable(void)
{
    HWREG(SOC_PRCM_REGS + CM_PER_CPGMAC0_CLKCTRL) =
                      CM_PER_CPGMAC0_CLKCTRL_MODULEMODE_ENABLE;

    while(0 != (HWREG(SOC_PRCM_REGS + CM_PER_CPGMAC0_CLKCTRL)
                & CM_PER_CPGMAC0_CLKCTRL_IDLEST));

    HWREG(SOC_PRCM_REGS + CM_PER_CPSW_CLKSTCTRL) =
                      CM_PER_CPSW_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    while(0 == (HWREG(SOC_PRCM_REGS + CM_PER_CPSW_CLKSTCTRL)
                & CM_PER_CPSW_CLKSTCTRL_CLKACTIVITY_CPSW_125MHZ_GCLK));
}

/**
 * \brief   This function sets the RGMII mode for both ports
 *
 * \param   None
 *
 * \return  None.
 */
void PortRmiiModeSelect(void)
{
    /* Select RMII, REF CLOCK FROM EXTERN PIN */
    HWREG(SOC_CONTROL_REGS + CONTROL_GMII_SEL) = 1<<6 | 0x01;
}



/**
 * \brief   This function returns the MAC address for the EVM
 *
 * \param   addrIdx    the MAC address index.
 * \param   macAddr    the Pointer where the MAC address shall be stored
 *     'addrIdx' can be either 0 or 1
 *
 * \return  None.
 */
void EVMMACAddrGet(unsigned int addrIdx, unsigned char *macAddr)
{
    macAddr[0] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx))
                   >> 8) & 0xFF;
    macAddr[1] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_LO(addrIdx)))
                  & 0xFF;
    macAddr[2] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 24) & 0xFF;
    macAddr[3] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 16) & 0xFF;
    macAddr[4] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx))
                   >> 8) & 0xFF;
    macAddr[5] =  (HWREG(SOC_CONTROL_REGS + CONTROL_MAC_ID_HI(addrIdx)))
                  & 0xFF;
}

/****************************** End Of File *********************************/
