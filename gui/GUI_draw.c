
#include "type.h"
#include "pf_lcd.h"
#include <string.h>
#include "lib_gui.h"
#include "pf_lcd.h"
#include "utf8.h"
#include "mem.h"
#include "debug.h"
#include "pf_platform_cfg.h"

#ifndef CHARACTER_DIS_CODEC
    #error "must define CHARACTER_DIS_CODEC"
#endif





extern GUI_FONT GUI_Fontascii_16;

GUI_CONTEXT GUI_Context = {
    .ClipRect = { 0, 0, 800, 600 },
    .BkColor = C_BLUE,
    .Color = C_WHITE,
    .pAFont = &GUI_Fontascii_16,
};



void drawPix(uint16 x, uint16 y, COLOR color) {
    if (COLORA(color) != 0) {
        LCD_SetPixel(x, y, color2Lcd(color));
    }
}


void drawHLine(uint16 x, uint16 y, uint16 lineLen) {
    unsigned int color = color2Lcd(GUI_Context.Color);
#if LCD_PIX_SIZE==2
    memset16(&Pix(x, y), color, lineLen);
#elif LCD_PIX_SIZE==4
    memset32(&Pix(x, y), color, lineLen);
#else
#error
#endif
}


void drawHLineEx(uint16 x, uint16 y, uint16 lineLen, COLOR color) {
    unsigned int c = color2Lcd(color);
#if LCD_PIX_SIZE==2
    memset16(&Pix(x, y), c, lineLen);
#elif LCD_PIX_SIZE==4
    memset32(&Pix(x, y), c, lineLen);
#else
#error
#endif
}


void drawVLine(uint16 x, uint16 y, uint16 lineLen) {
    for (uint32 i = 0; i < lineLen; i++) {
        drawPix(x, y + i, GUI_Context.Color);
    }
}

void drawVLineEx(uint16 x, uint16 y, uint16 lineLen, COLOR color) {
    for (uint32 i = 0; i < lineLen; i++) {
        drawPix(x, y + i, color);
    }
}


void drawRect(uint16 x, uint16 y, uint16 width, uint16 heigh) {
    drawHLine(x, y, width);
    drawHLine(x, y + heigh - 1, width);
    drawVLine(x, y, heigh);
    drawVLine(x + width - 1, y, heigh);
}


void drawRectEx(uint16 x, uint16 y, uint16 width, uint16 heigh, COLOR color) {
    drawHLineEx(x, y, width, color);
    drawHLineEx(x, y + heigh - 1, width, color);
    drawVLineEx(x, y, heigh, color);
    drawVLineEx(x + width - 1, y, heigh, color);
}


void drawRectFill(uint32 x, uint32 y, uint32 width, uint32 height) {
    for (unsigned int i = y; i < (y + height); i++) {
        drawHLine(x, i, width);
    }
}


void drawRectFillEx(uint32 x, uint32 y, uint32 width, uint32 height, COLOR color) {
    for (unsigned int i = y; i < (y + height); i++) {
        drawHLineEx(x, i, width, color);
    }
}


void DrawCharUcs2(unsigned short character, unsigned int x, unsigned int y) {
    GUI_Context.DispPosX = x;
    GUI_Context.DispPosY = y;
    GUI_Context.pAFont->pfDispChar(character);
}

#if 0 //DRAW_ARABIC_STRING == 1

//#define bosi
#ifdef bosi
	#define albnum1 29
	#define albnum2 41
#else
	#define albnum1 23
	#define albnum2 35
