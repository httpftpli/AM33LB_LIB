#include "platform.h"
#include "pf_platform_cfg.h"
#include "usblib.h"
#include "usbhost.h"
#include "cppi41dma.h"
#include "usbhmsc.h"
#include "ff.h"
#include "pf_usbmsc.h"


#define USB_INSTANCE 1



//*****************************************************************************
//
// The instance data for the MSC driver.
//
//*****************************************************************************
unsigned int g_ulMSCInstance = 0;

//*****************************************************************************
// Current FAT fs 
//*****************************************************************************
static FATFS g_sFatFs;


//*****************************************************************************
//
// Declare the USB Events driver interface.
//
//*****************************************************************************
DECLARE_EVENT_DRIVER(g_sUSBEventDriver, 0, 0, USBHCDEvents);

//*****************************************************************************
//
// The global that holds all of the host drivers in use in the application.
// In this case, only the MSC class is loaded.
//
//*****************************************************************************
static tUSBHostClassDriver const * const g_ppHostClassDrivers[] =
{
    &g_USBHostMSCClassDriver,
    &g_sUSBEventDriver
};

//*****************************************************************************
//
// This global holds the number of class drivers in the g_ppHostClassDrivers
// list.
//
//*****************************************************************************
#define NUM_CLASS_DRIVERS       (sizeof(g_ppHostClassDrivers) /               \
                                 sizeof(g_ppHostClassDrivers[0]))


//*****************************************************************************
//
// Hold the current state for the application.
//
//*****************************************************************************

volatile tUsbMscState g_usbMscState = USBMSC_NO_DEVICE;


//*****************************************************************************
//
// This is the generic callback from host stack.
//
// \param pvData is actually a pointer to a tEventInfo structure.
//
// This function will be called to inform the application when a USB event has
// occurred that is outside those related to the mass storage device.  At this
// point this is used to detect unsupported devices being inserted and removed.
// It is also used to inform the application when a power fault has occurred.
// This function is required when the g_USBGenericEventDriver is included in
// the host controller driver array that is passed in to the
// USBHCDRegisterDrivers() function.
//
// \return None.
//
//*****************************************************************************
void USBHCDEvents(void *pvData)
{
    tEventInfo *pEventInfo;

    //
    // Cast this pointer to its actual type.
    //
    pEventInfo = (tEventInfo *)pvData;

    switch(pEventInfo->ulEvent)
    {
        //
        // New keyboard detected.
        //
        case USB_EVENT_CONNECTED:
        {
            //
            // An unknown device was detected.
            //
            g_usbMscState = USBMSC_UNKNOWN_DEVICE;

            break;
        }

        //
        // Keyboard has been unplugged.
        //
        case USB_EVENT_DISCONNECTED:
        {
            //
            // Unknown device has been removed.
            //
            g_usbMscState = USBMSC_NO_DEVICE;

            break;
        }

        case USB_EVENT_POWER_FAULT:
        {
            //
            // No power means no device is present.
            //
            g_usbMscState = USBMSC_POWER_FAULT;

            break;
        }

        default:
        {
            break;
        }
    }
}

#if USB_USE_CPPI41DMA  

endpointInfo epInfo[]=
{
	{
		USB_EP_TO_INDEX(USB_EP_1),
		CPDMA_DIR_RX,
		CPDMA_MODE_SET_TRANSPARENT,
	},
	
	{
		USB_EP_TO_INDEX(USB_EP_1),
		CPDMA_DIR_TX,
		CPDMA_MODE_SET_GRNDIS,
	},
	
	{
		USB_EP_TO_INDEX(USB_EP_2),
		CPDMA_DIR_RX,
		CPDMA_MODE_SET_TRANSPARENT,
	},

	{
		USB_EP_TO_INDEX(USB_EP_2),
		CPDMA_DIR_TX,
		CPDMA_MODE_SET_GRNDIS,
	}

};

#define NUMBER_OF_ENDPOINTS  4

#endif


static void MSCCallback(unsigned int ulInstance, unsigned int ulEvent, void *pvData)
{
    //
    // Determine the event.
    //
    switch(ulEvent)
    {
        //
        // Called when the device driver has successfully enumerated an MSC
        // device.
        //
        case MSC_EVENT_OPEN:
        {
            //
            // Proceed to the enumeration state.
            //
            g_usbMscState = USBMSC_DEVICE_ENUM;
            break;
        }

        //
        // Called when the device driver has been unloaded due to error or
        // the device is no longer present.
        //
        case MSC_EVENT_CLOSE:
        {
            //
            // Go back to the "no device" state and wait for a new connection.
            //
            g_usbMscState = USBMSC_NO_DEVICE;

            break;
        }

        default:
        {
            break;
        }
    }
}

//*****************************************************************************
//
// The current USB operating mode - Host, Device or unknown.
//
//*****************************************************************************
tUSBMode g_eCurrentUSBMode;

#define HCD_MEMORY_SIZE         128
unsigned char g_pHCDPool[HCD_MEMORY_SIZE];



void usbMscInit(){
 // Register the host class drivers.
    //
    USBHCDRegisterDrivers(USB_INSTANCE_FOR_USBDISK, g_ppHostClassDrivers, NUM_CLASS_DRIVERS);

    //
    // Open an instance of the mass storage class driver.
    //
    g_ulMSCInstance = USBHMSCDriveOpen(USB_INSTANCE_FOR_USBDISK, USB_INSTANCE_FOR_USBDISK, MSCCallback);

    //
    // Initialize the power configuration.  This sets the power enable signal
    // to be active high and does not enable the power fault.
    //
    USBHCDPowerConfigInit(USB_INSTANCE_FOR_USBDISK, USBHCD_VBUS_AUTO_HIGH);
	
#if USB_USE_CPPI41DMA
	Cppi41DmaInit(USB_INSTANCE_FOR_USBDISK, epInfo, NUMBER_OF_ENDPOINTS);
#endif
	//
	// Initialize the host controller.
	//
	USBHCDInit(USB_INSTANCE_FOR_USBDISK, g_pHCDPool, HCD_MEMORY_SIZE);

 }


void usbMscProcess() {
   USBHCDMain(USB_INSTANCE_FOR_USBDISK, g_ulMSCInstance);
   switch (g_usbMscState) {
   case USBMSC_NO_DEVICE:

      break;
   case USBMSC_DEVICE_ENUM:
      if (USBHMSCDriveReady(g_ulMSCInstance) == 0) {
         f_mount(FatFS_Drive_Index, &g_sFatFs);
         g_usbMscState = USBMSC_DEVICE_READY;
      }
      break;
   case USBMSC_DEVICE_READY:
      break;
   default:
      break;
   }
}


