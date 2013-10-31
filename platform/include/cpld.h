
#ifndef __CPLD__H__
#define __CPLD__H__

#include "pf_platform_cfg.h"


#define GPMC(A) (*(volatile unsigned short *)(CS_BASEADDR[0]+(A)))

#define	ADDR_SF_PWM	0x0000
#define	ADDR_SF_EN	0x0002

#define	ADDR_LIGHT_GREEN	0x0010
#define	ADDR_LGHT_YELLOW	0x0012
#define	ADDR_LIGHT_RED	0x0014
#define	ADDR_DAY_LIGHT_LAMP	0x0016

#define	ADDR_SHOCK_PLUS_FRONT	0x0020
#define	ADDR_SHOCK_PLUS_BACK	0x0022

#define	ADDR_SONGSHA_R	0x0030
#define	ADDR_SONGSHA_L	0x0032
#define	ADDR_LOU_LA	0x0034

#define	ADDR_LJD_CLK	0x0040
#define	ADDR_LJD_REF	0x0042
#define	ADDR_LJD_DIR	0x0044

#define	ADDR_LJZ1_CLK	0x0048
#define	ADDR_LJZ1_REF	0x004A
#define	ADDR_LJZ1_DIR	0x004C

#define	ADDR_LJZ2_CLK	0x0050
#define	ADDR_LJZ2_REF	0x0052
#define	ADDR_LJZ2_DIR	0x0054

#define	ADDR_RJD_CLK	0x0058
#define	ADDR_RJD_REF	0x005A
#define	ADDR_RJD_DIR	0x005C

#define	ADDR_RJZ1_CLK	0x0060
#define	ADDR_RJZ1_REF	0x0062
#define	ADDR_RJZ1_DIR	0x0064

#define	ADDR_RJZ2_CLK	0x0068
#define	ADDR_RJZ2_REF	0x006A
#define	ADDR_RJZ2_DIR	0x006C

#define	ADDR_QD_UP	0x0070
#define	ADDR_QD_DOWM	0x0072
#define	ADDR_QD_SC	0x0074
#define	ADDR_NEEDLE_CW	0x0076
#define	ADDR_NEEDLE_CCW	0x0078

#define	ADDR_LM0_INPUT	0x0080
#define	ADDR_ALM1_INPUT	0x0082
#define	ADDR_ALL2_INPUT	0x0084

#define	ADDR_YARN_OUT_CRT	0x0090



#define	SF_PWM	GPMC(ADDR_SF_PWM)
#define	SF_EN	GPMC(ADDR_SF_EN)

#define	LIGHT_GREEN	GPMC(ADDR_LIGHT_GREEN)
#define	LGHT_YELLOW	GPMC(ADDR_LGHT_YELLOW)
#define LIGHT_RED	GPMC(ADDR_LIGHT_RED)
#define	DAY_LIGHT_LAMP	GPMC(ADDR_DAY_LIGHT_LAMP)

#define	SHOCK_PLUS_FRONT	GPMC(ADDR_SHOCK_PLUS_FRONT)
#define	SHOCK_PLUS_BACK	GPMC(ADDR_SHOCK_PLUS_BACK)

#define	SONGSHA_R	GPMC(ADDR_SONGSHA_R)
#define	SONGSHA_L	GPMC(ADDR_SONGSHA_L)
#define	LOU_LA	GPMC(ADDR_LOU_LA)

#define	LJD_CLK	GPMC(ADDR_LJD_CLK)
#define	LJD_REF	GPMC(ADDR_LJD_REF)
#define	LJD_DIR	GPMC(ADDR_LJD_DIR)

#define	LJZ1_CLK	GPMC(ADDR_LJZ1_CLK)
#define	LJZ1_REF	GPMC(ADDR_LJZ1_REF)
#define	LJZ1_DIR	GPMC(ADDR_LJZ1_DIR)

#define	LJZ2_CLK	GPMC(ADDR_LJZ2_CLK)
#define	LJZ2_REF	GPMC(ADDR_LJZ2_REF)
#define	LJZ2_DIR	GPMC(ADDR_LJZ2_DIR)
#define	RJD_CLK	GPMC(ADDR_RJD_CLK)
#define	RJD_REF	GPMC(ADDR_RJD_REF)
#define	RJD_DIR	GPMC(ADDR_RJD_DIR)

#define	RJZ1_CLK	GPMC(ADDR_RJZ1_CLK)
#define	RJZ1_REF	GPMC(ADDR_RJZ1_REF)
#define	RJZ1_DIR	GPMC(ADDR_RJZ1_DIR)

#define	RJZ2_CLK	GPMC(ADDR_RJZ2_CLK)
#define	RJZ2_REF	GPMC(ADDR_RJZ2_REF)
#define	RJZ2_DIR	GPMC(ADDR_RJZ2_DIR)

#define	QD_UP	GPMC(ADDR_QD_UP)
#define	QD_DOWM	GPMC(ADDR_QD_DOWM)
#define	QD_SC	GPMC(ADDR_QD_SC)
#define	NEEDLE_CW	GPMC(ADDR_NEEDLE_CW)
#define	NEEDLE_CCW	GPMC(ADDR_NEEDLE_CCW)

#define	LM0_INPUT	GPMC(ADDR_LM0_INPUT)
#define	ALM1_INPUT  GPMC(ADDR_ALM1_INPUT)
#define	ALL2_INPUT	GPMC(ADDR_ALL2_INPUT)
#define	YARN_OUT_CRT	GPMC(ADDR_YARN_OUT_CRT)



#endif 
