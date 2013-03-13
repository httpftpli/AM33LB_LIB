#ifndef __PF_PLATFORM_CFG_H_
#define __PF_PLATFORM_CFG_H_

//set witch usb instance use for udist : 0 or 1
#define USB_INSTANCE_FOR_USBDISK   1
//set if use use dma or pull
#define USB_USE_DMA               1


//bellow config interrupt priority

#define INT_PRIORITY_MAX          (0x7f)
#define INT_PRIORITY_MIN           (0)
#define INT_PRIORITY_USER_NORMAL  (0x30)
#define INT_PRIORITY_USER_PRI     (0x10)
#define INT_PRIORITY_DMA_COMP     (INT_PRIORITY_USER_NORMAL - 3)
#define INT_PRIORITY_DMA_ERROR    (INT_PRIORITY_USER_NORMAL - 4) 
#define INT_PRIORITY_MMCSD0       (INT_PRIORITY_USER_NORMAL-2)
#define INT_PRIORITY_MMCSD1       (INT_PRIORITY_USER_NORMAL-1)
#define INT_PRIORITY_RTC          (INT_PRIORITY_USER_NORMAL-5)
#define INT_PRIORITY_DCAN0_LINE0  (INT_PRIORITY_USER_NORMAL-6)
#define INT_PRIORITY_TIMER2       (INT_PRIORITY_USER_NORMAL-7)
#define INT_PRIORITY_EQEP2         (INT_PRIORITY_USER_NORMAL-8)
#define INT_PRIORITY_USB0        (INT_PRIORITY_USER_NORMAL-9)
#define INT_PRIORITY_USB1        (INT_PRIORITY_USER_NORMAL-10)
#if USB_USE_DMA
#define INT_PRIORITY_USB_DMA     (INT_PRIORITY_USER_NORMAL-11)
#endif

#define INT_PRIORITY_GPIO0       (INT_PRIORITY_USER_NORMAL-12)
#define INT_PRIORITY_GPIO1       (INT_PRIORITY_USER_NORMAL-13)
#define INT_PRIORITY_GPIO2       (INT_PRIORITY_USER_NORMAL-14)
#define INT_PRIORITY_GPIO3       (INT_PRIORITY_USER_NORMAL-15)


//config GPIO
#define GPIO_USE  0x03 //bit map of GPIO use , bit 0 for gpio0 ;bit1 for gpio1;bit3 for gpio3

static const unsigned int  GPIODebounce[4] = {0xffffffff,0xffffffff,0xffffffff,0xffffffff}; 
static const unsigned int  GPIODebounceTimer[4] = {24,24,24,24};// gpio debounce time 

//config GPMC

//config whitch cs to be usbed
#define CS_USE   0x7f  //bit map of CS use ,bit0 for CS0;bit1 for CS1;BIT6 for CS6

/*config cs bank baseAddr ,baseaddr should corresponds 
 to A29, A28, A27, A26, A25, and A24 */
static const unsigned int CS_BASEADDR[7]= {1<<24,2<<24,3<<24,4<<24,5<<24,6<<24,7<<24}; 
/*config cs size */
static const unsigned int CS_ADDRLINE_BIT[7] = {24,24,24,24,24,24,24};

#endif

