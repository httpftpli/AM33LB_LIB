#include "pf_hsi2c.h"
#include "soc_am335x.h"
#include "misc.h"
#include "type.h"


#define HSI2C_Rx8025_MEMBASE  SOC_I2C_1_REGS
#define RX8025_SLAVE_ADDR       0x32


static BOOL Rx8025Write(unsigned char addr, unsigned char data) {
   unsigned char buf[2];
   buf[0] = addr<<4;
   buf[1] = data;
   return I2CMasterWrite(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR, buf, 2);
}

static unsigned char Rx8205Read(unsigned char addr) {
   unsigned char data;
   addr <<=4;
   if (I2CMasterReadEx(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR,
                       &addr, 1, &data, 1) == SUCCESS) {
      return data;
   } else {
      return -1;
   }
}

static BOOL Rx8205ReadEx(unsigned char addr, unsigned char *buf,
                         unsigned char szRead) {
   addr <<=4;
   return I2CMasterReadEx(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR,
                          &addr, 1, buf, szRead);
}


BOOL Rx8025Init() {
   if (Rx8025Write(0x0e, 0x30) == FAIL) return FAIL;
   if (Rx8025Write(0x0f, 0x08) == FAIL)  return FAIL;
   return SUCCESS;
}


BOOL Rx8025SetTime(unsigned char hour,
                   unsigned char minute, unsigned char second) { 
   hour = hex2bcd_byte(hour);
   minute = hex2bcd_byte(minute);
   second = hex2bcd_byte(second);
   if (Rx8025Write(0, second) == FAIL) return FAIL;
   if (Rx8025Write(1, minute) == FAIL) return FAIL;
   if (Rx8025Write(2, hour) == FAIL) return FAIL;
   return SUCCESS;
}

BOOL Rx8025GetTime(unsigned char *hour,
                   unsigned char *minute, unsigned char *second) {
   unsigned char buf[3];
   if (Rx8205ReadEx(0, buf, 3) == FAIL) return FAIL;
   *hour = bcd2hex_2(buf[2]);
   *minute = bcd2hex_2(buf[1]);
   *second = bcd2hex_2(buf[0]);
   return SUCCESS;
}

BOOL Rx8025SetDate(unsigned short year,
                   unsigned char month, unsigned char day) {
   year = hex2bcd_byte(year-2000);
   month = hex2bcd_byte(month);
   day = hex2bcd_byte(day);
   if (Rx8025Write(6, year) == FAIL) return FAIL;
   if (Rx8025Write(5, month) == FAIL) return FAIL;
   if (Rx8025Write(4, day) == FAIL) return FAIL;
   return SUCCESS;
}

BOOL Rx8025GetDate(unsigned short *year,
                   unsigned char *month, unsigned char *day) {
   unsigned char buf[3];
   if (Rx8205ReadEx(4, buf, 3) == FAIL) return FAIL;
   *year = bcd2hex_2(buf[2])+2000;
   *month = bcd2hex_2(buf[1]);
   *day = bcd2hex_2(buf[0]);
   return SUCCESS;
}

