


#ifndef  __LIB_GUI_H
#define  __LIB_GUI_H



#include "GUIType.h"
#include "type.h"
#include "pf_lcd.h"
//#include <string.h>
#include "pf_lcd.h"
#include "utf8.h"
#include "mem.h"
#include "debug.h"
#include "pf_platform_cfg.h"




/*********************************************************************
*
*        macros, function replacement
*
**********************************************************************
*/
#define GUI_COUNTOF(a) (sizeof(a) / sizeof(a[0]))




/*
      *********************************
      *                               *
      * Text and drawing mode defines *
      *                               *
      *********************************

These defines come in two flavors: the long version (.._DRAWMODE_..)
and the short ones (.._DM_..). They are identical, feel free to use
which ever one you like best.

*/

#define GUI_DRAWMODE_NORMAL  LCD_DRAWMODE_NORMAL
#define GUI_DRAWMODE_XOR     LCD_DRAWMODE_XOR
#define GUI_DRAWMODE_TRANS   LCD_DRAWMODE_TRANS
#define GUI_DRAWMODE_REV     LCD_DRAWMODE_REV
#define GUI_DM_NORMAL        LCD_DRAWMODE_NORMAL
#define GUI_DM_XOR           LCD_DRAWMODE_XOR
#define GUI_DM_TRANS         LCD_DRAWMODE_TRANS
#define GUI_DM_REV           LCD_DRAWMODE_REV

#define GUI_TEXTMODE_NORMAL  LCD_DRAWMODE_NORMAL
#define GUI_TEXTMODE_XOR     LCD_DRAWMODE_XOR
#define GUI_TEXTMODE_TRANS   LCD_DRAWMODE_TRANS
#define GUI_TEXTMODE_REV     LCD_DRAWMODE_REV
#define GUI_TM_NORMAL        LCD_DRAWMODE_NORMAL
#define GUI_TM_XOR           LCD_DRAWMODE_XOR
#define GUI_TM_TRANS         LCD_DRAWMODE_TRANS
#define GUI_TM_REV           LCD_DRAWMODE_REV

/* Text alignment flags, horizontal */
#define GUI_TA_HORIZONTAL  (3<<0)
#define GUI_TA_LEFT        (0<<0)
#define GUI_TA_RIGHT	     (1<<0)
#define GUI_TA_CENTER	     (2<<0)
#define GUI_TA_HCENTER	   GUI_TA_CENTER  /* easier to remember :-)  */

/* Text alignment flags, vertical */
#define GUI_TA_VERTICAL   (3<<2)
#define GUI_TA_TOP	      (0<<2)
#define GUI_TA_BOTTOM	    (1<<2)
#define GUI_TA_BASELINE   (2<<2)
#define GUI_TA_VCENTER    (3<<2)

/*    *********************************
      *
      *     Min/Max coordinates
      *
      *********************************
*/
/* Define minimum and maximum coordinates in x and y */
#define GUI_XMIN -4095
#define GUI_XMAX  4095
#define GUI_YMIN -4095
#define GUI_YMAX  4095




/*********************************************************************
*
*             GUI_CONTEXT
*
**********************************************************************

  This structure is public for one reason only:
  To allow the application to save and restore the context.
*/

typedef union {
  U8  aColorIndex8[2];
  U16 aColorIndex16[2];
  U32 aColorIndex32[2];
} LCD_COLORINDEX_UNION;

struct GUI_CONTEXT {
  LCD_RECT       ClipRect;
/* Variables in GUICHAR module */
  const GUI_FONT     * pAFont;
 
  I16P LBorder;
  I16P DispPosX, DispPosY;
  I16P DrawPosX, DrawPosY;
  I16P TextMode, TextAlign;
  GUI_COLOR Color, BkColor;           /* Required only when changing devices and for speed opt (caching) */
  #if GUI_SUPPORT_AA
    const tLCD_HL_APIList* pLCD_HL;     /* Required to reroute drawing (HLine & Pixel) to the AA module */
    U8 AA_Factor;
    U8 AA_HiResEnable;
  #endif
};

