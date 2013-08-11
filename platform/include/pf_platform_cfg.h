#ifndef __PF_PLATFORM_CFG_H_
#define __PF_PLATFORM_CFG_H_

#include "soc_AM335x.h"
#include "module.h"
#include "platform_cfg.h" 

//set witch usb instance use for udist : 0 or 1
#ifndef USB_INSTANCE_FOR_USBDISK
#define USB_INSTANCE_FOR_USBDISK   0
#endif
//define USBDISK logic volume
#define FatFS_Drive_Index          2
//set if use use dma or pull
#define USB_USE_CPPI41DMA          0


//bellow config interrupt priority
#ifndef INT_PRIORITY_MAX
#define INT_PRIORITY_MAX          (0x7f)
#endif
#ifndef INT_PRIORITY_MIN
#define INT_PRIORITY_MIN           (0)
#endif

#ifndef INT_PRIORITY_USER_NORMAL
#define INT_PRIORITY_USER_NORMAL  (0x30)
#endif

#ifndef INT_PRIORITY_USER_PRI
#define INT_PRIORITY_USER_PRI     (0x10)
#endif

#ifndef INT_PRIORITY_DMA_COMP
#define INT_PRIORITY_DMA_COMP     (INT_PRIORITY_USER_NORMAL - 3)
#endif

#ifndef INT_PRIORITY_DMA_ERROR
#define INT_PRIORITY_DMA_ERROR    (INT_PRIORITY_USER_NORMAL - 4)
#endif
 
#ifndef INT_PRIORITY_MMCSD0
#define INT_PRIORITY_MMCSD0       (INT_PRIORITY_USER_NORMAL-2)
#endif

#ifndef INT_PRIORITY_MMCSD1
#define INT_PRIORITY_MMCSD1       (INT_PRIORITY_USER_NORMAL-1)
#endif

#ifndef INT_PRIORITY_ADCTSC
#define INT_PRIORITY_ADCTSC       (INT_PRIORITY_USER_NORMAL-1)
#endif

#ifndef INT_PRIORITY_RTC
#define INT_PRIORITY_RTC          (INT_PRIORITY_USER_NORMAL-5)
#endif

#ifndef INT_PRIORITY_DCAN0_LINE0
#define INT_PRIORITY_DCAN0_LINE0  (INT_PRIORITY_USER_NORMAL-6)
#endif

#ifndef INT_PRIORITY_DCAN1_LINE0
#define INT_PRIORITY_DCAN1_LINE0  (INT_PRIORITY_USER_NORMAL-6)
#endif

#ifndef INT_PRIORITY_TIMER0
#define INT_PRIORITY_TIMER0       (INT_PRIORITY_USER_NORMAL-7)
#endif

#ifndef INT_PRIORITY_TIMER1
#define INT_PRIORITY_TIMER1       (INT_PRIORITY_USER_NORMAL-7)
#endif

#ifndef INT_PRIORITY_TIMER2
#define INT_PRIORITY_TIMER2       (INT_PRIORITY_USER_NORMAL-7)
#endif

#ifndef INT_PRIORITY_TIMER3
#define INT_PRIORITY_TIMER3       (INT_PRIORITY_USER_NORMAL-7)
#endif

#ifndef INT_PRIORITY_TIMER4
#define INT_PRIORITY_TIMER4       (INT_PRIORITY_USER_NORMAL-7)
#endif

#ifndef INT_PRIORITY_TIMER5
#define INT_PRIORITY_TIMER5       (INT_PRIORITY_USER_NORMAL-7)
#endif

#ifndef INT_PRIORITY_TIMER6
#define INT_PRIORITY_TIMER6       (INT_PRIORITY_USER_NORMAL-7)
#endif

#ifndef INT_PRIORITY_TIMER7
#define INT_PRIORITY_TIMER7       (INT_PRIORITY_USER_NORMAL-7)
#endif

#ifndef INT_PRIORITY_EQEP2
#define INT_PRIORITY_EQEP2        (INT_PRIORITY_USER_NORMAL-8)
#endif

#ifndef INT_PRIORITY_USB0
#define INT_PRIORITY_USB0         (INT_PRIORITY_USER_NORMAL-9)
#endif

#ifndef INT_PRIORITY_USB1
#define INT_PRIORITY_USB1         (INT_PRIORITY_USER_NORMAL-10)
#endif


