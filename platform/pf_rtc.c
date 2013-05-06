
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

/*
** This function enables the system L3 and system L4_WKUP clocks.
** This also enables the clocks for the RTC module.
*/

static void RTCModuleClkConfig(void)
{
    /* Configuring L3 Interface Clocks. */

    /* Writing to MODULEMODE field of CM_PER_L3_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) |=
          CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) &
           CM_PER_L3_CLKCTRL_MODULEMODE));

    /* Writing to MODULEMODE field of CM_PER_L3_INSTR_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) |=
          CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_PER_L3_INSTR_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_PER_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) |=
          CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_OCPWP_L3_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) |=
          CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_PER_L3S_CLKSTCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) |=
          CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_PER_L3S_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
           CM_PER_L3S_CLKSTCTRL_CLKTRCTRL));

    /* Checking fields for necessary values.  */

    /* Waiting for IDLEST field in CM_PER_L3_CLKCTRL register to be set to 0x0. */
    while((CM_PER_L3_CLKCTRL_IDLEST_FUNC << CM_PER_L3_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKCTRL) & CM_PER_L3_CLKCTRL_IDLEST));

    /*
    ** Waiting for IDLEST field in CM_PER_L3_INSTR_CLKCTRL register to attain the
    ** desired value.
    */
    while((CM_PER_L3_INSTR_CLKCTRL_IDLEST_FUNC <<
           CM_PER_L3_INSTR_CLKCTRL_IDLEST_SHIFT)!=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_INSTR_CLKCTRL) &
           CM_PER_L3_INSTR_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_GCLK field in CM_PER_L3_CLKSTCTRL register to
    ** attain the desired value.
    */
    while(CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3_CLKSTCTRL) &
           CM_PER_L3_CLKSTCTRL_CLKACTIVITY_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_OCPWP_L3_GCLK field in CM_PER_OCPWP_L3_CLKSTCTRL
    ** register to attain the desired value.
    */
    while(CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_OCPWP_L3_CLKSTCTRL) &
           CM_PER_OCPWP_L3_CLKSTCTRL_CLKACTIVITY_OCPWP_L3_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L3S_GCLK field in CM_PER_L3S_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK !=
          (HWREG(SOC_CM_PER_REGS + CM_PER_L3S_CLKSTCTRL) &
          CM_PER_L3S_CLKSTCTRL_CLKACTIVITY_L3S_GCLK));


    /* Configuring registers related to Wake-Up region. */

    /* Writing to MODULEMODE field of CM_WKUP_CONTROL_CLKCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) |=
          CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_WKUP_CONTROL_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_WKUP_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) |=
          CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKTRCTRL));

    /* Writing to CLKTRCTRL field of CM_L3_AON_CLKSTCTRL register. */
    HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) |=
          CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /*Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKTRCTRL));

    /* Verifying if the other bits are set to required settings. */

    /*
    ** Waiting for IDLEST field in CM_WKUP_CONTROL_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_CONTROL_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_CONTROL_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CONTROL_CLKCTRL) &
           CM_WKUP_CONTROL_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L3_AON_GCLK field in CM_L3_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L3_AON_CLKSTCTRL) &
           CM_WKUP_CM_L3_AON_CLKSTCTRL_CLKACTIVITY_L3_AON_GCLK));

    /*
    ** Waiting for IDLEST field in CM_WKUP_L4WKUP_CLKCTRL register to attain
    ** desired value.
    */
    while((CM_WKUP_L4WKUP_CLKCTRL_IDLEST_FUNC <<
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST_SHIFT) !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_L4WKUP_CLKCTRL) &
           CM_WKUP_L4WKUP_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_GCLK field in CM_WKUP_CLKSTCTRL register
    ** to attain desired value.
    */
    while(CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CLKSTCTRL) &
           CM_WKUP_CLKSTCTRL_CLKACTIVITY_L4_WKUP_GCLK));

    /*
    ** Waiting for CLKACTIVITY_L4_WKUP_AON_GCLK field in CM_L4_WKUP_AON_CLKSTCTRL
    ** register to attain desired value.
    */
    while(CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK !=
          (HWREG(SOC_CM_WKUP_REGS + CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL) &
           CM_WKUP_CM_L4_WKUP_AON_CLKSTCTRL_CLKACTIVITY_L4_WKUP_AON_GCLK));

    /* Performing Clock configurations of RTC. */

    /* Writing to MODULEMODE field of CM_RTC_RTC_CLKCTRL register. */
    HWREG(SOC_CM_RTC_REGS + CM_RTC_RTC_CLKCTRL) |=
          CM_RTC_RTC_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while(CM_RTC_RTC_CLKCTRL_MODULEMODE_ENABLE !=
          (HWREG(SOC_CM_RTC_REGS + CM_RTC_RTC_CLKCTRL) &
           CM_RTC_RTC_CLKCTRL_MODULEMODE));

    /* Writing to CLKTRCTRL field of CM_RTC_CLKSTCTRL register. */
    HWREG(SOC_CM_RTC_REGS + CM_RTC_CLKSTCTRL) |=
          CM_RTC_CLKSTCTRL_CLKTRCTRL_SW_WKUP;

    /* Waiting for CLKTRCTRL field to reflect the written value. */
    while(CM_RTC_CLKSTCTRL_CLKTRCTRL_SW_WKUP !=
          (HWREG(SOC_CM_RTC_REGS + CM_RTC_CLKSTCTRL) &
           CM_RTC_CLKSTCTRL_CLKTRCTRL));

    /* Checking the RTC registers for desired values being set. */

    /*
    ** Waiting for IDLEST field in CM_RTC_RTC_CLKCTRL register to attain the
    ** desired value.
    */
   while((CM_RTC_RTC_CLKCTRL_IDLEST_FUNC <<
           CM_RTC_RTC_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_RTC_REGS + CM_RTC_RTC_CLKCTRL) &
           CM_RTC_RTC_CLKCTRL_IDLEST));

    /*
    ** Waiting for CLKACTIVITY_L4_RTC_GCLK field in CM_RTC_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_RTC_CLKSTCTRL_CLKACTIVITY_L4_RTC_GCLK !=
          (HWREG(SOC_CM_RTC_REGS + CM_RTC_CLKSTCTRL) &
           CM_RTC_CLKSTCTRL_CLKACTIVITY_L4_RTC_GCLK));

    /*
    ** Waiting for CLKACTIVITY_RTC_32KCLK field in CM_RTC_CLKSTCTRL register
    ** to attain the desired value.
    */
    while(CM_RTC_CLKSTCTRL_CLKACTIVITY_RTC_32KCLK !=
          (HWREG(SOC_CM_RTC_REGS + CM_RTC_CLKSTCTRL) &
           CM_RTC_CLKSTCTRL_CLKACTIVITY_RTC_32KCLK));
}


static void RTCAM335XInit(unsigned int calendar,unsigned int time){
    RTCModuleClkConfig();   
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




void RTCInit(void){
    unsigned char hour,minute,second,year,month,day;
    Rx8025Init();
    Rx8025GetTime(&hour,&minute,&second);
    Rx8025GetCalendar(&year,&month,&day);
    RTCAM335XInit(year<<YEAR_SHIFT | month<<MONTH_SHIFT | day<<DAY_SHIFT,
                  hour<<HOUR_SHIFT | minute<< MINUTE_SHIFT | second<<SECOND_SHIFT);
}


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


/******************************* End of file ********************************/
