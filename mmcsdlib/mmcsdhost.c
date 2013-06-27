#include "mmcsd_proto.h"
#include "hs_mmcsd.h"
#include "type.h"
#include "debug.h"
#include "module.h"
#include "mmath.h"

mmcsdCtrlInfo mmcsdctr[2];

BOOL hsMmcSdCardPresent(mmcsdCtrlInfo *ctrl){
   return HSMMCSDIsCardInserted(ctrl->memBase);
}

void hsMmcSdBusWidthSet(mmcsdCtrlInfo *ctrl, unsigned char buswidth,BOOL ddr){
   HSMMCSDBusWidthSetDdr(ctrl->memBase, buswidth,ddr);
}

void hsMmcSdBusFreqSet(mmcsdCtrlInfo *ctrl, unsigned int busFreq){
   HSMMCSDBusFreqSet(ctrl->memBase, ctrl->ipClk, busFreq, 0);
}

void hsMmcSdInit(mmcsdCtrlInfo *ctrl)
{
    int status = 0;
    unsigned int baseaddr = modulelist[ctrl->moduleId].baseAddr;
    moduleEnable(ctrl->moduleId);

    /*Refer to the MMC Host and Bus configuration steps in TRM */
    /* controller Reset */
    status = HSMMCSDSoftReset(baseaddr);

    if (status != 0)
    {
        mdError("HS MMC/SD Reset failed");
    }
    
    HSMMCSDIntrStatusClear(baseaddr,0xFFFFFFFF);

    /* Lines Reset */
    HSMMCSDLinesReset(baseaddr, HS_MMCSD_ALL_RESET);

    /* Set supported voltage list */
    HSMMCSDSupportedVoltSet(baseaddr, HS_MMCSD_SUPPORT_VOLT_1P8 |
                                           HS_MMCSD_SUPPORT_VOLT_3P0
                             |HS_MMCSD_SUPPORT_VOLT_3P0);  

    HSMMCSDSystemConfig(baseaddr, HS_MMCSD_AUTOIDLE_ENABLE);

    /* Set the bus width */
    HSMMCSDBusWidthSet(baseaddr, HS_MMCSD_BUS_WIDTH_1BIT );

    /* Set the bus voltage */
    HSMMCSDBusVoltSet(baseaddr, HS_MMCSD_BUS_VOLT_3P0);

    /* Bus power on */
    status = HSMMCSDBusPower(baseaddr, HS_MMCSD_BUS_POWER_ON);
   
    mdAssert(status ==0 );

    /* Set the initialization frequency */  
    HSMMCSDBusFreqSet(baseaddr, ctrl->ipClk, 400000, 0);

    HSMMCSDInitStreamSend(baseaddr);

    unsigned int intr = (HS_MMCSD_INTR_CMDCOMP | HS_MMCSD_INTR_CMDTIMEOUT |
                            HS_MMCSD_INTR_DATATIMEOUT | HS_MMCSD_INTR_TRNFCOMP);
    HSMMCSDIntrEnable(baseaddr, intr); 
    moduleIntConfigure(ctrl->moduleId);
}

/**
 * \brief   This function sends the command to MMCSD.
 *
 * \param    mmcsdCtrlInfo It holds the mmcsd control information.
 *
 * \param    mmcsdCmd It determines the mmcsd cmd
 *
 * \return   status of the command.
 *
 */
unsigned int hsMmcSdCmdSend(mmcsdCtrlInfo *ctrl, mmcsdCmd *c)
{
    unsigned int blksize;
    unsigned int status = 0;
    unsigned int rspType;
    unsigned int dataPresent;
    unsigned int datadir;
    unsigned int cmd = 0;

    if (c->flags & MMCSD_CMDFLAG_TYPE_STOP)
    {
         cmd |= HS_MMCSD_CMD_TYPE_SUSPEND;
    }
    else if (c->flags & MMCSD_CMDFLAG_TYPE_FUNCSEL)
    {
         cmd |= HS_MMCSD_CMD_TYPE_FUNCSEL;
    }
    else if (c->flags & MMCSD_CMDFLAG_TYPE_ABORT)
    {
         cmd |= HS_MMCSD_CMD_TYPE_ABORT;
    }
    
    
    dataPresent = !!(c->flags & MMCSD_CMDFLAG_DATA);
    datadir = !!(c->flags & (1<<MMCSD_CMDFLAG_DATA_RW_SHIFT));
    blksize = dataPresent ? c->blksize : 0;
    
    if (c->flags & MMCSD_CMDFLAG_RSP_NONE)
    {
        rspType = HS_MMCSD_NO_RESPONSE;
    }
    else if (c->flags & MMCSD_CMDFLAG_RSP_136BITS)
    {
        rspType = HS_MMCSD_136BITS_RESPONSE;
    }
    else if (c->flags & MMCSD_CMDFLAG_RSP_BUSY)
    {
        rspType = HS_MMCSD_48BITS_BUSY_RESPONSE;
    }
    else
    {
        rspType = HS_MMCSD_48BITS_RESPONSE;
    }
    
    cmd = (c->idx<<MMCHS_CMD_INDX_SHIFT)|rspType
            |(datadir<<MMCHS_CMD_DDIR_SHIFT)
            |(dataPresent<<MMCHS_CMD_DP_SHIFT)
            |(ctrl->dmaEnable<<0)
            |(dataPresent<<MMCHS_CMD_MSBS_SHIFT)
            |(dataPresent<<MMCHS_CMD_BCE_SHIFT); 
              
    if(dataPresent){
        HSMMCSDIntrStatusClear(ctrl->memBase, MMCHS_STAT_CC |(1<< MMCHS_STAT_TC_SHIFT) | MMCHS_STAT_CTO);
        HSMMCSDDataTimeoutSet(ctrl->memBase, HS_MMCSD_DATA_TIMEOUT(27));
    }else{
        HSMMCSDIntrStatusClear(ctrl->memBase, MMCHS_STAT_CC | MMCHS_STAT_CTO);
    }
        
    HSMMCSDCommandSend(ctrl->memBase, cmd, c->arg,
                       blksize,c->nblks);
  
    status = ctrl->cmdStatusGet(ctrl);
    if(0 == status){
        return 0;
    }
    if (HS_MMCSD_NO_RESPONSE!=rspType)
    {
        HSMMCSDResponseGet(ctrl->memBase, c->rsp);
    }

    if ((rspType == HS_MMCSD_48BITS_BUSY_RESPONSE)&&
        (!dataPresent)&&(ctrl->xferStatusGet(ctrl)==0)) {
       return 0;
    }
    return 1;
}



