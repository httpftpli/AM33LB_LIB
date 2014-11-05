#ifndef __MODULE__H__
#define __MODULE__H__


#include "interrupt.h"
#include "pf_int.h"
#include "clock.h"




#define MODULE_ID_EMU       SYS_INT_EMUINT                         
#define MODULE_ID_COMMTX    SYS_INT_COMMTX                       
#define MODULE_ID_COMMRX    SYS_INT_COMMRX                        
#define MODULE_ID_BENCH     SYS_INT_BENCH                          
#define MODULE_ID_ELM_IRQ         SYS_INT_ELM_IRQ                        
#define MODULE_ID_SSM_WFI_IRQ     SYS_INT_SSM_WFI_IRQ            
#define MODULE_ID_SSM_IRQ         SYS_INT_SSM_IRQ                        
#define MODULE_ID_NMI             SYS_INT_NMI                            
#define MODULE_ID_SEC_EVNT        SYS_INT_SEC_EVNT                      
#define MODULE_ID_L3DEBUG  SYS_INT_L3DEBUG                        
#define MODULE_ID_L3APP    SYS_INT_L3APPINT                       
#define MODULE_ID_PRCM     SYS_INT_PRCMINT                        
#define MODULE_ID_EDMA    SYS_INT_EDMACOMPINT                    
#define MODULE_ID_EDMA2    SYS_INT_EDMAMPERR                      
#define MODULE_ID_EDMA_ERR    SYS_INT_EDMAERRINT                    
#define MODULE_ID_WDT0     SYS_INT_WDT0INT                        
#define MODULE_ID_ADCTSC   SYS_INT_ADC_TSC_GENINT                 
#define MODULE_ID_USBSS    SYS_INT_USBSSINT                       
#define MODULE_ID_USB0     SYS_INT_USB0                         
#define MODULE_ID_USB1     SYS_INT_USB1                          
#define MODULE_ID_PRUSS1_EVTOUT0    SYS_INT_PRUSS1_EVTOUT0                 
#define MODULE_ID_PRUSS1_EVTOUT1    SYS_INT_PRUSS1_EVTOUT1                
#define MODULE_ID_PRUSS1_EVTOUT2    SYS_INT_PRUSS1_EVTOUT2                
#define MODULE_ID_PRUSS1_EVTOUT3    SYS_INT_PRUSS1_EVTOUT3                 
#define MODULE_ID_PRUSS1_EVTOUT4    SYS_INT_PRUSS1_EVTOUT4               
#define MODULE_ID_PRUSS1_EVTOUT5    SYS_INT_PRUSS1_EVTOUT5                
#define MODULE_ID_PRUSS1_EVTOUT6    SYS_INT_PRUSS1_EVTOUT6               
#define MODULE_ID_PRUSS1_EVTOUT7    SYS_INT_PRUSS1_EVTOUT7                 
#define MODULE_ID_MMCSD1         SYS_INT_MMCSD1INT                     
#define MODULE_ID_MMCSD2         SYS_INT_MMCSD2INT                      
#define MODULE_ID_I2C2           SYS_INT_I2C2INT                        
#define MODULE_ID_eCAP0          SYS_INT_eCAP0INT                      
#define MODULE_ID_GPIO2      SYS_INT_GPIOINT2A                     
#define MODULE_ID_GPIO2_ddf      SYS_INT_GPIOINT2A                      
#define MODULE_ID_USBWAKEUP      SYS_INT_USBWAKEUP                      
#define MODULE_ID_LCDC           SYS_INT_LCDCINT                        
#define MODULE_ID_GFX            SYS_INT_GFXINT                        
#define MODULE_ID_2DHWA    SYS_INT_2DHWAINT                       
#define MODULE_ID_ePWM2    SYS_INT_ePWM2INT                       
#define MODULE_ID_3GPSW      SYS_INT_3PGSWRXTHR0                
#define MODULE_ID_3GPSW_1      SYS_INT_3PGSWRXINT0                   
#define MODULE_ID_3GPSW_2      SYS_INT_3PGSWTXINT0                   
#define MODULE_ID_3GPSW_3     SYS_INT_3PGSWMISC0                    
#define MODULE_ID_UART3    SYS_INT_UART3INT                      
#define MODULE_ID_UART4    SYS_INT_UART4INT                      
#define MODULE_ID_UART5    SYS_INT_UART5INT                      
#define MODULE_ID_eCAP1    SYS_INT_eCAP1INT                       
#define MODULE_ID_DCAN0    SYS_INT_DCAN0_INT0                     
#define MODULE_ID_DCAN0_1     SYS_INT_DCAN0_INT1                     
#define MODULE_ID_DCAN0_2     SYS_INT_DCAN0_PARITY                  
#define MODULE_ID_DCAN1_0     SYS_INT_DCAN1_INT0                  
#define MODULE_ID_DCAN1       SYS_INT_DCAN1_INT0                     
#define MODULE_ID_DCAN1_2     SYS_INT_DCAN1_PARITY                   
#define MODULE_ID_ePWM0_TZ       SYS_INT_ePWM0_TZINT                    
#define MODULE_ID_ePWM1_TZ       SYS_INT_ePWM1_TZINT                    
#define MODULE_ID_ePWM2_TZ       SYS_INT_ePWM2_TZINT                    
#define MODULE_ID_eCAP2       SYS_INT_eCAP2INT                       
#define MODULE_ID_GPIO3    SYS_INT_GPIOINT3A                      
#define MODULE_ID_GPIO3dfdd    SYS_INT_GPIOINT3B                      
#define MODULE_ID_MMCSD0      SYS_INT_MMCSD0INT                      
#define MODULE_ID_SPI0        SYS_INT_SPI0INT                       
#define MODULE_ID_TIMER0      SYS_INT_TINT0                          
#define MODULE_ID_TIMER1      SYS_INT_TINT1_1MS                     
#define MODULE_ID_TIMER2      SYS_INT_TINT2                        
#define MODULE_ID_TIMER3      SYS_INT_TINT3                        
#define MODULE_ID_I2C0        SYS_INT_I2C0INT                        
#define MODULE_ID_I2C1        SYS_INT_I2C1INT                      
#define MODULE_ID_UART0       SYS_INT_UART0INT                       
#define MODULE_ID_UART1       SYS_INT_UART1INT                      
#define MODULE_ID_UART2       SYS_INT_UART2INT                       
#define MODULE_ID_RTC         SYS_INT_RTCINT                       
#define MODULE_ID_RTC_1       SYS_INT_RTCALARMINT                    
#define MODULE_ID_MB          SYS_INT_MBINT0                         
#define MODULE_ID_M3          SYS_INT_M3_TXEV                      
#define MODULE_ID_eQEP0       SYS_INT_eQEP0INT                     
#define MODULE_ID_546         SYS_INT_MCATXINT0                     
#define MODULE_ID_JDD         SYS_INT_MCARXINT0                      
#define MODULE_ID_FDG         SYS_INT_MCATXINT1                      
#define MODULE_ID_PP          SYS_INT_MCARXINT1                      
#define MODULE_ID_ePWM0       SYS_INT_ePWM0INT                       
#define MODULE_ID_ePWM1       SYS_INT_ePWM1INT                       
#define MODULE_ID_eQEP1       SYS_INT_eQEP1INT                      
#define MODULE_ID_eQEP2       SYS_INT_eQEP2INT                       
#define MODULE_ID_DFDD54      SYS_INT_DMA_INTR_PIN2                  
#define MODULE_ID_8RFF        SYS_INT_WDT1INT                       
#define MODULE_ID_TIMER4      SYS_INT_TINT4                          
#define MODULE_ID_TIMER5      SYS_INT_TINT5                          
#define MODULE_ID_TIMER6      SYS_INT_TINT6                          
#define MODULE_ID_TIMER7      SYS_INT_TINT7                          
#define MODULE_ID_GPIO0       SYS_INT_GPIOINT0A                     
#define MODULE_ID_GPIO0_2     SYS_INT_GPIOINT0B                      
#define MODULE_ID_GPIO1       SYS_INT_GPIOINT1A                     
#define MODULE_ID_GPIO1_2     SYS_INT_GPIOINT1B                     
#define MODULE_ID_GPMC        SYS_INT_GPMCINT                       
#define MODULE_ID_DDR         SYS_INT_DDRERR0                        
#define MODULE_IDDDFG4        SYS_INT_AES0_IRQ_S                     
#define MODULE_ID4454         SYS_INT_AES0_IRQ_P                    
#define MODULE_IDJKD          SYS_INT_SHA_IRQ_S                     
#define MODULE_ID3FD          SYS_INT_SHA_IRQ_P                      
#define MODULE_ID9KG          SYS_INT_FPKA_SINTREQUEST_S             
#define MODULE_ID4GFJHDF      SYS_INT_RNG_IRQ                        
#define MODULE_IDDFDFDFG      SYS_INT_TCERRINT0                     
#define MODULE_IDKJFDSF       SYS_INT_TCERRINT1                      
#define MODULE_ID56Y          SYS_INT_TCERRINT2                      
#define MODULE_ID565F         SYS_INT_SMRFLX_Sabertooth              
#define MODULE_ID56D5F         SYS_INT_SMRFLX_Core                    
#define MODULE_ID565GHG       SYS_INT_DMA_INTR_PIN0                  
#define MODULE_ID4546         SYS_INT_DMA_INTR_PIN1                 
#define MODULE_ID_SPI1        SYS_INT_SPI1INT                       



typedef struct _module_ {
  unsigned int baseAddr;
  unsigned short index;
  unsigned short INTNum[3];
  unsigned short INTPriority[3];
  ISR   isr[3];
  ModuleClock *moduleClk;
} MODULE;


extern MODULE modulelist[128];

extern void moduleEnable(unsigned int moduleId);
extern void moduleIntConfigure(unsigned int moduleID);





#endif
