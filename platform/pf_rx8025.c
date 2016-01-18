/**
 *  \file   pf_rx8025.c
 *
 *  \brief
 *  \author  李飞亮
 *  \addtogroup RTC
 *  @brief RTC
 *
 *  #include "pf_rx8025.h"
 *  @{
 *
 */


#include "pf_hsi2c.h"
#include "soc_am335x.h"
#include "misc.h"
#include "type.h"
#include "string.h"


#define HSI2C_Rx8025_MEMBASE  SOC_I2C_0_REGS
#define RX8025_SLAVE_ADDR       0x32


typedef struct {
    BOOL (*write)(unsigned char addr, unsigned char data);
    unsigned char (*read)(unsigned char addr);
    BOOL (*readEx)(unsigned char addr, unsigned char *buf,
                   unsigned char szRead);
}RX8025_OPERATION;


static RX8025_OPERATION rx8025op;

static BOOL Rx8025SAWrite(unsigned char addr, unsigned char data) {
    unsigned char buf[2];
    buf[0] = addr << 4;
    buf[1] = data;
    return I2CMasterWrite(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR, buf, 2);
}

static BOOL Rx8025TWrite(unsigned char addr, unsigned char data) {
    unsigned char buf[2];
    buf[0] = addr;
    buf[1] = data;
    return I2CMasterWrite(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR, buf, 2);
}



static unsigned char Rx8025SARead(unsigned char addr) {
    unsigned char data;
    addr <<= 4;
    if (I2CMasterReadEx(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR,
                        &addr, 1, &data, 1) == SUCCESS) {
        return data;
    } else {
        return -1;
    }
}


static unsigned char Rx8025TRead(unsigned char addr) {
    unsigned char data;
    if (I2CMasterReadEx(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR,
                        &addr, 1, &data, 1) == SUCCESS) {
        return data;
    } else {
        return -1;
    }
}

static BOOL Rx8025SAReadEx(unsigned char addr, unsigned char *buf,
                           unsigned char szRead) {
    addr <<= 4;
    return I2CMasterReadEx(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR,
                           &addr, 1, buf, szRead);
}

static BOOL Rx8025TReadEx(unsigned char addr, unsigned char *buf,
                          unsigned char szRead) {
    return I2CMasterReadEx(HSI2C_Rx8025_MEMBASE, RX8025_SLAVE_ADDR,
                           &addr, 1, buf, szRead);
}


static uint32 Rx8025Detect(void) {
    unsigned char buf[16];
    unsigned char buftempsa[4];
    unsigned char buftempt[4];
    for (int i = 0; i < 2; i++) {
        bool r = Rx8025SAReadEx(0, buf, 16);
        if (r == false) {
            return 0;
        }
        Rx8025SAReadEx(3, buftempsa, 4);
        Rx8025TReadEx(3, buftempt, 4);
        if (memcmp(buftempsa, buftempt, 4) == 0) {
            return 0;
        }
        char difsa = memcmp(buftempsa, &buf[3], 4);
        char dift = memcmp(buftempt, &buf[3], 4);
        if (difsa == 0 && dift != 0) {
            return 1;
        } else if (difsa != 0 && dift == 0) {
            return 2;
        }
    }
    return 0;
}


/**
 * @brief RX8025初始化
 * @return NONE
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
uint32 Rx8025Init() {
    uint32 r = Rx8025Detect();
    uint32 r2 = 1;
    uint8 data;
    if (r == 0) {
        return 0;
    }
    if (r == 1) {
        rx8025op.write = Rx8025SAWrite;
        rx8025op.read = Rx8025SARead;
        rx8025op.readEx = Rx8025SAReadEx;
        data = rx8025op.read(0x0f);
        if (data == 0xff) {
            return 0;
        } else if (data & 0x50) { //have power off
            r2 = 2;
            if (rx8025op.write(0x0e, 0x30) == false) return 0;
            if (rx8025op.write(0x0f, 0x08) == false) return 0;
        }
    } else if (r == 2) {
        rx8025op.write = Rx8025TWrite;
        rx8025op.read = Rx8025TRead;
        rx8025op.readEx = Rx8025TReadEx;
        data = rx8025op.read(0x0e);
        if (data == 0xff) {
            return 0;
        } else if (data & 0x02) {//have power off
            r2 = 2;
            rx8025op.write(0x0e,0);
        }
    }
    return r2;
}




/**
 * @brief 设置Rx8025时间
 * @param [in] hour
 * @param [in] minute
 * @param [in] second
 * @return
 *  - SUCCESS 成功 \n
 *  - FAUL 失败 \n
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL Rx8025SetTime(unsigned char hour,
                   unsigned char minute, unsigned char second) {
    if (rx8025op.write(0, second) == FAIL) return FAIL;
    if (rx8025op.write(1, minute) == FAIL) return FAIL;
    if (rx8025op.write(2, hour) == FAIL) return FAIL;
    return SUCCESS;
}


/**
 * @brief 读取Rx8025时间
 * @param [out] hour
 * @param [out] minute
 * @param [out] second
 * @return
 *  - SUCCESS 成功 \n
 *  - FAUL 失败 \n
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL Rx8025GetTime(unsigned char *hour, unsigned char *minute, unsigned char *second) {
    unsigned char buf[3];
    if (rx8025op.readEx(0, buf, 3) == FAIL) return FAIL;
    *hour = buf[2];
    *minute = buf[1];
    *second = buf[0];
    return SUCCESS;
}



/**
 * @brief 设置Rx8025日期
 * @param [in] year
 * @param [in] month
 * @param [in] day
 * @return
 *  - SUCCESS 成功 \n
 *  - FAUL 失败 \n
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL Rx8025SetCalendar(unsigned char year,
                       unsigned char month, unsigned char day) {
    if (rx8025op.write(6, year) == FAIL) return FAIL;
    if (rx8025op.write(5, month) == FAIL) return FAIL;
    if (rx8025op.write(4, day) == FAIL) return FAIL;
    return SUCCESS;
}

/**
 * @brief 读取Rx8025日期
 * @param [out] year
 * @param [out] month
 * @param [out] day
 * @return
 *  - SUCCESS 成功 \n
 *  - FAUL 失败 \n
 * @date    2013/5/7
 * @note
 * @code
 * @endcode
 * @pre
 * @see
 */
BOOL Rx8025GetCalendar(unsigned char *year,
                       unsigned char *month, unsigned char *day) {
    unsigned char buf[3];
    if (rx8025op.readEx(4, buf, 3) == FAIL) return FAIL;
    *year = buf[2];
    *month = buf[1];
    *day = buf[0];
    return SUCCESS;
}

//! @}