#endif
const static uint16 Arbic_Position[][4] =  // first, last, middle, alone
{
    { 0xfe80, 0xfe80, 0xfe80, 0xfe80 },    // 0x621
    { 0xfe82, 0xfe81, 0xfe82, 0xfe81 },
    { 0xfe84, 0xfe83, 0xfe84, 0xfe83 },
    { 0xfe86, 0xfe85, 0xfe86, 0xfe85 },
    { 0xfe88, 0xfe87, 0xfe88, 0xfe87 },
    { 0xfe8a, 0xfe8b, 0xfe8c, 0xfe89 },
    { 0xfe8e, 0xfe8d, 0xfe8e, 0xfe8d },
    { 0xfe90, 0xfe91, 0xfe92, 0xfe8f },   // 0x628
    { 0xfe94, 0xfe93, 0xfe94, 0xfe93 },
    { 0xfe96, 0xfe97, 0xfe98, 0xfe95 },   // 0x62A
    { 0xfe9a, 0xfe9b, 0xfe9c, 0xfe99 },
    { 0xfe9e, 0xfe9f, 0xfea0, 0xfe9d },
    { 0xfea2, 0xfea3, 0xfea4, 0xfea1 },
    { 0xfea6, 0xfea7, 0xfea8, 0xfea5 },
    { 0xfeaa, 0xfea9, 0xfeaa, 0xfea9 },
    { 0xfeac, 0xfeab, 0xfeac, 0xfeab },   // 0x630
    { 0xfeae, 0xfead, 0xfeae, 0xfead },
    { 0xfeb0, 0xfeaf, 0xfeb0, 0xfeaf },
    { 0xfeb2, 0xfeb3, 0xfeb4, 0xfeb1 },
    { 0xfeb6, 0xfeb7, 0xfeb8, 0xfeb5 },
    { 0xfeba, 0xfebb, 0xfebc, 0xfeb9 },
    { 0xfebe, 0xfebf, 0xfec0, 0xfebd },
    { 0xfec2, 0xfec3, 0xfec4, 0xfec1 },
    { 0xfec6, 0xfec7, 0xfec8, 0xfec5 },  // 0x638
    { 0xfeca, 0xfecb, 0xfecc, 0xfec9 },
    { 0xfece, 0xfecf, 0xfed0, 0xfecd },  //0x63A
    { 0x63b, 0x63b, 0x63b, 0x63b },
    { 0x63c, 0x63c, 0x63c, 0x63c },
    { 0x63d, 0x63d, 0x63d, 0x63d },
    { 0x63e, 0x63e, 0x63e, 0x63e },
    { 0x63f, 0x63f, 0x63f, 0x63f },
    { 0x640, 0x640, 0x640, 0x640 },   // 0x640
    { 0xfed2, 0xfed3, 0xfed4, 0xfed1 },
    { 0xfed6, 0xfed7, 0xfed8, 0xfed5 },
    { 0xfeda, 0xfedb, 0xfedc, 0xfed9 },
    { 0xfede, 0xfedf, 0xfee0, 0xfedd },
    { 0xfee2, 0xfee3, 0xfee4, 0xfee1 },
    { 0xfee6, 0xfee7, 0xfee8, 0xfee5 },
    { 0xfeea, 0xfeeb, 0xfeec, 0xfee9 },
    { 0xfeee, 0xfeed, 0xfeee, 0xfeed },   // 0x648
    { 0xfef0, 0xfef3, 0xfef4, 0xfeef },
    { 0xfef2, 0xfef3, 0xfef4, 0xfef1 },   // 0x64A
        //以下为波斯语?比阿拉伯多6个
    {0xfb57, 0xfb58, 0xfb59, 0xfb56},   // 0x067e
	{0xfb7b, 0xfb7c, 0xfb7d, 0xfb7a},   // 0x0686
	{0xfb8b, 0xfb8a, 0xfb8b, 0xfb8a},   // 0x0698
	{0xfb8f, 0xfb90, 0xfb91, 0xfb8e},   // 0x06a9
	{0xfb93, 0xfb94, 0xfb95, 0xfb92},   // 0x06af
	{0xfbfd, 0xfbfe, 0xfbff, 0xfbfc},   // 0x06cc
};

const static uint16 theSet1[29] =
{
    0x626, 0x628, 0x62a, 0x62b, 0x62c, 0x62d, 0x62e, 0x633,
    0x634, 0x635, 0x636, 0x637, 0x638, 0x639, 0x63a, 0x641,
    0x642, 0x643, 0x644, 0x645, 0x646, 0x647, 0x64a,
    0x67e, 0x686, 0x698, 0x6a9, 0x6af, 0x6cc,//为波斯语?比阿拉伯多6个
};

const static uint16 theSet2[41] =
{
    0x622, 0x623, 0x624, 0x625, 0x626, 0x627, 0x628, 0x629,0x62a,
	0x62b, 0x62c, 0x62d, 0x62e, 0x62f, 0x630, 0x631, 0x632,0x633,
	0x634, 0x635, 0x636, 0x637, 0x638, 0x639, 0x63a, 0x641,0x642,
    0x643, 0x645, 0x644,  0x646, 0x647, 0x648,0x649, 0x64a,
    0x67e,0x686,0x698,0x6a9,0x6af,0x6cc,//后6个为波斯语?

};

const static uint16 arabic_specs[][2] =
{
    { 0xFEF5, 0xFEF6 },
    { 0xFEF7, 0xFEF8 },
    { 0xFEF9, 0xFEFA },
    { 0xFEFB, 0xFEFC },
};

const static uint16 arabic_specs2[4] =
{
    0x622, 0x623, 0x625, 0x627
};

