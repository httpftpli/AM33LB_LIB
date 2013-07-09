
#include "hw_types.h"
#include "debug.h"
#include "usblib.h"
#include "usblibpriv.h"

//*****************************************************************************
//
// \addtogroup general_usblib_api
// @{
//
//*****************************************************************************

//*****************************************************************************
//
// These are the internal timer tick handlers used by the USB stack.  Handlers
// in g_pfTickHandlers are called in the context of the USB SOF interrupt
// every USB_SOF_TICK_DIVIDE milliseconds.
//
//*****************************************************************************
tUSBTickHandler g_pfTickHandlers[USB_TICK_HANDLER_NUM];
void *g_pvTickInstance[USB_TICK_HANDLER_NUM];

//*****************************************************************************
//
// Flag to indicate whether or not we have been initialized.
//
//*****************************************************************************
tBoolean g_bUSBTimerInitialized = false;

//*****************************************************************************
//
// This is the current tick value in ms for the system.  This is used for all
// instances of USB controllers and for all timer tick handlers.
//
//*****************************************************************************
unsigned int g_ulCurrentUSBTick = 0;

//*****************************************************************************
//
// This is the total number of SOF interrupts received since the system
// booted.  The value is incremented by the low level device- or host-interrupt
// handler functions.
//
//*****************************************************************************
unsigned int g_ulUSBSOFCount = 0;

//*****************************************************************************
//
// This internal function initializes the variables used in processing timer
// ticks.
//
// This function should only be called from within the USB library.  It is set
// up to ensure that it can be called multiple times if necessary without
// the previous configuration being erased (to cater for OTG mode switching).
//
// \return None.
//
//*****************************************************************************
void
InternalUSBTickInit(void)
{
    unsigned int ulLoop;

    if(!g_bUSBTimerInitialized)
    {
        for(ulLoop = 0; ulLoop < USB_TICK_HANDLER_NUM; ulLoop++)
        {
            g_pfTickHandlers[ulLoop] = (tUSBTickHandler)0;
            g_pvTickInstance[ulLoop] = 0;
        }

        g_bUSBTimerInitialized = true;
    }
}

//*****************************************************************************
//
// This internal function handles registering OTG, Host, or Device SOF timer
// handler functions.
//
// \param ulHandler specifies which type of handler to register.
// \param pfHandler specifies the handler to call for the given type of
// handler.
//
// This function should only be called inside the USB library and only as a
// result to a call to reinitialize the stack in a new mode.  Currently the
// following 3 types of timer tick handlers can be registered:
// TICK_HANDLER_OTG, TICK_HANDLER_HOST, or TICK_HANDLER_DEVICE.  Handlers
// registered via this function are called in the context of the SOF interrupt.
//
// \return None.
//
//*****************************************************************************
void
InternalUSBRegisterTickHandler(unsigned int ulHandler,
                               tUSBTickHandler pfHandler,
                               void *pvInstance)
{
    ASSERT(ulHandler < USB_TICK_HANDLER_NUM);

    //
    // Save the handler.
    //
    g_pfTickHandlers[ulHandler] = pfHandler;

    //
    // Save the instance data.
    //
    g_pvTickInstance[ulHandler] = pvInstance;
}

//*****************************************************************************
//
//! \internal
//!
//! Calls internal handlers in response to a tick based on the start of frame
//! interrupt.
//!
//! \param ulTicksmS specifies how many milliseconds have passed since the last
//! call to this function.
//!
//! This function is called every 5mS in the context of the Start of Frame
//! (SOF) interrupt.  It is used to call any registered internal tick
//! functions.
//!
//! This function should only be called from within the USB library.
//!
//! \return None.
//
//*****************************************************************************
void InternalUSBStartOfFrameTick(unsigned int ulTicksmS, 
                                    unsigned int ulIndex)
{
    int iIdx;

    //
    // Advance time.
    //
    g_ulCurrentUSBTick += ulTicksmS;

    //
    // Call any registered SOF tick handlers.
    //
    for(iIdx = 0; iIdx < USB_TICK_HANDLER_NUM; iIdx++)
    {
        if(g_pfTickHandlers[iIdx])
        {
            g_pfTickHandlers[iIdx](g_pvTickInstance[iIdx], ulTicksmS, ulIndex);
        }
    }
}

//*****************************************************************************
//
// Close the Doxygen group.
// @}
//
//*****************************************************************************
