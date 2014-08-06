/**
*  \file lwiplib.c
*
*  \brief lwip related initializations
*/
/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
*/

/*
** Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
** ALL RIGHTS RESERVED
*/

/*
** lwIP Compile Time Options for StarterWare.
*/
#include "lwiplib.h"
#include "netif/cpswif.h"
#include "lwip/dhcp.h"
#include "lwip/init.h"
#include "lwipopts.h"
#include "delay.h"



/*
** lwIP high-level API/Stack/IPV4/SNMP/Network Interface/PPP codes
*/



/******************************************************************************
**                       INTERNAL FUNCTION PROTOTYPES
******************************************************************************/
static void lwIPDHCPComplete(unsigned int ifNum);

/******************************************************************************
**                       INTERNAL VARIABLE DEFINITIONS
******************************************************************************/
/*
** The lwIP network interface structure for CPSW ports.
*/
#ifdef CPSW_DUAL_MAC_MODE
static struct netif cpswNetIF[MAX_CPSW_INST * MAX_SLAVEPORT_PER_INST];
#else
static struct netif cpswNetIF[MAX_CPSW_INST];
#endif

/*
** Helper to identify ports
*/
static struct cpswportif cpswPortIf[MAX_CPSW_INST * MAX_SLAVEPORT_PER_INST];

/******************************************************************************
**                          FUNCTION DEFINITIONS
******************************************************************************/
/**
 * \brief   This function waits for DHCP completion with a timeout
 *
 * \param   ifNum  The netif number for the interface
 *
 * \return  None
*/
static void lwIPDHCPComplete(unsigned int ifNum)
{
    unsigned int dhcpTries = NUM_DHCP_TRIES;
    int cnt;
    volatile unsigned char *state;

    while(dhcpTries--)
    {
        LWIP_PRINTF("\n\rDHCP Trial %d...", (NUM_DHCP_TRIES - dhcpTries));
        dhcp_start(&cpswNetIF[ifNum]);

        cnt = LWIP_DHCP_TIMEOUT;

        /* Check for DHCP completion for 'cnt' number of times, each 10ms */
        while(cnt--)
        {
            delay(10);
            state = &(cpswNetIF[ifNum].dhcp->state);
            if(DHCP_BOUND == *state)
            {
                return;
            }
        }
    }

    LWIP_PRINTF("\n\rUnable to complete DHCP! \n\r");
}

/**
 *
 * \brief Initializes the lwIP TCP/IP stack.
 *
 * \param lwipIf  The interface structure for lwIP
 *
 * \return IP Address.
*/
unsigned int lwIPInit(LWIP_IF *lwipIf)
{
    struct ip_addr ip_addr;
    struct ip_addr net_mask;
    struct ip_addr gw_addr;
    unsigned int *ipAddrPtr;
    static unsigned int lwipInitFlag = 0;
    unsigned int ifNum;
    unsigned int temp;

    /* do lwip library init only once */
    if(0 == lwipInitFlag)
    {
        lwip_init();
    }

    /* Setup the network address values. */
    if(lwipIf->ipMode == IPADDR_USE_STATIC)
    {
        ip_addr.addr = htonl(lwipIf->ipAddr);
        net_mask.addr = htonl(lwipIf->netMask);
        gw_addr.addr = htonl(lwipIf->gwAddr);
    }
    else
    {
        ip_addr.addr = 0;
        net_mask.addr = 0;
        gw_addr.addr = 0;
    }

#ifdef CPSW_DUAL_MAC_MODE
    ifNum = (lwipIf->instNum * MAX_SLAVEPORT_PER_INST) + lwipIf->slvPortNum - 1;
#else
    ifNum = lwipIf->instNum;
#endif

    cpswPortIf[ifNum].inst_num = lwipIf->instNum;
    cpswPortIf[ifNum].port_num = lwipIf->slvPortNum;

    /* set MAC hardware address */
    for(temp = 0; temp < LEN_MAC_ADDRESS; temp++)
    {
        cpswPortIf[ifNum].eth_addr[temp] =
                         lwipIf->macArray[(LEN_MAC_ADDRESS - 1) - temp];
    }

    /*
    ** Create, configure and add the Ethernet controller interface with
    ** default settings.  ip_input should be used to send packets directly to
    ** the stack. The lwIP will internaly call the cpswif_init function.
    */
    if(NULL ==
       netif_add(&cpswNetIF[ifNum], &ip_addr, &net_mask, &gw_addr,
                 &cpswPortIf[ifNum], cpswif_init, ip_input))
    {
        LWIP_PRINTF("\n\rUnable to add interface for interface %d", ifNum);
        return 0;
    }

    if(0 == lwipInitFlag)
    {
        netif_set_default(&cpswNetIF[ifNum]);
        lwipInitFlag = 1;
    }

    /* Start DHCP, if enabled. */
#if LWIP_DHCP
    if(lwipIf->ipMode == IPADDR_USE_DHCP)
    {
        lwIPDHCPComplete(ifNum);
    }
#endif

    /* Start AutoIP, if enabled and DHCP is not. */
#if LWIP_AUTOIP
    if(lwipIf->ipMode == IPADDR_USE_AUTOIP)
    {
        autoip_start(&cpswNetIF[ifNum]);
    }
#endif

    if((lwipIf->ipMode == IPADDR_USE_STATIC)
       ||(lwipIf->ipMode == IPADDR_USE_AUTOIP))
    {
       /* Bring the interface up */
       netif_set_up(&cpswNetIF[ifNum]);
    }

    ipAddrPtr = (unsigned int*)&(cpswNetIF[ifNum].ip_addr);

    return (*ipAddrPtr);
}

