// font.h 
//

#if !defined(AFX_FONT_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_)
#define AFX_FONT_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_

#define UNICODE_FONT_MAX_SECTION   20


//#include "..\include\typedef.h"
#include "ff.h"

#define  PIXELS_PER_BYTE					8
#define	FONT_INDEX_TAB_SIZE				4    //�����ַ���Ӧ�����������Ϣ����Ϊ 4Byte  (b0~b25: ��¼������Ϣ����ʼ��ַ, b26~b31: ��¼��ǰ�ַ������ؿ��)
#define	GET_FONT_WIDTH(charinfo)		(charinfo >> 26)
#define	GET_FONT_OFFADDR(charinfo)		(charinfo & 0x3ffffff)
#define	DB2UC(c1,c2)					(c1 | (((U16)c2)<<8))

typedef struct tagFlSectionInfo {
   unsigned short  First;
   unsigned short  Last;
   unsigned int   OffAddr;
} FL_SECTION_INF, *PFL_SECTION_INF;

typedef struct tagFontLibHeader {
   char             magic[4];    //'U'(or ��M��), 'F', 'L', X---Unicode(or MBCS) Font Library, X: ��ʾ�汾��. �ָߵ�4λ���� 0x12��ʾ Ver 1.2
   unsigned int 	   Size;
   unsigned char    nSection; // ���ּ������ݣ���Ҫ��� UNICODE ������Ч��
   unsigned char    YSize;
   unsigned short   wCpFlag;    // codepageflag:  bit0~bit13 ÿ��bit�ֱ����һ��CodePage ��־�������1�����ʾ��ǰCodePage ��ѡ��������Ϊ��ѡ����
   unsigned short   bTotalChars;
      char             reserved[2];      // Ԥ���ֽ�
} FL_HEADER, *PFL_HEADER; 

typedef struct tagCharInfo{
   unsigned char height;
   unsigned char width;
   unsigned int  pixaddr;
}FL_CHARINFO,*PFL_CHARINFO;


unsigned char  GetFontYSize();
char loadFont(TCHAR * filepath, unsigned int memaddr);
char getCharInfo(unsigned short wCode,FL_CHARINFO *pcharinfo);


#endif // !defined(AFX_FONT_H__6E6D67A4_78AE_410B_B728_2464EF2E40C7__INCLUDED_)