typedef struct GUI_CONTEXT GUI_CONTEXT;




/*********************************************************************
*
*      Get / Set Attributes
*
**********************************************************************
*/
 
GUI_COLOR GUI_GetBkColor     (void);
GUI_COLOR GUI_GetColor       (void);
int       GUI_GetBkColorIndex(void);
int       GUI_GetColorIndex  (void);
U8        GUI_GetPenSize     (void);
U8        GUI_GetPenShape    (void);
U8        GUI_GetLineStyle   (void);
U8        GUI_GetFillStyle   (void);

void      GUI_SetBkColor   (GUI_COLOR);
void      GUI_SetColor     (GUI_COLOR);
void      GUI_SetBkColorIndex(int Index);
void      GUI_SetColorIndex(int Index);

U8        GUI_SetPenSize   (U8 Size);
U8        GUI_SetPenShape  (U8 Shape);
U8        GUI_SetLineStyle (U8 Style);
U8        GUI_SetFillStyle (U8 Style);

/* Get/Set Character used as decimal point (usually '.' or ',') */
char      GUI_GetDecChar(void);
char      GUI_SetDecChar(char c);


/*********************************************************************
*
*       Color / Index related functions
*
**********************************************************************
*/

int       GUI_Color2Index(GUI_COLOR color);
GUI_COLOR GUI_Color2VisColor(GUI_COLOR color);
char      GUI_ColorIsAvailable(GUI_COLOR color);
GUI_COLOR GUI_Index2Color(int Index);
void      GUI_InitLUT(void);
void      GUI_SetLUTEntry (U8 Pos, GUI_COLOR Color);
void      GUI_SetLUTColor (U8 Pos, GUI_COLOR Color);
void      GUI_SetLUTColorEx(U8 Pos, LCD_COLOR Color, unsigned int LayerIndex);
U32       GUI_CalcColorDist (GUI_COLOR Color0, GUI_COLOR  Color1);
U32       GUI_CalcVisColorError(GUI_COLOR color);



/*********************************************************************
*
*             draw
*
**********************************************************************
*/




typedef unsigned int COLOR;
#define RGB(R,G,B)      ((B)|(G)<<8|(R)<<16|0xff000000)
#define RGBA(R,G,B,A)   ((B)|(G)<<8|(R)<<16|(A)<<24)
#define COLORR(C)       (((C)&0xff0000)>>16)
#define COLORG(C)       (((C)&0x00ff00)>>8)
#define COLORB(C)       ((C)&0xff)
#define COLORA(C)       (((C)&0xff000000)>>24)
#define C_RED				RGB(0xff,0,0)
#define C_GREEN				RGB(0,0xff,0)
#define C_BLUE				RGB(0,0,0xff)
#define C_BLACK             RGB(0,0,0)
#define C_WHITE             RGB(0xff,0xff,0xff)
#define C_TRANSPARENT       RGBA(0,0,0,0)
#define C_GRAY				RGB(0x80,0x80,0x80)
#define C_DARKGRAY          RGB(0xA9,0xA9,0xA9)

#if LCD_PIX_SIZE==2
#define color2Lcd(C) ((COLORB(C)&0xf8)<<8|(COLORG(C)&0xfc)<<3|(COLORR(C)>>3))
#elif LCD_PIX_SIZE==4
#define color2Lcd(C) (C)
#else 
#error 
#endif




#define ALIGN_LEFT            0
#define ALIGN_RIGHT           1
#define ALIGN_MIDDLE          2