static uint16 strArabicCvtBuf[1024];

static __arabiccvt(uint16 ustr){
    for (int n = 0;; n++,m++;) {
        character = ustr[n];
        chara[m] = ustr[n];
        //阿拉伯语转换
        if (character > 0x5ff && character < 0x700) {  //0x600-0x6ff  FB50-FDFF  FE70-FEFF
            k = 1;
            j = 0;
            alonef = 0;
            for (i = 0; i < 23; i++) {
                if (ustr[n] == theSet1[i]) {
                    chara[m] = Arbic_Position[ustr[n] - 0x621][3];
                    alonef = 1; break;
                }
            }
#ifdef bosi
            for (i = 0; i < 6; i++) {
                if (ustr[n] == theSet1[i + 23]) {
                    chara[m] = Arbic_Position[42 + i][3];
                    alonef = i + 2; break;
                }
            }
#endif
            for (i = 0; i < 35; i++) {
                if (ustr[n] == theSet2[i]) {
                    chara[m] = Arbic_Position[ustr[n] - 0x621][3];
                    alonef = 1; break;
                }
            }
            if (n != 0) {
                for (i = 0; i < albnum1; i++) {
                    if (ustr[n - 1] == theSet1[i]) {
                        if (alonef == 1) chara[m] = Arbic_Position[ustr[n] - 0x621][0];
                        else if (alonef > 1) chara[m] = Arbic_Position[40 + alonef][0];
                        else chara[m] = ustr[n];
                        j = 1; break;
                    }
                }
            }

            if ((n != (len - 1))) {
                for (i = 0; i < albnum2; i++) {
                    if (ustr[n + 1] == theSet2[i] && j == 0) {
                        if (alonef == 1) chara[m] = Arbic_Position[ustr[n] - 0x621][1];
                        else if (alonef > 1) chara[m] = Arbic_Position[40 + alonef][1];
                        else chara[m] = ustr[n];
                        break;
                    } else if (ustr[n + 1] == theSet2[i] && j == 1) {
                        if (alonef == 1) chara[m] = Arbic_Position[ustr[n] - 0x621][2];
                        else if (alonef > 1) chara[m] = Arbic_Position[40 + alonef][2];
                        else chara[m] = ustr[n];
                        break;
                    }
                }
            }
            if (ustr[n] == 0x644) {
                //if(ustr[n - 1] > 0x5ff && ustr[n - 1] < 0x700)//
                //	chara[m - 1] = Arbic_Position[ustr[n - 1] - 0x621][1];
                for (i = 0; i < 4; i++) {
                    if (ustr[n + 1] == arabic_specs2[i]) {
                        chara[m] = arabic_specs[i][j];
                        //chara[m + 1] = arabic_specs[i][j];
                        //m++;
                        n++;
                    }
                }
            }
        }
    }
}
#endif


void DrawCharUcs2Ex(unsigned short character, unsigned int x, unsigned int y, GUI_FONT *font, COLOR color_f, COLOR color_b) {
    GUI_CONTEXT oldcontex;
    GUI_SaveContext(&oldcontex);
    GUI_Context.Color = color_f;
    GUI_Context.BkColor = color_b;
    GUI_Context.DispPosX = x;
    GUI_Context.DispPosY = y;
    GUI_Context.pAFont = font;
    GUI_Context.pAFont->pfDispChar(character);
    GUI_RestoreContext(&oldcontex);
}


void drawString_ucs2(const wchar *wcs, unsigned int x, unsigned int y) {
    GUI_Context.DispPosX = x;
    while (*wcs) {
        DrawCharUcs2(*wcs++, GUI_Context.DispPosX, y);
    }
}


void drawString_utf8(const char *utf8string, unsigned x, unsigned int y) {
    unsigned short ucs2;
    unsigned char charoffset = 0;
    unsigned char signelcharlen = 0;
    GUI_Context.DispPosX = x;
    while (1) {
        signelcharlen = UTF8toUCS2(utf8string + charoffset, &ucs2);
        charoffset += signelcharlen;
        if (0 == signelcharlen) {
            break;
        }
        DrawCharUcs2(ucs2, GUI_Context.DispPosX, y);
    }
}

void drawString_ascii(const char *string, unsigned x, unsigned int y) {
    GUI_Context.DispPosX = x;
    while (*string) {
        DrawCharUcs2(*string++, GUI_Context.DispPosX, y);
    }
}


void drawString(const TEXTCHAR *text, unsigned int x, unsigned int y) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
    STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
    drawString_utf8(text, x, y);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
    STATIC_ASSERT(sizeof(TEXTCHAR) == 2);
    drawString_ucs2(text, x, y);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
    STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
    drawString_ascii(text, x, y);