/*
 * \brief   Checks if the ethernet link is up
 *
 * \param   instNum     The instance number of CPSW module
 * \param   slvPortNum  The Slave Port Number
 *
 * \return  Interface status.
*/
unsigned int lwIPNetIfStatusGet(unsigned int instNum, unsigned int slvPortNum)
{
    unsigned int ifNum;

    ifNum = instNum * MAX_SLAVEPORT_PER_INST + slvPortNum - 1;

    return (cpswif_netif_status(&cpswNetIF[ifNum]));
}

/*
 * \brief   Checks if the ethernet link is up
 *
 * \param   instNum     The instance number of CPSW module
 * \param   slvPortNum  The Slave Port Number
 *
 * \return  The link status.
*/
unsigned int lwIPLinkStatusGet(unsigned int instNum, unsigned int slvPortNum)
{
    return (cpswif_link_status(instNum, slvPortNum));
}

/**
 * \brief   Interrupt handler for Receive Interrupt. Directly calls the
 *          cpsw interface receive interrupt handler.
 *
 * \param   instNum  The instance number of CPSW module for which receive
 *                   interrupt happened
 *
 * \return  None.
*/
void lwIPRxIntHandler(unsigned int instNum)
{
    cpswif_rx_inthandler(instNum, &cpswNetIF[0]);
}

/**
 * \brief   Interrupt handler for Transmit Interrupt. Directly calls the
 *          cpsw interface transmit interrupt handler.
 *
 * \param   instNum  The instance number of CPSW module for which transmit
 *                   interrupt happened
 *
 * \return  None.
*/
void lwIPTxIntHandler(unsigned int instNum)
{
    cpswif_tx_inthandler(instNum);
}

/**
 * \brief   Starts DHCP negotiation
 *
 * \param   instNum     The instance number of CPSW module
 * \param   slvPortNum  The Slave Port Number
 *
 * \return  IP address assigned. If IP acquisition failed, zero will be returned.
*/
unsigned int lwIPDHCPStart(unsigned int instNum, unsigned int slvPortNum)
{
    unsigned int *ipAddrPtr;
    unsigned int ifNum;

    ifNum = instNum * MAX_SLAVEPORT_PER_INST + slvPortNum - 1;

    lwIPDHCPComplete(ifNum);

    ipAddrPtr = (unsigned int*)&(cpswNetIF[ifNum].ip_addr);

    return (*ipAddrPtr);
}

/***************************** End Of File ***********************************/