#define ALIGN_LEFT_UP         ALIGN_LEFT<<4 | ALIGN_LEFT
#define ALIGN_LEFT_MIDDLE     ALIGN_MIDDLE<<4 | ALIGN_LEFT		
#define ALIGN_MIDDLE_UP	      ALIGN_LEFT<<4 | ALIGN_MIDDLE
#define ALIGN_MIDDLE_MIDDLE	  ALIGN_MIDDLE<<4 | ALIGN_MIDDLE			
#define ALIGN_RIGHT_UP	      ALIGN_LEFT<<4 | ALIGN_RIGHT
#define ALIGN_RIGHT_MIDDLE	  ALIGN_MIDDLE<<4 | ALIGN_RIGHT				


extern GUI_FONT *fonts[4];

#define FONT_ASCII_16 fonts[0]
#define FONT_ASCII_20 fonts[1]
#define FONT_16 fonts[2]
#define FONT_20 fonts[3]

extern GUI_CONTEXT GUI_Context;
extern unsigned int g_fontFlag;

extern int getStringMetricWidth(const TEXTCHAR *string);
extern int getStringMetricWidthEx(const TEXTCHAR *string,unsigned int len);
extern int getStringMetricWidthEx_ucs2(const wchar *wcs,unsigned int len);
extern unsigned int  initFont(unsigned int addr);
extern char  getCurFontYSize();
extern char getFontYSize(GUI_FONT *font);

extern void drawPix(uint16 x,uint16 y,COLOR color);
extern void drawHLine(uint16 x, uint16 y, uint16 lineLen);
extern void drawHLineEx(uint16 x, uint16 y, uint16 lineLen, COLOR color) ;
extern void drawVLine(uint16 x, uint16 y, uint16 lineLen);
extern void drawVLineEx(uint16 x, uint16 y, uint16 lineLen,COLOR color);
extern void drawRect(uint16 x, uint16 y, uint16 width, uint16 heigh);
extern void drawRectEx(uint16 x, uint16 y, uint16 width, uint16 heigh, COLOR color);
extern void drawRectFill(uint32 x, uint32 y, uint32 width, uint32 height) ;
extern void drawRectFillEx(uint32 x, uint32 y, uint32 width, uint32 height, COLOR color) ;
extern void DrawCharUcs2(unsigned short character, unsigned int x, unsigned int y);
extern void DrawCharUcs2Ex(unsigned short character, unsigned int x, unsigned int y, GUI_FONT *font, COLOR color_f, COLOR color_b);
extern void drawString_ascii(const char *string,unsigned x ,unsigned int y);
extern void drawString_utf8(const char *utf8string,unsigned x ,unsigned int y);
extern void drawString_ucs2(const wchar *wcs,unsigned int x,unsigned int y);
extern void drawString(const TEXTCHAR *text, unsigned int x, unsigned int y);
extern void drawStringLen(const TEXTCHAR *text, unsigned int x, unsigned int y,unsigned int len);
extern void drawStringEx(const TEXTCHAR *text, unsigned int x, unsigned int y, const GUI_FONT *font, COLOR color_f,COLOR color_b);
extern void drawStringEx_ucs2(const wchar *wcs, unsigned int x, unsigned int y, const GUI_FONT *font, COLOR color_f,COLOR color_b);
extern void drawStringAlign(const TEXTCHAR *text, uint32 align, uint16 x, uint16 y, uint16 width, uint16 height);
extern void drawStringAlign_ucs2(const wchar *wcs,unsigned int align,unsigned short x,unsigned short y,unsigned short width,unsigned short height);
extern void drawStringAlignEx(const TEXTCHAR *text, uint32 align, uint16 x, uint16 y, uint16 width,uint16 height, const GUI_FONT *font, COLOR color_f, COLOR color_b);
extern void drawStringAlignEx_ucs2(const wchar *wcs, uint32 align, uint16 x, uint16 y, uint16 width,uint16 height, const GUI_FONT *font, COLOR color_f, COLOR color_b);
extern void    GUI_SaveContext(const GUI_CONTEXT* pContext);
extern void    GUI_RestoreContext(const GUI_CONTEXT* pContext);

