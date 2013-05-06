#ifndef __MMCSD_PROTO_HOOKS_H__
#define __MMCSD_PROTO_HOOKS_H__

#include "mmcsd_proto.h"

extern void HSMMCSDXferSetup(mmcsdCtrlInfo *ctrl, unsigned char rwFlag, void *ptr, 
                             unsigned int blkSize, unsigned int nBlks);

/*
 * Check command status.will block
 */
extern unsigned int HSMMCSDCmdStatusGet(mmcsdCtrlInfo *ctrl);
/*
 * Check data transfer finish .will block
 */
extern unsigned int HSMMCSDXferStatusGet(mmcsdCtrlInfo *ctrl);



#endif


















