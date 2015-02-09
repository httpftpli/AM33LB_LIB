/**
 *  \file   algorithm.c
 *
 *  \brief
 *  \author  lfl
 *  \addtogroup Algorithm
 *  \# include "misc.h"
 *   @{
 *
 */


#include "type.h"
#include "algorithm.h"
#include "string.h"
#include "list.h"




#define                 P_32_NORMAL         0x04C11DB7L
/* 32: 04C11DB7 reflected */
#define                 P_32_REFLECTED      0xEDB88320L

/* C: 1EDC6F41 reflected */
#define					P_32C		0x82F63B78L

/* D: A833982B reflected */
#define					P_32D		0xD419CC15L
#define					P_32K		0x741B8CD7L
#define					P_32Q		0x814141ABL
#define                 P_32_XFER   0x000000AFL




void bubbleSortAscend_I32(int *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_U32(unsigned int *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_I16(short *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_U16(unsigned short *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_I8(char *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend_U8(unsigned char *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}

void bubbleSortAscend(int *buf, unsigned int nNum) {
    int j, k;
    int flag = nNum;
    unsigned int tmp;
    while (flag > 0) {
        k = flag;
        flag = 0;
        for (j = 1; j < k; j++) {
            if (buf[j - 1] > buf[j]) {
                tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = j;
            }
        }
    }
}




/*void list_insert_sort(struct list_head *list,  int (*cmp)(const struct list_head *, const struct list_head *)) {
    struct list_head *literal, *n,*literal1,*temp;
    list_for_each_safe(literal, n, list) {
        if (cmp(literal, n) <= 0) {
            continue;
        }
        temp = literal->prev;
        list_del(literal);
        list_for_each(literal1, list) {
            if (cmp(literal, literal1) >= 0) {
                list_add(literal, literal1);
                break;
            }
        }
        if (n == list) {
            break;
        }
    }
}*/



void list_sort_insert(struct list_head *new, struct list_head *list, int (*cmp)(const struct list_head *, const struct list_head *)) {
    struct list_head *literal, *n;
    list_for_each_safe(literal, n, list) {
        if (cmp(new, literal) || n == list) {
            list_add(new, literal);
            break;
        }
    }
}


int sum(int *buf, unsigned int nNum) {
    unsigned int sum = 0;
    for (int i = 0; i < nNum; i++) {
        sum += buf[i];
    }
    return sum;
}



void ringBufInit(RINGBUF *ringBuf, void *buf, unsigned int sizeOfItem, unsigned int nItem) {
    ringBuf->writeIndex = 0;
    ringBuf->readIndex = 0;
    ringBuf->sizeOfItem = sizeOfItem;
    ringBuf->nItem = nItem;
    ringBuf->buf = buf;
    ringBuf->empty = true;
    ringBuf->full = false;
}


BOOL isRingBufFull(RINGBUF *ringBuf) {
    return ringBuf->full;
}

BOOL isRingBufEmpty(RINGBUF *ringBuf) {
    return ringBuf->empty;
}



BOOL ringBufPush(RINGBUF *ringBuf, void *item) {

    if (isRingBufFull(ringBuf)) return FALSE;
    memcpy((unsigned char *)(ringBuf->buf) + ((ringBuf->writeIndex) * ringBuf->sizeOfItem)
           , item, ringBuf->sizeOfItem);
    if (++ringBuf->writeIndex == ringBuf->nItem) ringBuf->writeIndex = 0;
    if (ringBuf->writeIndex==ringBuf->readIndex) {
        ringBuf->full = true;
    }
    ringBuf->empty = false;
    return TRUE;
}



BOOL ringBufPop(RINGBUF *ringBuf, void *item) {
    if (isRingBufEmpty(ringBuf)) {
        return FALSE;
    }
    memcpy(item, (unsigned char *)ringBuf->buf + ringBuf->readIndex * ringBuf->sizeOfItem, ringBuf->sizeOfItem);
    if (++ringBuf->readIndex == ringBuf->nItem) ringBuf->readIndex = 0;
    if (ringBuf->writeIndex==ringBuf->readIndex) {
        ringBuf->empty = true;
    }
    ringBuf->full = false;
    return TRUE;
}



BOOL ringBufRead(RINGBUF *ringBuf, void **item) {
    if (isRingBufEmpty(ringBuf)) {
        return FALSE;
    }
    *item = (void *)((unsigned char *)ringBuf->buf + ringBuf->readIndex * ringBuf->sizeOfItem);
    return TRUE;
}


void ringBufClear(RINGBUF *ringBuf) {
    ringBuf->readIndex = 0;
    ringBuf->writeIndex = 0;
    ringBuf->empty = true;
    ringBuf->full = false;
}



static const unsigned int CRC16Table_IBM[256] = {
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};


static const unsigned int CRC16Table_MD[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};


   //Name   : "CRC-8/CCITT" (new entry) 1-Wire?
   //Width  : 8
   //Poly   : 8D
   //Init   : 00?
   //RefIn  : False?
   //RefOut : False?
   //XorOut : 00?
   //Check  : D2
#if 0
static const unsigned short CRC16Table_CCITT[256] ={
};
#endif







static uint32 crc_tab32_normal[256];
static uint32 crc_tab32_reflected[256];
static uint32 crc_tabxfer_normal[256];
static uint32 crc_tab32C[256];
static uint32 crc_tab32D[256];
static uint32 crc_tab32K[256];
static uint32 crc_tab32Q[256];

void init_crc32_normal_tab(uint32 *table, uint32 polynom)
{
  int i, j;
  uint32 crc;

  for (i=0; i<256; i++)
    {
	  crc = ((uint32) i) << 24;

	  for (j=0; j<8; j++)
        {
		  if ( crc & 0x80000000L ) crc = ( crc << 1 ) ^ polynom;
		  else                     crc <<= 1;
        }
	  *table++ = crc;
    }
}



void init_crc32_reflected_tab(uint32 *table, uint32 polynom)
{
  int i, j;
  uint32 crc;

  for (i=0; i<256; i++)
    {
	  crc = (uint32) i;

	  for (j=0; j<8; j++)
        {
		  if ( crc & 0x00000001L ) crc = ( crc >> 1 ) ^ polynom;
		  else                     crc >>= 1;
        }
	  *table++ = crc;
    }
}


static uint32 update_crc32_normal( uint32 *table, uint32 crc, char c )
{
  uint32 long_c;

  long_c = 0x000000ffL & (uint32) c;

  return (crc << 8) ^ table[((crc >> 24) ^ long_c) & 0xff];
}

static uint32 update_crc32_reflected( uint32 *table, uint32 crc, char c )
{
  uint32 long_c;

  long_c = 0x000000ffL & (uint32) c;

  return (crc >> 8) ^ table[(crc ^ long_c) & 0xff];
}


static uint32 update_crc32_refl( uint32 crc, char c )
{
  return update_crc32_reflected(crc_tab32_reflected,crc,c);
}

static uint32 update_crc32_norm( uint32 crc, char c )
{
  return update_crc32_normal(crc_tab32_normal,crc,c);
}

static uint32 update_crc32_xfer( uint32 crc, char c )
{
  return update_crc32_normal(crc_tabxfer_normal,crc,c);
}

static uint32 update_crc32_c( uint32 crc, char c )
{
  return update_crc32_reflected(crc_tab32C,crc,c);
}

static uint32 update_crc32_d( uint32 crc, char c )
{
  return update_crc32_reflected(crc_tab32D,crc,c);
}

static uint32 update_crc32_k( uint32 crc, char c )
{
  return update_crc32_normal(crc_tab32K,crc,c);
}

static uint32 update_crc32_q( uint32 crc, char c )
{
  return update_crc32_normal(crc_tab32Q,crc,c);
}


static bool crc_tab32_reflected_inited = false;

//<crc-32>
//   Name   : "CRC-32"
//   Alias  : "CRC-32/ADCCP"
//   Alias  : "PKZIP"
//   Width  : 32
//   Poly   : 04C11DB7
//   Init   : FFFFFFFF
//   RefIn  : True
//   RefOut : True
//   XorOut : FFFFFFFF
//   Check  : CBF43926
uint32 calculate_crc32(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0xFFFFFFFFL;

  if(!crc_tab32_reflected_inited) {
      init_crc32_reflected_tab(crc_tab32_reflected, P_32_REFLECTED);
      crc_tab32_reflected_inited  = true;
  }

  for (i=0; i < length; i++)
	{
	  crc32 = update_crc32_refl(crc32,*p++);
	}

  /* One's complement = Xor with FFFFFFFF */
  return ~crc32;
}



//<crc-32-jamcrc>
//   Name   : "JAMCRC"
//   Width  : 32
//   Poly   : 04C11DB7
//   Init   : FFFFFFFF
//   RefIn  : True
//   RefOut : True
//   XorOut : 00000000
//   Check  : 340BC6D9
uint32 calculate_crc32_jamcrc(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0xFFFFFFFFL;

  if(!crc_tab32_reflected_inited) {
      init_crc32_reflected_tab(crc_tab32_reflected, P_32_REFLECTED);
      crc_tab32_reflected_inited  = true;
  }

  for (i=0; i < length; i++)
	{
	  crc32 = update_crc32_refl(crc32,*p++);
	}

  return crc32;
}


static bool crc_tab32c_inited = false;

//<crc-32-C>
//   Name   : "CRC-32C"
//   Alias  : "CRC-32/ISCSI"
//   Alias  : "CRC-32/CASTAGNOLI"
//   Width  : 32
//   Poly   : 1EDC6F41
//   Init   : FFFFFFFF
//   RefIn  : True
//   RefOut : True
//   XorOut : FFFFFFFF
//   Check  : E3069283

uint32 calculate_crc32_c(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0xFFFFFFFFL;
  if (!crc_tab32c_inited) {
      crc_tab32c_inited = true;
      init_crc32_reflected_tab(crc_tab32C, P_32C);
  }
  for (i = 0; i < length; i++)
	{
	  crc32 = update_crc32_c(crc32,*p++);
	}

  /* One's complement = Xor with FFFFFFFF */
  return ~crc32;
}


static bool crc_tab32d_inited = false;

//<crc-32-D>
//   Name   : "CRC-32D"
//   Width  : 32
//   Poly   : A833982B
//   Init   : FFFFFFFF
//   RefIn  : True
//   RefOut : True
//   XorOut : FFFFFFFF
//   Check  : 87315576
uint32 calculate_crc32_d(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0xFFFFFFFFL;

  if (!crc_tab32d_inited) {
      crc_tab32d_inited = true;
      init_crc32_reflected_tab(crc_tab32D, P_32D);
  }
  for (i=0; i < length; i++)
	{
	  crc32 = update_crc32_d(crc32,*p++);
	}

  /* One's complement = Xor with FFFFFFFF */
  return ~crc32;
}


static bool crc_tab32_normal_inited = false;

//<crc-32-bzip2>
//   Name   : "CRC-32/BZIP2"
//   Alias  : "B-CRC-32"
//   Width  : 32
//   Poly   : 04C11DB7
//   Init   : FFFFFFFF
//   RefIn  : False
//   RefOut : False
//   XorOut : FFFFFFFF
//   Check  : FC891918
uint32 calculate_crc32_bzip2(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0xFFFFFFFFL;

  if (!crc_tab32_normal_inited) {
      crc_tab32_normal_inited =  true;
      init_crc32_normal_tab(crc_tab32_normal, P_32_NORMAL);
  }
  for (i=0; i < length; i++)
	{
	  crc32 = update_crc32_norm(crc32,*p++);
	}

  /* One's complement = Xor with FFFFFFFF */
  return ~crc32;
}


//<crc-32-mpeg-2>
//   Name   : "CRC-32/MPEG-2"
//   Width  : 32
//   Poly   : 04C11DB7
//   Init   : FFFFFFFF
//   RefIn  : False
//   RefOut : False
//   XorOut : 00000000
//   Check  : 0376E6E7
uint32 calculate_crc32_mpeg2(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0xFFFFFFFFL;

  if (!crc_tab32_normal_inited) {
      crc_tab32_normal_inited =  true;
      init_crc32_normal_tab(crc_tab32_normal, P_32_NORMAL);
  }
  for (i=0; i < length; i++)
	{
	  crc32 = update_crc32_norm(crc32,*p++);
	}

  return crc32;
}


//<crc-32-posix>
//   Name   : "CRC-32/POSIX"
//   Alias  : "CKSUM"
//   Width  : 32
//   Poly   : 04C11DB7
//   Init   : 00000000
//   RefIn  : False
//   RefOut : False
//   XorOut : FFFFFFFF
//   Check  : 765E7680
//   LCheck : 377A6011
uint32 calculate_crc32_posix(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0L;

  if (!crc_tab32_normal_inited) {
      crc_tab32_normal_inited =  true;
      init_crc32_normal_tab(crc_tab32_normal, P_32_NORMAL);
  }
  for (i=0; i < length; i++)
	{
	  crc32 = update_crc32_norm(crc32,*p++);
	}

  /* One's complement = Xor with FFFFFFFF */
  return ~crc32;
}

static bool crc_tab32k_init = false;

//<crc-32-K> (new entry)
//   Name   : "CRC-32K"
//   Alias  : "CRC-32/KOOPMAN"
//  Width  : 32
//   Poly   : 741B8CD7
//   Init   : 00000000?
//   RefIn  : False?
//   RefOut : False?
//   XorOut : 00000000?
//   Check  : not sure
uint32 calculate_crc32_k(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0L;
  if (!crc_tab32k_init) {
      crc_tab32k_init = true;
      init_crc32_normal_tab(crc_tab32K, P_32K); /* Not sure */
  }
  for (i=0; i < length; i++)
	{
	  crc32 = update_crc32_k(crc32,*p++);
	}

  return crc32;
}



static bool crc_tab32q_init = false;

//<crc-32-Q>
//   Name   : "CRC-32Q"
//   Width  : 32
//   Poly   : 814141AB
//   Init   : 00000000
//   RefIn  : False
//   RefOut : False
//   XorOut : 00000000
//   Check  : 3010BF7F
uint32 calculate_crc32_q(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0L;
  if (!crc_tab32q_init) {
      crc_tab32q_init = false;
      init_crc32_normal_tab(crc_tab32Q, P_32Q);
  }
  for (i = 0; i < length; i++)
	{
	  crc32 = update_crc32_q(crc32,*p++);
	}

  return crc32;
}

static bool crc_tabxfer_normal_inited = false;


//<crc-32-xfer>
//   Name   : "XFER"
//   Width  : 32
//   Poly   : 000000AF
//   Init   : 00000000
//   RefIn  : False
//   RefOut : False
//   XorOut : 00000000
//   Check  : BD0BE338
uint32 calculate_crc32_xfer(char *p, unsigned int length)
{
  uint32 crc32;
  unsigned int i;

  crc32 = 0L;
  if (!crc_tabxfer_normal_inited) {
      crc_tabxfer_normal_inited = true;
      init_crc32_normal_tab(crc_tabxfer_normal, P_32_XFER);
  }
  for (i = 0; i < length; i++)
	{
	  crc32 = update_crc32_xfer(crc32,*p++);
	}

  return crc32;
}


unsigned short crc16_IBM(void *dataIn, int length) {
    unsigned short result = 0;
    unsigned short tableNo = 0;
    unsigned char *data = (unsigned char *)dataIn;
    for (unsigned int i = 0; i < length; i++) {
        tableNo = ((result & 0xff) ^ (data[i] & 0xff));
        result = ((result >> 8) & 0xff) ^ CRC16Table_IBM[tableNo];
    }
    return result;
}



uint16 crc16_MD(void *date, uint16 len) {
    unsigned char byte;
    unsigned short crc16 = 0;
    unsigned char *dat = (unsigned char *)date;
    for (unsigned int i = 0; i < len; i++) {
        byte = (uint8)(crc16 >> 8);
        crc16 <<= 8;
        crc16 ^= CRC16Table_MD[byte ^ *dat];
        dat++;
    }
    return crc16;
}




#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
{ \
	a += F(b,c,d) + x + ac; \
	a = ROTATE_LEFT(a,s); \
	a += b; \
}
#define GG(a,b,c,d,x,s,ac) \
{ \
	a += G(b,c,d) + x + ac; \
	a = ROTATE_LEFT(a,s); \
	a += b; \
}
#define HH(a,b,c,d,x,s,ac) \
{ \
	a += H(b,c,d) + x + ac; \
	a = ROTATE_LEFT(a,s); \
	a += b; \
}
#define II(a,b,c,d,x,s,ac) \
{ \
	a += I(b,c,d) + x + ac; \
	a = ROTATE_LEFT(a,s); \
	a += b; \
}



unsigned char PADDING[] = { 0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };



static void MD5Encode(unsigned char *output, unsigned int *input, unsigned int len) {
    unsigned int i = 0, j = 0;
    while (j < len) {
        output[j] = input[i] & 0xFF;
        output[j + 1] = (input[i] >> 8) & 0xFF;
        output[j + 2] = (input[i] >> 16) & 0xFF;
        output[j + 3] = (input[i] >> 24) & 0xFF;
        i++;
        j += 4;
    }
}
static void MD5Decode(unsigned int *output, unsigned char *input, unsigned int len) {
    unsigned int i = 0, j = 0;
    while (j < len) {
        output[i] = (input[j]) |
                    (input[j + 1] << 8) |
                    (input[j + 2] << 16) |
                    (input[j + 3] << 24);
        i++;
        j += 4;
    }
}


static void MD5Transform(unsigned int state[4], unsigned char block[64]) {
    unsigned int a = state[0];
    unsigned int b = state[1];
    unsigned int c = state[2];
    unsigned int d = state[3];
    unsigned int x[64];
    MD5Decode(x, block, 64);
    FF(a, b, c, d, x[0], 7, 0xd76aa478); /* 1 */
    FF(d, a, b, c, x[1], 12, 0xe8c7b756); /* 2 */
    FF(c, d, a, b, x[2], 17, 0x242070db); /* 3 */
    FF(b, c, d, a, x[3], 22, 0xc1bdceee); /* 4 */
    FF(a, b, c, d, x[4], 7, 0xf57c0faf); /* 5 */
    FF(d, a, b, c, x[5], 12, 0x4787c62a); /* 6 */
    FF(c, d, a, b, x[6], 17, 0xa8304613); /* 7 */
    FF(b, c, d, a, x[7], 22, 0xfd469501); /* 8 */
    FF(a, b, c, d, x[8], 7, 0x698098d8); /* 9 */
    FF(d, a, b, c, x[9], 12, 0x8b44f7af); /* 10 */
    FF(c, d, a, b, x[10], 17, 0xffff5bb1); /* 11 */
    FF(b, c, d, a, x[11], 22, 0x895cd7be); /* 12 */
    FF(a, b, c, d, x[12], 7, 0x6b901122); /* 13 */
    FF(d, a, b, c, x[13], 12, 0xfd987193); /* 14 */
    FF(c, d, a, b, x[14], 17, 0xa679438e); /* 15 */
    FF(b, c, d, a, x[15], 22, 0x49b40821); /* 16 */

    /* Round 2 */
    GG(a, b, c, d, x[1], 5, 0xf61e2562); /* 17 */
    GG(d, a, b, c, x[6], 9, 0xc040b340); /* 18 */
    GG(c, d, a, b, x[11], 14, 0x265e5a51); /* 19 */
    GG(b, c, d, a, x[0], 20, 0xe9b6c7aa); /* 20 */
    GG(a, b, c, d, x[5], 5, 0xd62f105d); /* 21 */
    GG(d, a, b, c, x[10], 9, 0x2441453); /* 22 */
    GG(c, d, a, b, x[15], 14, 0xd8a1e681); /* 23 */
    GG(b, c, d, a, x[4], 20, 0xe7d3fbc8); /* 24 */
    GG(a, b, c, d, x[9], 5, 0x21e1cde6); /* 25 */
    GG(d, a, b, c, x[14], 9, 0xc33707d6); /* 26 */
    GG(c, d, a, b, x[3], 14, 0xf4d50d87); /* 27 */
    GG(b, c, d, a, x[8], 20, 0x455a14ed); /* 28 */
    GG(a, b, c, d, x[13], 5, 0xa9e3e905); /* 29 */
    GG(d, a, b, c, x[2], 9, 0xfcefa3f8); /* 30 */
    GG(c, d, a, b, x[7], 14, 0x676f02d9); /* 31 */
    GG(b, c, d, a, x[12], 20, 0x8d2a4c8a); /* 32 */

    /* Round 3 */
    HH(a, b, c, d, x[5], 4, 0xfffa3942); /* 33 */
    HH(d, a, b, c, x[8], 11, 0x8771f681); /* 34 */
    HH(c, d, a, b, x[11], 16, 0x6d9d6122); /* 35 */
    HH(b, c, d, a, x[14], 23, 0xfde5380c); /* 36 */
    HH(a, b, c, d, x[1], 4, 0xa4beea44); /* 37 */
    HH(d, a, b, c, x[4], 11, 0x4bdecfa9); /* 38 */
    HH(c, d, a, b, x[7], 16, 0xf6bb4b60); /* 39 */
    HH(b, c, d, a, x[10], 23, 0xbebfbc70); /* 40 */
    HH(a, b, c, d, x[13], 4, 0x289b7ec6); /* 41 */
    HH(d, a, b, c, x[0], 11, 0xeaa127fa); /* 42 */
    HH(c, d, a, b, x[3], 16, 0xd4ef3085); /* 43 */
    HH(b, c, d, a, x[6], 23, 0x4881d05); /* 44 */
    HH(a, b, c, d, x[9], 4, 0xd9d4d039); /* 45 */
    HH(d, a, b, c, x[12], 11, 0xe6db99e5); /* 46 */
    HH(c, d, a, b, x[15], 16, 0x1fa27cf8); /* 47 */
    HH(b, c, d, a, x[2], 23, 0xc4ac5665); /* 48 */

    /* Round 4 */
    II(a, b, c, d, x[0], 6, 0xf4292244); /* 49 */
    II(d, a, b, c, x[7], 10, 0x432aff97); /* 50 */
    II(c, d, a, b, x[14], 15, 0xab9423a7); /* 51 */
    II(b, c, d, a, x[5], 21, 0xfc93a039); /* 52 */
    II(a, b, c, d, x[12], 6, 0x655b59c3); /* 53 */
    II(d, a, b, c, x[3], 10, 0x8f0ccc92); /* 54 */
    II(c, d, a, b, x[10], 15, 0xffeff47d); /* 55 */
    II(b, c, d, a, x[1], 21, 0x85845dd1); /* 56 */
    II(a, b, c, d, x[8], 6, 0x6fa87e4f); /* 57 */
    II(d, a, b, c, x[15], 10, 0xfe2ce6e0); /* 58 */
    II(c, d, a, b, x[6], 15, 0xa3014314); /* 59 */
    II(b, c, d, a, x[13], 21, 0x4e0811a1); /* 60 */
    II(a, b, c, d, x[4], 6, 0xf7537e82); /* 61 */
    II(d, a, b, c, x[11], 10, 0xbd3af235); /* 62 */
    II(c, d, a, b, x[2], 15, 0x2ad7d2bb); /* 63 */
    II(b, c, d, a, x[9], 21, 0xeb86d391); /* 64 */
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
}



void MD5Init(MD5_CTX *context) {
    context->count[0] = 0;
    context->count[1] = 0;
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
}



void MD5Update(MD5_CTX *context, unsigned char *input, unsigned int inputlen) {
    unsigned int i = 0, index = 0, partlen = 0;
    index = (context->count[0] >> 3) & 0x3F;
    partlen = 64 - index;
    context->count[0] += inputlen << 3;
    if (context->count[0] < (inputlen << 3)) context->count[1]++;
    context->count[1] += inputlen >> 29;

    if (inputlen >= partlen) {
        memcpy(&context->buffer[index], input, partlen);
        MD5Transform(context->state, context->buffer);
        for (i = partlen; i + 64 <= inputlen; i += 64) MD5Transform(context->state, &input[i]);
        index = 0;
    } else {
        i = 0;
    }
    memcpy(&context->buffer[index], &input[i], inputlen - i);
}




void MD5Final(MD5_CTX *context, unsigned char digest[16]) {
    unsigned int index = 0, padlen = 0;
    unsigned char bits[8];
    index = (context->count[0] >> 3) & 0x3F;
    padlen = (index < 56) ? (56 - index) : (120 - index);
    MD5Encode(bits, context->count, 8);
    MD5Update(context, PADDING, padlen);
    MD5Update(context, bits, 8);
    MD5Encode(digest, context->state, 16);
}


//! @}