/*********************************************************************
*
*              Text related routines
*
**********************************************************************
*/


const GUI_FONT   * GUI_SetFont(const GUI_FONT   * pNewFont);

/*********************************************************************
*
*       Position independent fonts
*
**********************************************************************
*/
BOOL GUI_SIF_CreateFont(const void * pFontData, GUI_FONT * pFont, const GUI_SIF_TYPE * pFontType);
void GUI_SIF_DeleteFont(GUI_FONT * pFont);




/******************************************************************
*
*                 Anti Aliasing
*
*******************************************************************
*/

void GUI_AA_DisableHiRes     (void);
void GUI_AA_EnableHiRes      (void);
int  GUI_AA_GetFactor        (void);
void GUI_AA_SetFactor        (int Factor);
void GUI_AA_DrawArc          (int x0, int y0, int rx, int ry, int a0, int a1);
void GUI_AA_DrawLine         (int x0, int y0, int x1, int y1);
void GUI_AA_DrawPolyOutline  (const GUI_POINT * pSrc, int NumPoints, int Thickness, int x, int y);
void GUI_AA_DrawPolyOutlineEx(const GUI_POINT * pSrc, int NumPoints, int Thickness, int x, int y, GUI_POINT * pBuffer);
void GUI_AA_FillCircle       (int x0, int y0, int r);
void GUI_AA_FillPolygon      (GUI_POINT* pPoints, int NumPoints, int x0, int y0);



extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_AA2;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_AA4;

/**************************************************************
*
*      Defines for constants
*
***************************************************************
*/

/* Reading data routines */

static inline  unsigned short  GUI__Read16(const void **pdat) {
   unsigned short val = **(unsigned short**)pdat ;
   *pdat = *(unsigned char**)pdat+2;
   return val;
}
static inline  unsigned int GUI__Read32(const void **pdat){
   unsigned int val = **(unsigned int**)pdat ;
   *pdat = *(unsigned char**)pdat+4;
   return val;
}

