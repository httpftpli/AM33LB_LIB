/**
 * \file  hw_types.h
 *
 * \brief Common type definitions and macros
 */




#ifndef _HW_TYPES_H_
#define _HW_TYPES_H_

//*****************************************************************************
//
// Define a boolean type, and values for true and false.
//
//*****************************************************************************
typedef unsigned char tBoolean;

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif
//*****************************************************************************
//
// Macros for hardware access, both direct and via the bit-band region.
//
//*****************************************************************************
#define HWREG(x)                                                              \
        (*((volatile unsigned int *)(x)))
#define HWREGH(x)                                                             \
        (*((volatile unsigned short *)(x)))
#define HWREGB(x)                                                             \
        (*((volatile unsigned char *)(x)))
#define HWREGBITW(x, b)                                                       \
        HWREG(((unsigned int)(x) & 0xF0000000) | 0x02000000 |                \
              (((unsigned int)(x) & 0x000FFFFF) << 5) | ((b) << 2))
#define HWREGBITH(x, b)                                                       \
        HWREGH(((unsigned int)(x) & 0xF0000000) | 0x02000000 |               \
               (((unsigned int)(x) & 0x000FFFFF) << 5) | ((b) << 2))
#define HWREGBITB(x, b)                                                       \
        HWREGB(((unsigned int)(x) & 0xF0000000) | 0x02000000 |               \
               (((unsigned int)(x) & 0x000FFFFF) << 5) | ((b) << 2))

inline void  HWREGBITARRAY(unsigned int addr, unsigned int bit,
                           unsigned int bitnum, unsigned int val) 
{
   unsigned int reg = HWREG(addr);
   reg &= ~((1UL << (bitnum + 1) - 1) << bit);
   reg |= val << bit;
   HWREG(addr) = reg;
}

inline void  HWREGHBITARRAY(unsigned int addr, unsigned int bit,
                           unsigned int bitnum, unsigned int val) 
{
   unsigned short reg = HWREGH(addr);
   reg &= ~((1UL << (bitnum + 1) - 1) << bit);
   reg |= val << bit;
   HWREGH(addr) = reg;
}


#define TRUE    1
#define FALSE   0

#define UNUSED(x) (void)(x)

#define __mdisr 
#define __mdinout
#define __mdout
#define __mdin


#endif // __HW_TYPES_H__
