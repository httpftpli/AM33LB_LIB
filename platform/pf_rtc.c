/**
 *  \file   pf_rtc.c
 *
 *  \brief
 *  \author  李飞亮  
 *  \addtogroup RTC
 *  \# include "pf_rtc.h"
 *  RTC分内置RTC和外置RTC,上点复位时从外置RTC读取时钟，并初始化内置RTC,运
 *  行时只从内置RTC读取 
 *  
 *  @{
 *   
 */



#include "hw_cm_wkup.h"
#include "soc_AM335x.h"
#include "hw_cm_per.h"
#include "hw_cm_rtc.h"
#include "hw_types.h"
#include "rtc.h"
#include "debug.h"
#include "misc.h"
#include "pf_rx8025.h"
#include "pf_timertick.h"
#include "module.h"


void isr_RTC(unsigned int intnum) {
   UNUSED(intnum);
   unsigned int calendarValue;
   unsigned int timeValue;
   timeValue = RTCTimeGet(SOC_RTC_0_REGS);
   calendarValue = RTCCalendarGet(SOC_RTC_0_REGS);
   mdDebug("calendar:");
   mdDebugNum(calendarValue);
   mdDebug("\r\n");
   mdDebug("time:");
   mdDebugNum(timeValue);
   mdDebug("\r\n:");
}


static void RTCAM335XInit(unsigned int calendar,unsigned int time){
    //RTCModuleClkConfig();   
    moduleEnable(MODULE_ID_RTC);
    RTCWriteProtectDisable(SOC_RTC_0_REGS);
    RTC32KClkSourceSelect(SOC_RTC_0_REGS, RTC_INTERNAL_CLK_SRC_SELECT);
    RTC32KClkClockControl(SOC_RTC_0_REGS, RTC_32KCLK_ENABLE);
    RTCEnable(SOC_RTC_0_REGS);
    RTCCalendarSet(SOC_RTC_0_REGS,  calendar);
    RTCTimeSet(SOC_RTC_0_REGS, time);      
    RTCRun(SOC_RTC_0_REGS); 
    RTCIntTimerEnable(SOC_RTC_0_REGS, RTC_INT_EVERY_SECOND);
    RTCWriteProtectEnable(SOC_RTC_0_REGS);
 }




/**
 * @brief RTC初始化 
 * 读出RX8025，初始化内置RTC 
 * @return 
 * - FAIL 
 * - SUCCESS 
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre 
 * 初始化I2C0 
 * @see 
 */
BOOL RTCInit(void){
    unsigned char hour,minute,second,year,month,day;
    BOOL r;
    Rx8025Init();
    r = Rx8025GetTime(&hour,&minute,&second);
    if (FAIL==r) {
       return r;
    }
    r = Rx8025GetCalendar(&year,&month,&day);
    if (FAIL==r) {
       return r;
    }
    RTCAM335XInit(year<<YEAR_SHIFT | month<<MONTH_SHIFT | day<<DAY_SHIFT,
                  hour<<HOUR_SHIFT | minute<< MINUTE_SHIFT | second<<SECOND_SHIFT);
    return SUCCESS;
}


/**
 * @brief 读取RTC 
 *  从内置RTC读取时钟和日历
 * @param [out] year
 * @param [out] month           
 * @param [out] day
 * @param [out] hour
 * @param [out] minute
 * @param [out] second
 * @return NONE          
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
void RTCRead(unsigned char *year, unsigned char *month,
             unsigned char  *day, unsigned char *hour,
             unsigned char *minute, unsigned char *second) {
   unsigned int time = RTCTimeGet(SOC_RTC_0_REGS); //FIRST READ TIME;
   unsigned int can = RTCCalendarGet(SOC_RTC_0_REGS);
   *year = (can & YEAR_MASK) >> YEAR_SHIFT;
   *month = (can & MONTH_MASK) >> MONTH_SHIFT;
   *day = (can & DAY_MASK) >> DAY_SHIFT;
   *hour = (time & HOUR_MASK) >> HOUR_SHIFT;
   *minute = (time & MINUTE_MASK) >> MINUTE_SHIFT;
   *second = (time & SECOND_MASK) >> SECOND_SHIFT;
}

/**
 * @brief 设置RTC ，并写入外置RTC 
 * @param [out] year
 * @param [out] month           
 * @param [out] day
 * @param [out] hour
 * @param [out] minute
 * @param [out] second
 * @return BOOL 
 * - FALSE -- 
 *   外部RTC写入失败，内部RTCXI设置成功\n\r
 * - TRUE --成功 
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
BOOL RTCSet(unsigned char year, unsigned char month,
             unsigned char  day, unsigned char hour,
             unsigned char minute, unsigned char second){
    unsigned int time = hour<<HOUR_SHIFT |minute<<MINUTE_SHIFT | second<<SECOND_SHIFT;
    RTCTimeSet(SOC_RTC_0_REGS,time);
    unsigned int can = day<<DAY_SHIFT|month<<MONTH_SHIFT|year<<YEAR_SHIFT;
    RTCCalendarSet(SOC_RTC_0_REGS,can);
    RTCRun(SOC_RTC_0_REGS);
    BOOL ret;
    ret = Rx8025SetCalendar(year,month,day);
    if (FALSE == ret ) {
       return ret;
    }
    return Rx8025SetTime(hour, minute, second); 
}


//! @}
/******************************* End of file ********************************/