#define	________	0x0
#define	_______X	0x1
#define	______X_	0x2
#define	______XX	0x3
#define	_____X__	0x4
#define	_____X_X	0x5
#define	_____XX_	0x6
#define	_____XXX	0x7
#define	____X___	0x8
#define	____X__X	0x9
#define	____X_X_	0xa
#define	____X_XX	0xb
#define	____XX__	0xc
#define	____XX_X	0xd
#define	____XXX_	0xe
#define	____XXXX	0xf
#define	___X____	0x10
#define	___X___X	0x11
#define	___X__X_	0x12
#define	___X__XX	0x13
#define	___X_X__	0x14
#define	___X_X_X	0x15
#define	___X_XX_	0x16
#define	___X_XXX	0x17
#define	___XX___	0x18
#define	___XX__X	0x19
#define	___XX_X_	0x1a
#define	___XX_XX	0x1b
#define	___XXX__	0x1c
#define	___XXX_X	0x1d
#define	___XXXX_	0x1e
#define	___XXXXX	0x1f
#define	__X_____	0x20
#define	__X____X	0x21
#define	__X___X_	0x22
#define	__X___XX	0x23
#define	__X__X__	0x24
#define	__X__X_X	0x25
#define	__X__XX_	0x26
#define	__X__XXX	0x27
#define	__X_X___	0x28
#define	__X_X__X	0x29
#define	__X_X_X_	0x2a
#define	__X_X_XX	0x2b
#define	__X_XX__	0x2c
#define	__X_XX_X	0x2d
#define	__X_XXX_	0x2e
#define	__X_XXXX	0x2f
#define	__XX____	0x30
#define	__XX___X	0x31
#define	__XX__X_	0x32
#define	__XX__XX	0x33
#define	__XX_X__	0x34
#define	__XX_X_X	0x35
#define	__XX_XX_	0x36
#define	__XX_XXX	0x37
#define	__XXX___	0x38
#define	__XXX__X	0x39
#define	__XXX_X_	0x3a
#define	__XXX_XX	0x3b
#define	__XXXX__	0x3c
#define	__XXXX_X	0x3d
#define	__XXXXX_	0x3e
#define	__XXXXXX	0x3f
#define	_X______	0x40
#define	_X_____X	0x41
#define	_X____X_	0x42
#define	_X____XX	0x43
#define	_X___X__	0x44
#define	_X___X_X	0x45
#define	_X___XX_	0x46
#define	_X___XXX	0x47
#define	_X__X___	0x48
#define	_X__X__X	0x49
#define	_X__X_X_	0x4a
#define	_X__X_XX	0x4b
#define	_X__XX__	0x4c
#define	_X__XX_X	0x4d
#define	_X__XXX_	0x4e
#define	_X__XXXX	0x4f
#define	_X_X____	0x50
#define	_X_X___X	0x51
#define	_X_X__X_	0x52
#define	_X_X__XX	0x53
#define	_X_X_X__	0x54
#define	_X_X_X_X	0x55
#define	_X_X_XX_	0x56
#define	_X_X_XXX	0x57
#define	_X_XX___	0x58
#define	_X_XX__X	0x59
#define	_X_XX_X_	0x5a
#define	_X_XX_XX	0x5b
#define	_X_XXX__	0x5c
#define	_X_XXX_X	0x5d
#define	_X_XXXX_	0x5e
#define	_X_XXXXX	0x5f
#define	_XX_____	0x60
#define	_XX____X	0x61
#define	_XX___X_	0x62
#define	_XX___XX	0x63
#define	_XX__X__	0x64
#define	_XX__X_X	0x65
#define	_XX__XX_	0x66
#define	_XX__XXX	0x67
#define	_XX_X___	0x68
#define	_XX_X__X	0x69
#define	_XX_X_X_	0x6a
#define	_XX_X_XX	0x6b
#define	_XX_XX__	0x6c
#define	_XX_XX_X	0x6d
#define	_XX_XXX_	0x6e
#define	_XX_XXXX	0x6f
#define	_XXX____	0x70
#define	_XXX___X	0x71
#define	_XXX__X_	0x72
#define	_XXX__XX	0x73
#define	_XXX_X__	0x74
#define	_XXX_X_X	0x75
#define	_XXX_XX_	0x76
#define	_XXX_XXX	0x77
#define	_XXXX___	0x78
#define	_XXXX__X	0x79
#define	_XXXX_X_	0x7a
#define	_XXXX_XX	0x7b
#define	_XXXXX__	0x7c
#define	_XXXXX_X	0x7d
#define	_XXXXXX_	0x7e
#define	_XXXXXXX	0x7f
#define	X_______	0x80
#define	X______X	0x81
#define	X_____X_	0x82
#define	X_____XX	0x83
#define	X____X__	0x84
#define	X____X_X	0x85
#define	X____XX_	0x86
#define	X____XXX	0x87
#define	X___X___	0x88
#define	X___X__X	0x89
#define	X___X_X_	0x8a
#define	X___X_XX	0x8b
#define	X___XX__	0x8c
#define	X___XX_X	0x8d
#define	X___XXX_	0x8e
#define	X___XXXX	0x8f
#define	X__X____	0x90
#define	X__X___X	0x91
#define	X__X__X_	0x92
#define	X__X__XX	0x93
#define	X__X_X__	0x94
#define	X__X_X_X	0x95
#define	X__X_XX_	0x96
#define	X__X_XXX	0x97
#define	X__XX___	0x98
#define	X__XX__X	0x99
#define	X__XX_X_	0x9a
#define X__XX_XX	0x9b
#define X__XXX__	0x9c
#define X__XXX_X	0x9d
#define	X__XXXX_	0x9e
#define	X__XXXXX	0x9f
#define	X_X_____	0xa0
#define	X_X____X	0xa1
#define	X_X___X_	0xa2
#define	X_X___XX	0xa3
#define	X_X__X__	0xa4
#define	X_X__X_X	0xa5
#define	X_X__XX_	0xa6
#define	X_X__XXX	0xa7
#define	X_X_X___	0xa8
#define	X_X_X__X	0xa9
#define	X_X_X_X_	0xaa
#define	X_X_X_XX	0xab
#define	X_X_XX__	0xac
#define	X_X_XX_X	0xad
#define	X_X_XXX_	0xae
#define	X_X_XXXX	0xaf
#define	X_XX____	0xb0
#define X_XX___X	0xb1
#define	X_XX__X_	0xb2
#define	X_XX__XX	0xb3
#define	X_XX_X__	0xb4
#define	X_XX_X_X	0xb5
#define	X_XX_XX_	0xb6
#define	X_XX_XXX	0xb7
#define	X_XXX___	0xb8
#define	X_XXX__X	0xb9
#define	X_XXX_X_	0xba
#define	X_XXX_XX	0xbb
#define	X_XXXX__	0xbc
#define	X_XXXX_X	0xbd
#define	X_XXXXX_	0xbe
#define	X_XXXXXX	0xbf
#define	XX______	0xc0
#define	XX_____X	0xc1
#define	XX____X_	0xc2
#define	XX____XX	0xc3
#define	XX___X__	0xc4
#define	XX___X_X	0xc5
#define	XX___XX_	0xc6
#define	XX___XXX	0xc7
#define	XX__X___	0xc8
#define	XX__X__X	0xc9
#define	XX__X_X_	0xca
#define	XX__X_XX	0xcb
#define	XX__XX__	0xcc
#define	XX__XX_X	0xcd
#define	XX__XXX_	0xce
#define XX__XXXX	0xcf
#define	XX_X____	0xd0
#define	XX_X___X	0xd1
#define	XX_X__X_	0xd2
#define	XX_X__XX	0xd3
#define	XX_X_X__	0xd4
#define	XX_X_X_X	0xd5
#define	XX_X_XX_	0xd6
#define	XX_X_XXX	0xd7
#define	XX_XX___	0xd8
#define	XX_XX__X	0xd9
#define	XX_XX_X_	0xda
#define	XX_XX_XX	0xdb
#define	XX_XXX__	0xdc
#define	XX_XXX_X	0xdd
#define	XX_XXXX_	0xde
#define	XX_XXXXX	0xdf
#define	XXX_____	0xe0
#define	XXX____X	0xe1
#define	XXX___X_	0xe2
#define	XXX___XX	0xe3
#define	XXX__X__	0xe4
#define	XXX__X_X	0xe5
#define	XXX__XX_	0xe6
#define	XXX__XXX	0xe7
#define	XXX_X___	0xe8
#define	XXX_X__X	0xe9
#define	XXX_X_X_	0xea
#define	XXX_X_XX	0xeb
#define	XXX_XX__	0xec
#define	XXX_XX_X	0xed
#define	XXX_XXX_	0xee
#define	XXX_XXXX	0xef
#define	XXXX____	0xf0
#define	XXXX___X	0xf1
#define	XXXX__X_	0xf2
#define	XXXX__XX	0xf3
#define	XXXX_X__	0xf4
#define	XXXX_X_X	0xf5
#define	XXXX_XX_	0xf6
#define	XXXX_XXX	0xf7
#define	XXXXX___	0xf8
#define	XXXXX__X	0xf9
#define	XXXXX_X_	0xfa
#define	XXXXX_XX	0xfb
#define	XXXXXX__	0xfc
#define	XXXXXX_X	0xfd
#define	XXXXXXX_	0xfe
#define	XXXXXXXX	0xff


#endif   /* ifdef GUI_H */

/*************************** End of file ****************************/
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