#endif
}

void drawStringLen_ascii(const char *string, unsigned int x, unsigned int y, unsigned int len) {
    GUI_Context.DispPosX = x;
    while ((*string) && (len--)) {
        DrawCharUcs2(*string++, GUI_Context.DispPosX, y);
    }
}


void drawStringLen_ucs2(const wchar *wcs, unsigned int x, unsigned int y, unsigned int len) {
    GUI_Context.DispPosX = x;
    while ((*wcs) && (len--)) {
        DrawCharUcs2(*wcs++, GUI_Context.DispPosX, y);
    }
}


void drawStringLen_utf8(const char *utf8string, unsigned int x, unsigned int y, unsigned int len) {
    unsigned short ucs2;
    unsigned char charoffset = 0;
    unsigned char signelcharlen = 0;
    GUI_Context.DispPosX = x;
    while (1) {
        signelcharlen = UTF8toUCS2(utf8string + charoffset, &ucs2);
        charoffset += signelcharlen;
        if ((0 == signelcharlen) || (0 == len)) {
            break;
        }
        len--;
        DrawCharUcs2(ucs2, GUI_Context.DispPosX, y);
    }
}

void drawStringLen(const TEXTCHAR *text, unsigned int x, unsigned int y, unsigned int len) {
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
    STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
    drawStringLen_utf8(text, x, y, len);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
    STATIC_ASSERT(sizeof(TEXTCHAR) == 2);
    drawStringLen_ucs2(text, x, y, len);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
    STATIC_ASSERT(sizeof(TEXTCHAR) == 1);
    drawStringLen_ascii(text, x, y, len);
#endif
}

void drawStringEx_ucs2(const wchar *wcs, unsigned int x, unsigned int y, const GUI_FONT *font, COLOR color_f, COLOR color_b) {
    GUI_CONTEXT oldc;
    GUI_SaveContext(&oldc);
    GUI_Context.DispPosX = x;
    GUI_Context.DispPosY = y;
    GUI_Context.Color = color_f;
    GUI_Context.BkColor = color_b;
    GUI_Context.pAFont = font;
    drawString_ucs2(wcs, x, y);
    GUI_RestoreContext(&oldc);
}

void drawStringEx(const TEXTCHAR *text, unsigned int x, unsigned int y, const GUI_FONT *font, COLOR color_f, COLOR color_b) {
    GUI_CONTEXT oldc;
    GUI_SaveContext(&oldc);
    GUI_Context.DispPosX = x;
    GUI_Context.DispPosY = y;
    GUI_Context.Color = color_f;
    GUI_Context.BkColor = color_b;
    GUI_Context.pAFont = font;
    drawString(text, x, y);
    GUI_RestoreContext(&oldc);
}


void drawStringAlign_ucs2(const wchar *wcs, unsigned int align, unsigned short x, unsigned short y, unsigned short width, unsigned short height) {
    unsigned int widthtemp;
    unsigned short len[10];
    const wchar *strptr[10];
    unsigned int str_len = wcslen((wchar_t *)wcs);
    unsigned int strheight = getCurFontYSize();
    unsigned int line = height / strheight;
    const wchar *ptr = wcs;
    unsigned int lineindex = 0;
    for (int i = 0, j = 0; i < str_len; i++, j++) {
        widthtemp = getStringMetricWidthEx_ucs2(ptr, j + 1);
        if (i == (str_len - 1)) j++;
        if ((widthtemp > width) || (i == (str_len - 1))) {
            strptr[lineindex] = ptr;
            len[lineindex] = j;
            ptr++;
            j = 0;
            if (++lineindex == line) {
                break;
            }
        }
    }
    unsigned xtemp, ytemp = y + (height - strheight * lineindex) / 2;
    for (int i = 0; i < lineindex; i++) {
        unsigned int strwidth = getStringMetricWidthEx_ucs2(strptr[i], len[i]);
        if (ALIGN_LEFT == align) {
            xtemp = x;
        } else if (ALIGN_MIDDLE == align) {
            xtemp = x + width / 2 - strwidth / 2;
        } else {
            xtemp = x + width - strwidth;
        }
        drawStringLen_ucs2(strptr[i], xtemp, ytemp, len[i]);
        ytemp += strheight;
    }
}



