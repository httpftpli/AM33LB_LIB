
#ifndef __USBHSCSI_H__
#define __USBHSCSI_H__

//*****************************************************************************
//
// If building with a C++ compiler, make all of the definitions in this header
// have a C binding.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// \addtogroup usblib_host_class
// @{
//
//*****************************************************************************

//*****************************************************************************
//
// Prototypes for the APIs exported by the USB SCSI layer.
//
//*****************************************************************************
extern unsigned int USBHSCSIInquiry(unsigned int ulIndex,
                                     unsigned int ulInPipe,
                                     unsigned int ulOutPipe,
                                     unsigned char *pucBuffer,
                                     unsigned int *pulSize);
extern unsigned int USBHSCSIReadCapacity(unsigned int ulIndex,
                                          unsigned int ulInPipe,
                                          unsigned int ulOutPipe,
                                          unsigned char *pData,
                                          unsigned int *pulSize);
extern unsigned int USBHSCSIReadCapacities(unsigned int ulIndex,
                                            unsigned int ulInPipe,
                                            unsigned int ulOutPipe,
                                            unsigned char *pData,
                                            unsigned int *pulSize);
extern unsigned int USBHSCSIModeSense6(unsigned int ulIndex,
                                        unsigned int ulInPipe,
                                        unsigned int ulOutPipe,
                                        unsigned int ulFlags,
                                        unsigned char *pData,
                                        unsigned int *pulSize);
extern unsigned int USBHSCSITestUnitReady(unsigned int ulIndex,
                                           unsigned int ulInPipe,
                                           unsigned int ulOutPipe);
extern unsigned int USBHSCSIRequestSense(unsigned int ulIndex,
                                          unsigned int ulInPipe,
                                          unsigned int ulOutPipe,
                                          unsigned char *pucData,
                                          unsigned int *pulSize);
extern unsigned int USBHSCSIRead10(unsigned int ulIndex,
                                    unsigned int ulInPipe,
                                    unsigned int ulOutPipe,
                                    unsigned int ulLBA,
                                    unsigned char *pucData,
                                    unsigned int *pulSize,
                                    unsigned int ulNumBlocks);
extern unsigned int USBHSCSIWrite10(unsigned int ulIndex,
                                     unsigned int ulInPipe,
                                     unsigned int ulOutPipe,
                                     unsigned int ulLBA,
                                     unsigned char *pucData,
                                     unsigned int *pulSize,
                                     unsigned int ulNumBlocks);

//*****************************************************************************
//
// @}
//
//*****************************************************************************

//*****************************************************************************
//
// Mark the end of the C bindings section for C++ compilers.
//
//*****************************************************************************
#ifdef __cplusplus
}
#endif

#endif // __USBHSCSI_H__
