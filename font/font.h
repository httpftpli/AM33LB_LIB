/**
 *  \file   font.h
 *
 *  \brief
 *  \author  李飞亮
 *  \addtogroup FONT 
 *  @{  
 */



#ifndef __FONT_H__
#define __FONT_H__

#define UNICODE_FONT_MAX_SECTION   20

#include "ff.h"
#include "type.h"

#define MAX_FONT_NFONTLIB      4

#define  PIXELS_PER_BYTE					8
#define	FONT_INDEX_TAB_SIZE				4    //单个字符对应的字体检索信息长度为 4Byte  (b0~b25: 记录点阵信息的起始地址, b26~b31: 记录当前字符的象素宽度)
#define	GET_FONT_WIDTH(charinfo)		(charinfo >> 26)
#define	GET_FONT_OFFADDR(charinfo)		(charinfo & 0x3ffffff)
#define	DB2UC(c1,c2)					(c1 | (((U16)c2)<<8))


typedef struct tagFlSectionInfo {
   unsigned short  First;
   unsigned short  Last;
   unsigned int   OffAddr;
} FL_SECTION_INF, *PFL_SECTION_INF;

typedef struct tagFontLibHeader {
   char             magic[4];    //'U'(or ’M’), 'F', 'L', X---Unicode(or MBCS) Font Library, X: 表示版本号. 分高低4位。如 0x12表示 Ver 1.2
   unsigned int 	Size;
   unsigned char    nSection; // 共分几段数据，主要针对 UNICODE 编码有效。
   unsigned char    YSize;
   unsigned short   wCpFlag;    // codepageflag:  bit0~bit13 每个bit分别代表一个CodePage 标志，如果是1，则表示当前CodePage 被选定，否则为非选定。
   unsigned short   bTotalChars;
      char          reserved[2];      // 预留字节
} FL_HEADER, *PFL_HEADER; 


typedef struct __font{
   FL_HEADER fl_header;
   unsigned int memaddr;
   FL_SECTION_INF *sectionInfoList[20];
} FONT;



/**
 * @brief 字符信息结构体 
 */
typedef struct tagCharInfo{
   unsigned char height; /**< @brief 字符高度 */
   unsigned char width;  /**< @brief 字符宽度 */
   unsigned int  pixaddr; /**< @brief 字符点阵数据首地址 */
}FL_CHARINFO, *PFL_CHARINFO;


typedef struct tagmetric{
   unsigned int width;
   unsigned int height;
}METRIC ;

unsigned char  GetFontYSize();
signed char loadFont(TCHAR * filepath, FONT  *font);
signed char  initFont(TCHAR * dirpath,unsigned int addr);
BOOL getCharInfo(unsigned short wCode,unsigned int font,FL_CHARINFO *pcharinfo);
void getStringMetric(const TEXTCHAR *string ,unsigned int font,METRIC *metric);


#endif 
//! @}
