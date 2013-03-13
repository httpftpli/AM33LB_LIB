#include "pf_platform_cfg.h"
#include "usblib.h"
#include "usbhost.h"
#include "cppi41dma.h"


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
typedef enum
{
    //
    // No device is present.
    //
    STATE_NO_DEVICE,

    //
    // Mass storage device is being enumerated.
    //
    STATE_DEVICE_ENUM,

    //
    // Mass storage device is ready.
    //
    STATE_DEVICE_READY,

    //
    // An unsupported device has been attached.
    //
    STATE_UNKNOWN_DEVICE,

    //
    // A power fault has occurred.
    //
    STATE_POWER_FAULT
} tState;
volatile tState g_eState;
volatile tState g_eUIState;

#ifdef USB_USE_DMA

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

#define NUMBER_OF_ENDPOINTS 4

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
            g_eState = STATE_DEVICE_ENUM;
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
            g_eState = STATE_NO_DEVICE;

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
    g_ulMSCInstance = USBHMSCDriveOpen(USB_INSTANCE_FOR_USBDISK, 0, MSCCallback);

    //
    // Initialize the power configuration.  This sets the power enable signal
    // to be active high and does not enable the power fault.
    //
    USBHCDPowerConfigInit(USB_INSTANCE_FOR_USBDISK, USBHCD_VBUS_AUTO_HIGH);
	
#if USB_USE_DMA
	Cppi41DmaInit(USB_INSTANCE_FOR_USBDISK, epInfo, NUMBER_OF_ENDPOINTS);
#endif
	//
	// Initialize the host controller.
	//
	USBHCDInit(USB_INSTANCE_FOR_USBDISK, g_pHCDPool, HCD_MEMORY_SIZE);
 }