#if USB_USE_CPPI41DMA
#ifndef INT_PRIORITY_USB_DMA
#define INT_PRIORITY_USB_DMA      (INT_PRIORITY_USER_NORMAL-11)
#endif
#endif

#ifndef INT_PRIORITY_GPIO0
#define INT_PRIORITY_GPIO0        (INT_PRIORITY_USER_NORMAL-12)
#endif

#ifndef INT_PRIORITY_GPIO1
#define INT_PRIORITY_GPIO1        (INT_PRIORITY_USER_NORMAL-13)
#endif

#ifndef INT_PRIORITY_GPIO2
#define INT_PRIORITY_GPIO2        (INT_PRIORITY_USER_NORMAL-14)
#endif

#ifndef INT_PRIORITY_GPIO3
#define INT_PRIORITY_GPIO3        (INT_PRIORITY_USER_NORMAL-15)
#endif

#ifndef INT_PRIORITY_LCD
#define INT_PRIORITY_LCD          (INT_PRIORITY_USER_NORMAL-16)
#endif

#ifndef INT_PRIORITY_eCAP0
#define INT_PRIORITY_eCAP0        (INT_PRIORITY_USER_NORMAL-16)
#endif

#ifndef INT_PRIORITY_eCAP1
#define INT_PRIORITY_eCAP1        (INT_PRIORITY_USER_NORMAL-16)
#endif

#ifndef INT_PRIORITY_eCAP2
#define INT_PRIORITY_eCAP2        (INT_PRIORITY_USER_NORMAL-16)
#endif

#ifndef INT_PRIORITY_UART1
#define INT_PRIORITY_UART1        (INT_PRIORITY_USER_NORMAL-16)
#endif

#ifndef INT_PRIORITY_UART2
#define INT_PRIORITY_UART2        (INT_PRIORITY_USER_NORMAL-16)
#endif

#ifndef INT_PRIORITY_UART3
#define INT_PRIORITY_UART3        (INT_PRIORITY_USER_NORMAL-16)
#endif

#ifndef INT_PRIORITY_UART4
#define INT_PRIORITY_UART4        (INT_PRIORITY_USER_NORMAL-16)
#endif

#ifndef INT_PRIORITY_UART5
#define INT_PRIORITY_UART5        (INT_PRIORITY_USER_NORMAL-16)
#endif


//config FRAMEBUFFER memory 
#ifndef FRAMEBUFFER_SIZE
#define FRAMEBUFFER_SIZE    3*1024*1024  //must 1M bytpe algined 
#endif

#ifndef DDR_RAM_SIZE 
#define DDR_RAM_SIZE  128*1024*1024
#endif


//config GPMC

//config whitch cs to be usbed
//bit map of CS use ,bit0 for CS0;bit1 for CS1;BIT6 for CS6
#ifndef CS_USE
#define CS_USE   0x7f 
#endif


//config LCD backlight control gpio
#ifndef GPIO_LCDBACKLIGHT_MODULE
#define GPIO_LCDBACKLIGHT_MODULE   MODULE_ID_GPIO0
#endif
#ifndef GPIO_LCDBACKLIGHT_PIN
#define GPIO_LCDBACKLIGHT_PIN    30
#endif
#ifndef LCD_BACKLIG_ON
#define LCD_BACKLIG_ON           1  
#endif
#ifndef LCD_BACKLIG_OFF
#define LCD_BACKLIG_OFF          0
#endif

//config LCD backlight control gpio
#ifndef GPIO_BEEP_MODULE
#define GPIO_BEEP_MODULE   MODULE_ID_GPIO2
#endif
#ifndef GPIO_BEEP_PIN
#define GPIO_BEEP_PIN    20
#endif
#ifndef LCD_BEEP_ON
#define LCD_BEEP_ON           1  
#endif
#ifndef LCD_BEEP_OFF
#define LCD_BEEP_OFF          0
#endif



#define TFT_AT080TN52    0
#define TFT_AT070TN92    1
#define TFT_LSA40AT9001  2

//select TFT panel 
#ifndef TFT_PANEL
#define TFT_PANEL    TFT_LSA40AT9001  
#endif



#ifndef ASCII_FONT16_IN_SOURCE
#define ASCII_FONT16_IN_SOURCE     1
#endif 

#ifndef ASCII_FONT16_IN_SOURCE
#define ASCII_FONT20_IN_SOURCE     1
#endif 


#endif

