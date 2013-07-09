
#include "hw_types.h"
#include "usblib.h"

//*****************************************************************************
//
// Flag to indicate whether or not we have been initialized.
//
//*****************************************************************************
tUSBInstanceObject g_USBInstance[USB_NUM_INSTANCE];

tUSBPerfInfo g_USBPerfInfo[5000];
unsigned int ulPerfInfoCounter=0;
unsigned int fReadEnabled=0;
unsigned int fWriteEnabled=0;
unsigned int fUSBDisconnected = 0;


//*****************************************************************************
//
// Close the Doxygen group.
// @}
//
//*****************************************************************************
