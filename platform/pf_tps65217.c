#include "pf_hsi2c.h"
#include "pf_tps65217.h"
#include "soc_AM335X.h"
#include "hw_tps65217.h"



unsigned char TPS65217RegRead(unsigned char regOffset) {
    unsigned char val;
    I2CMasterReadEx(SOC_I2C_0_REGS, 0x24, &regOffset, 1, &val, 1);
    return val;
}

/**
 *  \brief            - Generic function that can write a TPS65217 PMIC
 *                      register or bit field regardless of protection
 *                      level.
 *
 * \param regOffset:  - Register address to write.
 *
 * \param dest_val    - Value to write.
 *
 * \param mask        - Bit mask (8 bits) to be applied.  Function will only
 *                      change bits that are set in the bit mask.
 *
 * \return:            BOOL.
 */
BOOL TPS65217RegWrite(unsigned char regOffset,
                      unsigned char dest_val, unsigned char mask) {
    unsigned char read_val;
    unsigned char xor_reg;
    unsigned char pswreg = 0x55;
    unsigned char port_level;
    unsigned char i2cwritebuf[2];
    unsigned char i2cregaddr;
    if (regOffset <= 12) {
        port_level = PROT_LEVEL_NONE;
    } else if ((regOffset <= 21) && ((regOffset >= 14))) {
        port_level = PROT_LEVEL_2;
    } else {
        port_level = PROT_LEVEL_1;
    }

    if (mask != MASK_ALL_BITS) {
        I2CMasterReadEx(SOC_I2C_0_REGS, 0x24, &regOffset, 1, &read_val, 1);
        read_val &= (~mask);
        read_val |= (dest_val & mask);
        dest_val = read_val;
    }
    if (port_level == PROT_LEVEL_NONE) {
        i2cwritebuf[0] = regOffset;
        i2cwritebuf[1] = dest_val;
        return I2CMasterWrite(SOC_I2C_0_REGS, 0x24, i2cwritebuf, 2);
    }

    if (port_level != PROT_LEVEL_NONE) {
        xor_reg = regOffset ^ PASSWORD_UNLOCK;
        i2cwritebuf[0] = PASSWORD;
        i2cwritebuf[1] = xor_reg;
        if(!I2CMasterWrite(SOC_I2C_0_REGS, 0x24, i2cwritebuf, 2))  return false;
        i2cwritebuf[0] = regOffset;
        i2cwritebuf[1] = dest_val;
        if(!I2CMasterWrite(SOC_I2C_0_REGS, 0x24, i2cwritebuf, 2))  return false;
        if (port_level == PROT_LEVEL_1) {
            i2cregaddr = PASSWORD;
            if(!I2CMasterReadEx(SOC_I2C_0_REGS, 0x24, &i2cregaddr, 1, &pswreg, 1)) return false;
            if (pswreg == 0) {
                return TRUE;
            } else {
                return FALSE;
            }
        }
        //port_level == 2
        i2cwritebuf[0] = PASSWORD;
        i2cwritebuf[1] = xor_reg;
        if(!I2CMasterWrite(SOC_I2C_0_REGS, 0x24, i2cwritebuf, 2)) return false;
        i2cwritebuf[0] = regOffset;
        i2cwritebuf[1] = dest_val;
        if(!I2CMasterWrite(SOC_I2C_0_REGS, 0x24, i2cwritebuf, 2)) return false;
        i2cregaddr = PASSWORD;
        I2CMasterReadEx(SOC_I2C_0_REGS, 0x24, &i2cregaddr, 1, &pswreg, 1);
        if (pswreg == 0) {
            return TRUE;
        }
    }
    return FALSE;
}



/**
 *  \brief              - Controls output voltage setting for the DCDC1,
 *                        DCDC2, or DCDC3 control registers in the PMIC.
 *
 * \param  dc_cntrl_reg   DCDC Control Register address.
 *                        Must be DEFDCDC1, DEFDCDC2, or DEFDCDC3.
 *
 * \param  volt_sel       Register value to set.  See PMIC TRM for value set.
 *
 * \return:               None.
 */
bool TPS65217VoltageUpdate(unsigned char dc_cntrl_reg, unsigned char volt_sel) {
    /* set voltage level */
    if(!TPS65217RegWrite(dc_cntrl_reg, volt_sel, MASK_ALL_BITS)){
      return false;
    }
    /* set GO bit to initiate voltage transition */
    return TPS65217RegWrite(DEFSLEW, DCDC_GO, DCDC_GO);
}



unsigned char Tps65712ChipId(){
    unsigned char id = 0;
    id =  TPS65217RegRead(CHIPID); 
    return id>>4;
}
