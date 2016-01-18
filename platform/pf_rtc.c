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
#include <time.h>
#include "delay.h"



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


static void RTCAM335XInit(unsigned int calendar, unsigned int time) {
    //RTCModuleClkConfig();
    moduleEnable(MODULE_ID_RTC);
    IntMasterIRQDisable();
    RTCWriteProtectDisable(SOC_RTC_0_REGS);
    RTC32KClkSourceSelect(SOC_RTC_0_REGS, RTC_INTERNAL_CLK_SRC_SELECT);
    RTC32KClkClockControl(SOC_RTC_0_REGS, RTC_32KCLK_ENABLE);
    RTCEnable(SOC_RTC_0_REGS);
    for (int i=0;i<10000;i++); //delay
    RTCCalendarSet(SOC_RTC_0_REGS, calendar);
    RTCTimeSet(SOC_RTC_0_REGS, time);
    RTCRun(SOC_RTC_0_REGS);
    RTCIntTimerEnable(SOC_RTC_0_REGS, RTC_INT_EVERY_SECOND);
    RTCWriteProtectEnable(SOC_RTC_0_REGS);
    IntMasterIRQEnable();
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

#define RTC_FLAG_EXTERN_NONE      -1
#define RTC_FLAG_EXTERN_POWERDROP -2
#define RTC_FLAG_INTERN_ERR       -3
uint32 RTCInit(void) {
    unsigned char hour, minute, second, year, month, day;
    uint32 r1, r2 = 1;
    r1 = Rx8025Init();
    if (r1 == 0) {
        r2 = RTC_FLAG_EXTERN_NONE;
    } else if (r1 == 2) {
        r2 = RTC_FLAG_EXTERN_POWERDROP;
    }
    if (r1 != 0) {
        Rx8025GetTime(&hour, &minute, &second);
        Rx8025GetCalendar(&year, &month, &day);
    }

    RTCAM335XInit(year << YEAR_SHIFT | month << MONTH_SHIFT | day << DAY_SHIFT,
                  hour << HOUR_SHIFT | minute << MINUTE_SHIFT | second << SECOND_SHIFT);
    return r2;
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
void RTCReadBcd(unsigned short *year, unsigned char *month,
                unsigned char *day, unsigned char *hour,
                unsigned char *minute, unsigned char *second) {
    unsigned int time = RTCTimeGet(SOC_RTC_0_REGS); //FIRST READ TIME;
    unsigned int can = RTCCalendarGet(SOC_RTC_0_REGS);
    *year = ((can & YEAR_MASK) >> YEAR_SHIFT) + 0x2000;
    *month = (can & MONTH_MASK) >> MONTH_SHIFT;
    *day = (can & DAY_MASK) >> DAY_SHIFT;
    *hour = (time & HOUR_MASK) >> HOUR_SHIFT;
    *minute = (time & MINUTE_MASK) >> MINUTE_SHIFT;
    *second = (time & SECOND_MASK) >> SECOND_SHIFT;
}

void RTCReadHex(unsigned short *year, unsigned char *month,
                unsigned char *day, unsigned char *hour,
                unsigned char *minute, unsigned char *second) {
    RTCReadBcd(year, month, day, hour, minute, second);
    *year = bcd2hex_4(*year);
    *month = bcd2hex_2(*month);
    *day = bcd2hex_2(*day);
    *hour = bcd2hex_2(*hour);
    *minute = bcd2hex_2(*minute);
    *second = bcd2hex_2(*second);
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
BOOL RTCSetBcd(unsigned short year, unsigned char month,
               unsigned char day, unsigned char hour,
               unsigned char minute, unsigned char second) {
    year = (unsigned char)year;
    unsigned int time = hour << HOUR_SHIFT | minute << MINUTE_SHIFT | second << SECOND_SHIFT;
    RTCWriteProtectDisable(SOC_RTC_0_REGS);
    IntMasterIRQDisable();
    RTCTimeSet(SOC_RTC_0_REGS, time);
    IntMasterIRQEnable();
    unsigned int can = day << DAY_SHIFT | month << MONTH_SHIFT | year << YEAR_SHIFT;
    IntMasterIRQDisable();
    RTCCalendarSet(SOC_RTC_0_REGS, can);
    IntMasterIRQEnable();
    RTCWriteProtectEnable(SOC_RTC_0_REGS);
    BOOL ret;
    ret = Rx8025SetCalendar(year, month, day);
    if (FALSE == ret) {
        return ret;
    }
    return Rx8025SetTime(hour, minute, second);
}

BOOL RTCSetHex(unsigned short year, unsigned char month,
               unsigned char day, unsigned char hour,
               unsigned char minute, unsigned char second) {
    ASSERT(month < 13);
    ASSERT(day < 32);
    ASSERT(hour < 25);
    ASSERT(minute < 61);
    ASSERT(second < 61);
    year = hex2bcd_byte(year % 100);
    month = hex2bcd_byte(month);
    day = hex2bcd_byte(day);
    hour = hex2bcd_byte(hour);
    minute = hex2bcd_byte(minute);
    second = hex2bcd_byte(second);
    return RTCSetBcd(year, month, day, hour, minute, second);
}


//! @}
/******************************* End of file ********************************/
