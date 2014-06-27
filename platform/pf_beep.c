

#include "pf_gpio.h"
#include "pf_platform_cfg.h"
#include "pf_mux.h"

volatile unsigned int  beepdeuation;


void beepInit(){
   /*GPIOInit(GPIO_BEEP_MODULE, 0,0);
   unsigned int addr = modulelist[GPIO_BEEP_MODULE].baseAddr;
   GPIODirModeSet(addr,GPIO_BEEP_PIN,GPIO_DIR_OUTPUT); */
}


void beepOn(unsigned int duration){
   /*unsigned int addr = modulelist[GPIO_BEEP_MODULE].baseAddr;
   GPIOPinWrite(addr,GPIO_BEEP_PIN,LCD_BEEP_ON );
   beepdeuation = duration;*/
    unsigned char buf[] = {0xbb,0x03,0x00,0x00,0x00,0x00,0x00,0x0d};
    *(unsigned short *)(buf+2) = duration;
    while(!UARTSendNoBlock(UART_LCDBACKLIGHT_MODULE,buf,sizeof buf));
}



void beepOff(){
   /*unsigned int addr = modulelist[GPIO_BEEP_MODULE].baseAddr;
   GPIOPinWrite(addr,GPIO_BEEP_PIN, LCD_BEEP_OFF);
   beepdeuation = 0;*/
}