void drawStringAlign(const TEXTCHAR *text, uint32 align, uint16 x, uint16 y, uint16 width, uint16 height) {
    unsigned int widthtemp;
    unsigned short len[10];
    const TEXTCHAR *strptr[10];
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
    unsigned int str_len = strLen_UTF8(text);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
    unsigned int str_len = strlen(text);
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
    unsigned int str_len = wcslen(text);
#endif
    unsigned int strheight = getCurFontYSize();
    unsigned int line = height / strheight;
    const TEXTCHAR *ptr = text;
    unsigned int lineindex = 0;
    unsigned int lineflag = 0;
    unsigned int itemp = 0;
    for (int i = 0, j = 1; i < str_len; i++, j++) {
        widthtemp = getStringMetricWidthEx(ptr, j);
        if (widthtemp > width) {
            if (i - itemp <= 1) break;
            itemp = i;
            lineflag = 1;
            j--; i--;
        } else if (i == (str_len - 1)) {
            lineflag = 1;
        }
        if (1 == lineflag) {
            lineflag = 0;
            strptr[lineindex] = ptr;
            len[lineindex] = j;
#if (CHARACTER_DIS_CODEC==UTF8_CODEC)
            ptr = strForward_UTF8(ptr, j);
#elif(CHARACTER_DIS_CODEC==ASCII_CODEC)
            ptr += j;
#elif(CHARACTER_DIS_CODEC==UCS16_CODEC)
            STATIC_ASSERT(sizeof(*ptr) == 2);
            ptr += j;
#endif
            j = 1;
            if (++lineindex == line) {
                break;
            }
        }
    }
    unsigned int xtemp, ytemp;
    unsigned char alignx = align & 0x0f;
    unsigned char aligny = align >> 4;

    switch (aligny) {
    case ALIGN_LEFT:
        ytemp = y;
        break;
    case ALIGN_MIDDLE:
        ytemp = y + (height - strheight * lineindex) / 2;
        break;
    case ALIGN_RIGHT:
        ytemp = y + height - strheight * lineindex;
        break;
    default:
        ytemp = y;
        break;
    }
    for (int i = 0; i < lineindex; i++) {
        unsigned int strwidth = getStringMetricWidthEx(strptr[i], len[i]);
        switch (alignx) {
        case ALIGN_LEFT:
            xtemp = x;
            break;
        case ALIGN_MIDDLE:
            xtemp = x + width / 2 - strwidth / 2;
            break;
        case ALIGN_RIGHT:
            xtemp = x + width - strwidth;
            break;
        default:
            xtemp = x;
            break;
        }
        drawStringLen(strptr[i], xtemp, ytemp, len[i]);
        ytemp += strheight;
    }
}



void drawStringAlignEx(const TEXTCHAR *text, uint32 align, uint16 x, uint16 y, uint16 width, uint16 height, const GUI_FONT *font, COLOR color_f, COLOR color_b) {
    GUI_CONTEXT old;
    GUI_SaveContext(&old);
    GUI_SetFont(font);
    GUI_SetBkColor(color_b);
    GUI_SetColor(color_f);
    GUI_Context.DispPosX = x;
    GUI_Context.DispPosY = y;
    GUI_Context.pAFont = font;
    drawStringAlign(text, align, x, y, width, height);
    GUI_RestoreContext(&old);
}

void drawStringAlignEx_ucs2(const wchar *wcs, uint32 align, uint16 x, uint16 y, uint16 width, uint16 height, const GUI_FONT *font, COLOR color_f, COLOR color_b) {
    GUI_CONTEXT old;
    GUI_SaveContext(&old);
    GUI_SetFont(font);
    GUI_SetBkColor(color_b);
    GUI_SetColor(color_f);
    GUI_Context.DispPosX = x;
    GUI_Context.DispPosY = y;
    GUI_Context.pAFont = font;
    drawStringAlign_ucs2(wcs, align, x, y, width, height);
    GUI_RestoreContext(&old);
}

/*********************************************************************
*
*       GUI_SetBkColor
*/
void GUI_SetBkColor(GUI_COLOR color) {
    GUI_Context.BkColor = color;
}


void GUI_SetColor(GUI_COLOR color) {
    GUI_Context.Color = color;
}


const GUI_FONT* GUI_SetFont(const GUI_FONT *font) {
    const GUI_FONT *old = GUI_Context.pAFont;
    GUI_Context.pAFont = font;
    return old;
}



void GUI_SaveContext(const GUI_CONTEXT *pContext) {
    memcpy((void *)pContext, &GUI_Context, sizeof GUI_Context);
}

/*********************************************************************
*
*       GUI_RestoreContext
*/
void GUI_RestoreContext(const GUI_CONTEXT *pContext) {
    memcpy(&GUI_Context, (void *)pContext, sizeof GUI_Context);
}

/*************************** End of file ****************************/








