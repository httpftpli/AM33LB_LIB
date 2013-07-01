#ifndef __PF_USBMCS_C__
#define __PF_USBMCS_C__


typedef enum
{
    //
    // No device is present.
    //
    USBMSC_NO_DEVICE,

    //
    // Mass storage device is being enumerated.
    //
    USBMSC_DEVICE_ENUM,

    //
    // Mass storage device is ready.
    //
    USBMSC_DEVICE_READY,

    //
    // An unsupported device has been attached.
    //
    USBMSC_UNKNOWN_DEVICE,

    //
    // A power fault has occurred.
    //
    USBMSC_POWER_FAULT
} tUsbMscState;

extern volatile tUsbMscState g_usbMscState;
extern void usbMscProcess();
extern void usbMscInit();


#endif
