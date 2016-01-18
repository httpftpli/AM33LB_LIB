/**
 *  \file   misc.c
 *
 *  \brief  
 *  \author  lfl 
 *  \addtogroup misc
 *  \# include "misc.h"
 *  @{
 *   
 */



#include "type.h"

/**
 * @brief 2位BCD转HEX 
 * @param [in] byte BCD编码的2位数，比如99 
 * @return  16位值  比如 99 to 0x63     
 * @date    2013/7/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
unsigned char bcd2hex_2(unsigned char byte) {
    return(byte >> 4) * 10 + (byte & 0x0f);
}
/**
 * @brief HEX转2位BCD
 * @param [in] byte hex码 比如0x63 
 * @return  BCD  比如99     
 * @date    2013/7/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
unsigned short hex2bcd_byte(unsigned char byte) {
    unsigned char h, t, i;
    h = byte / 100;
    t = byte % 100 / 10;
    i = byte % 10;
    return(h << 8) | (t << 4) | (i);
}

/**
 * @brief 4位BCD转HEX 
 * @param [in] s BCD编码的2位数，比如9999 
 * @return  16位值  比如 9999 to 0x270f     
 * @date    2013/7/8
 * @note
 * @code
 * @endcode
 * @pre
 * @see 
 */
unsigned short bcd2hex_4(unsigned short s) {
    return((s >> 12) * 1000) + (((s & (0x0f << 8)) >> 8) * 100)
    + (((s & (0x0f << 4)) >> 4) * 10) + (s & 0x0f);
}


void strToWstr(wchar *wstr, char *str) {
    do {
        *wstr++ = *str;
    }while (*str++ != 0);
}

void wtrToStr(char *str, wchar *wstr) {
    do {
        *str++ = (char)(*wstr);
    }while (*wstr++ != 0);
}

void strtrim(char *desstr, char *srcstr) {
    do {
        if (*srcstr != ' ') {
            *desstr++ = *srcstr;
        }
    }while (*srcstr++ != 0);
}


