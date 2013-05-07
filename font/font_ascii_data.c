
#ifndef __Asc8X16_dot_H
#define __Asc8X16_dot_H


const uint8 Hex8x10[]={
0x18,0x24,0x42,0x46,0x4A,0x52,0x62,0x24,0x18,0x01,		// 0.
0x10,0x30,0x10,0x10,0x10,0x10,0x10,0x10,0x38,0x01,		// 1.
0x38,0x44,0x02,0x02,0x04,0x18,0x20,0x42,0x7E,0x01,		// 2.
0x78,0x84,0x04,0x04,0x38,0x04,0x04,0x84,0x78,0x01,		// 3.
0x0C,0x14,0x24,0x44,0x84,0xFE,0x04,0x04,0x0E,0x01,		// 4.
0xFC,0x80,0x80,0xF8,0x04,0x04,0x04,0x84,0x78,0x01,		// 5.
0x38,0x40,0x80,0xB8,0xC4,0x84,0x84,0x84,0x78,0x01,		// 6.
0xFE,0x82,0x02,0x04,0x08,0x08,0x08,0x08,0x1C,0x01,		// 7.
0x7C,0x82,0x82,0x82,0x7C,0x82,0x82,0x82,0x7C,0x01,	// 8.
0x30,0x48,0x84,0x84,0x4C,0x34,0x04,0x88,0x70,0x01,		// 9.
0x10,0x28,0x44,0x82,0x82,0xFE,0x82,0x82,0x82,0x01,		// A.
0xFC,0x42,0x42,0x44,0x78,0x44,0x42,0x42,0xFC,0x01,		// B.
0x1C,0x22,0x40,0x40,0x40,0x40,0x40,0x22,0x1C,0x01,	// C.
0xF8,0x44,0x42,0x42,0x42,0x42,0x42,0x44,0xF8,0x01,		// D.
0xFE,0x42,0x40,0x40,0x7C,0x40,0x40,0x42,0xFE,0x01,		// E.
0xFE,0x42,0x40,0x40,0x7C,0x40,0x40,0x40,0xE0,0x01,		//F.
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,		// .
};
*/
const uint8 Asc8x16[1536] = {
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	//
0x00,0x00,0x18,0x3C,0x3C,0x3C,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,	// !
0x00,0x66,0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// "
0x00,0x00,0x00,0x6C,0x6C,0xFE,0x6C,0x6C,0x6C,0xFE,0x6C,0x6C,0x00,0x00,0x00,0x00,	// #
0x18,0x18,0x7C,0xC6,0xC2,0xC0,0x7C,0x06,0x86,0xC6,0x7C,0x18,0x18,0x00,0x00,0x00,	// $
0x00,0x00,0x00,0x00,0xC2,0xC6,0x0C,0x18,0x30,0x60,0xC6,0x86,0x00,0x00,0x00,0x00,	// % [5]
0x00,0x00,0x38,0x6C,0x6C,0x38,0x76,0xDC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00,	// &
0x00,0x30,0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// '
0x00,0x00,0x0C,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0x18,0x0C,0x00,0x00,0x00,0x00,	// (
0x00,0x00,0x30,0x18,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x18,0x30,0x00,0x00,0x00,0x00,	// )

0x00,0x00,0x00,0x00,0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,0x00,0x00,0x00,0x00,	// * [10]
0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,0x00,0x00,0x00,0x00,	// +
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x18,0x30,0x00,0x00,0x00,	// ,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,	// -
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,	// . 
0x00,0x00,0x00,0x00,0x02,0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00,0x00,0x00,0x00,	// / [15]
0x00,0x00,0x7C,0xC6,0xC6,0xCE,0xD6,0xD6,0xE6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,	// 0 [16]
0x00,0x00,0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00,0x00,0x00,	// 1
0x00,0x00,0x7C,0xC6,0x06,0x0C,0x18,0x30,0x60,0xC0,0xC6,0xFE,0x00,0x00,0x00,0x00,	// 2
0x00,0x00,0x7C,0xC6,0x06,0x06,0x3C,0x06,0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00,	// 3

0x00,0x00,0x0C,0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x0C,0x0C,0x1E,0x00,0x00,0x00,0x00,	// 4 [20]
0x00,0x00,0xFE,0xC0,0xC0,0xC0,0xFC,0x0E,0x06,0x06,0xC6,0x7C,0x00,0x00,0x00,0x00,	// 5
0x00,0x00,0x38,0x60,0xC0,0xC0,0xFC,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,	// 6
0x00,0x00,0xFE,0xC6,0x06,0x06,0x0C,0x18,0x30,0x30,0x30,0x30,0x00,0x00,0x00,0x00,	// 7
0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7C,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,	// 8
0x00,0x00,0x7C,0xC6,0xC6,0xC6,0x7E,0x06,0x06,0x06,0x0C,0x78,0x00,0x00,0x00,0x00,	// 9 [25]
0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00,0x00,	// :
0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x30,0x00,0x00,0x00,0x00,	// ;
0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,0x00,0x00,	// <
0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x00,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,	// =

0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0x00,0x00,0x00,0x00,	// > [30]
0x00,0x00,0x7C,0xC6,0xC6,0x0C,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00,0x00,	// ?
0x00,0x00,0x00,0x7C,0xC6,0xC6,0xDE,0xDE,0xDE,0xDC,0xC0,0x7C,0x00,0x00,0x00,0x00,	// @
0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00,	// A [33]
/*B    */
0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x66,0x66,0x66,0x66,0xFC,0x00,0x00,0x00,0x00,
/*C    */
0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xC0,0xC0,0xC2,0x66,0x3C,0x00,0x00,0x00,0x00,
/*D    */
0x00,0x00,0xF8,0x6C,0x66,0x66,0x66,0x66,0x66,0x66,0x6C,0xF8,0x00,0x00,0x00,0x00,
/*E    */
0x00,0x00,0xFE,0x66,0x62,0x68,0x78,0x68,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00,
/*F    */
0x00,0x00,0xFE,0x66,0x62,0x68,0x78,0x68,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00,
/*G    */
0x00,0x00,0x3C,0x66,0xC2,0xC0,0xC0,0xDE,0xC6,0xC6,0x66,0x3A,0x00,0x00,0x00,0x00,
/*H    */
0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xFE,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00,
/*I    */
0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,
/*J    */
0x00,0x00,0x1E,0x0C,0x0C,0x0C,0x0C,0x0C,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,0x00,
/*K    */
0x00,0x00,0xE6,0x66,0x6C,0x6C,0x78,0x78,0x6C,0x66,0x66,0xE6,0x00,0x00,0x00,0x00,
/*L    */
0x00,0x00,0xF0,0x60,0x60,0x60,0x60,0x60,0x60,0x62,0x66,0xFE,0x00,0x00,0x00,0x00,
/*M    */
0x00,0x00,0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00,
/*N    */
0x00,0x00,0xC6,0xE6,0xF6,0xFE,0xDE,0xCE,0xC6,0xC6,0xC6,0xC6,0x00,0x00,0x00,0x00,
/*O    */
0x00,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0x00,0x00,0x00,
/*P    */
0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x60,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00,
/*Q    */
0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xD6,0xDE,0x7C,0x0C,0x0E,0x00,0x00,
/*R    */
0x00,0x00,0xFC,0x66,0x66,0x66,0x7C,0x6C,0x66,0x66,0x66,0xE6,0x00,0x00,0x00,0x00,
/*S    */
0x00,0x00,0x7C,0xC6,0xC6,0x60,0x38,0x0C,0x06,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,
/*T    */
0x00,0x00,0x7E,0x7E,0x5A,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,
/*U    */
0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,
/*V    */
0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x6C,0x38,0x10,0x00,0x00,0x00,0x00,
/*W    */
0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xD6,0xD6,0xFE,0x6C,0x6C,0x00,0x00,0x00,0x00,
/*X    */
0x00,0x00,0xC6,0xC6,0x6C,0x6C,0x38,0x38,0x6C,0x6C,0xC6,0xC6,0x00,0x00,0x00,0x00,
/*Y    */
0x00,0x00,0x66,0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,
/*Z    */
0x00,0x00,0xFE,0xC6,0x86,0x0C,0x18,0x30,0x60,0xC2,0xC6,0xFE,0x00,0x00,0x00,0x00,
/*[    */
0x00,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,0x00,0x00,0x00,
/*\    */
0x00,0x00,0x00,0x80,0xC0,0xE0,0x70,0x38,0x1C,0x0E,0x06,0x02,0x00,0x00,0x00,0x00,
/*]    */
0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,0x00,0x00,0x00,
/*^    */
0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*_    */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,
/*`    */
0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
/*a    */
0x00,0x00,0x00,0x00,0x00,0x78,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00,
/*b    */
0x00,0x00,0xE0,0x60,0x60,0x78,0x6C,0x66,0x66,0x66,0x66,0xDC,0x00,0x00,0x00,0x00,
/*c    */
0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC0,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00,
/*d    */
0x00,0x00,0x1C,0x0C,0x0C,0x3C,0x6C,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00,
/*e    */
0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xFE,0xC0,0xC0,0xC6,0x7C,0x00,0x00,0x00,0x00,
/*f    */
0x00,0x00,0x38,0x6C,0x64,0x60,0xF0,0x60,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00,
/*g    */
0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0xCC,0xCC,0xCC,0x7C,0x0C,0xCC,0x78,0x00,
/*h    */
0x00,0x00,0xE0,0x60,0x60,0x6C,0x76,0x66,0x66,0x66,0x66,0xE6,0x00,0x00,0x00,0x00,
/*i    */
0x00,0x00,0x18,0x18,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,
/*j    */
0x00,0x00,0x06,0x06,0x00,0x0E,0x06,0x06,0x06,0x06,0x06,0x06,0x66,0x66,0x3C,0x00,
/*k    */
0x00,0x00,0xE0,0x60,0x60,0x66,0x6C,0x78,0x78,0x6C,0x66,0xE6,0x00,0x00,0x00,0x00,
/*l    */
0x00,0x00,0x38,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00,0x00,
/*m    */
0x00,0x00,0x00,0x00,0x00,0xEC,0xFE,0xD6,0xD6,0xD6,0xD6,0xD6,0x00,0x00,0x00,0x00,
/*n    */
0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00,0x00,
/*o    */
0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0xC6,0xC6,0xC6,0xC6,0x7C,0x00,0x00,0x00,0x00,
/*p    */
0x00,0x00,0x00,0x00,0x00,0xDC,0x66,0x66,0x66,0x66,0x66,0x7C,0x60,0x60,0xF0,0x00,
/*q    */
0x00,0x00,0x00,0x00,0x00,0x76,0xCC,0xCC,0xCC,0xCC,0xCC,0x7C,0x0C,0x0C,0x1E,0x00,
/*r    */
0x00,0x00,0x00,0x00,0x00,0xDC,0x76,0x62,0x60,0x60,0x60,0xF0,0x00,0x00,0x00,0x00,
/*s    */
0x00,0x00,0x00,0x00,0x00,0x7C,0xC6,0x60,0x38,0x0C,0xC6,0x7C,0x00,0x00,0x00,0x00,
/*t    */
0x00,0x00,0x10,0x30,0x30,0xFC,0x30,0x30,0x30,0x30,0x36,0x1C,0x00,0x00,0x00,0x00,
/*u    */
0x00,0x00,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0x00,
/*v    */
0x00,0x00,0x00,0x00,0x00,0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00,0x00,0x00,0x00,
/*w    */
0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xC6,0xD6,0xD6,0xFE,0x6C,0x00,0x00,0x00,0x00,
/*x    */
0x00,0x00,0x00,0x00,0x00,0xC6,0x6C,0x38,0x38,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,

/*y    */
0x00,0x00,0x00,0x00,0x00,0xC6,0xC6,0xC6,0xC6,0xC6,0xC6,0x7E,0x06,0x0C,0xF8,0x00,

/*z    */
0x00,0x00,0x00,0x00,0x00,0xFE,0xCC,0x18,0x30,0x60,0xC6,0xFE,0x00,0x00,0x00,0x00,

/*{    */
0x00,0x00,0x0E,0x18,0x18,0x18,0x70,0x18,0x18,0x18,0x18,0x0E,0x00,0x00,0x00,0x00,

/*|    */
0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00,0x00,
/*}    */
0x00,0x00,0x70,0x18,0x18,0x18,0x0E,0x18,0x18,0x18,0x18,0x70,0x00,0x00,0x00,0x00,
/*~    */
0x00,0x00,0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//
0x00,0x00,0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xC6,0xFE,0x00,0x00,0x00,0x00,0x00,
};


const uint8 Asc12x24[4608] = {
/*    CHAR_20 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*!   CHAR_21 */
0x00,0x00,0x02,0x00,0x07,0x00,0x07,0x00,
0x07,0x00,0x07,0x00,0x07,0x00,0x07,0x00,
0x07,0x00,0x07,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x00,0x00,0x00,0x00,0x02,0x00,
0x07,0x00,0x07,0x00,0x02,0x00,0x00,0x00,

/*"   CHAR_22 */
0x00,0x00,0x00,0x00,0x39,0xC0,0x39,0xC0,
0x39,0xC0,0x10,0x80,0x10,0x80,0x10,0x80,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*#   CHAR_23 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x01,0x80,0x31,0x80,0x31,0x80,0x31,0xE0,
0x3F,0x80,0xF1,0x80,0x31,0x80,0x31,0x80,
0x31,0x80,0x31,0xE0,0x3F,0x80,0xF1,0x80,
0x31,0x80,0x31,0x80,0x31,0x80,0x31,0x80,
0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*$   CHAR_24 */
0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,
0x3F,0xC0,0x66,0x60,0xC6,0x30,0xC6,0x30,
0xC6,0x00,0xC6,0x00,0x66,0x00,0x3E,0x00,
0x0F,0x80,0x07,0xC0,0x06,0x60,0x06,0x30,
0xC6,0x30,0xC6,0x30,0xC6,0x30,0x66,0x60,
0x3F,0xC0,0x06,0x00,0x06,0x00,0x00,0x00,

/*%   CHAR_25 */
0x00,0x00,0x00,0x00,0x30,0x00,0x48,0x20,
0xCC,0x60,0xCC,0x40,0xCC,0xC0,0xCC,0x80,
0xCD,0x80,0x4B,0x00,0x32,0x00,0x06,0x00,
0x04,0x00,0x0C,0x00,0x09,0x80,0x1A,0x40,
0x36,0x60,0x26,0x60,0x66,0x60,0x46,0x60,
0xC6,0x60,0x82,0x40,0x01,0x80,0x00,0x00,

/*&   CHAR_26 */
0x00,0x00,0x00,0x00,0x1C,0x00,0x22,0x00,
0x42,0x00,0x42,0x00,0x42,0x00,0x64,0x00,
0x28,0x00,0x30,0x00,0x38,0x00,0x48,0x00,
0x4C,0x00,0xC4,0x00,0xC6,0x70,0xC3,0x20,
0xC3,0x20,0xC1,0xC0,0xC0,0xC0,0xC1,0xC0,
0x67,0x60,0x3C,0x30,0x00,0x00,0x00,0x00,

/*'   CHAR_27 */
0x00,0x00,0x00,0x00,0x07,0x00,0x07,0x00,
0x07,0x00,0x07,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*(   CHAR_28 */
0x00,0x20,0x00,0x40,0x00,0x80,0x01,0x80,
0x01,0x00,0x03,0x00,0x03,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x03,0x00,0x03,0x00,0x01,0x00,
0x01,0x80,0x00,0x80,0x00,0x40,0x00,0x20,

/*)   CHAR_29 */
0x40,0x00,0x20,0x00,0x10,0x00,0x18,0x00,
0x08,0x00,0x0C,0x00,0x0C,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x0C,0x00,0x0C,0x00,0x08,0x00,
0x18,0x00,0x10,0x00,0x20,0x00,0x40,0x00,

/**   CHAR_2A */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x04,0x00,0x0E,0x00,0x0E,0x00,0xC4,0x60,
0xE4,0xE0,0x64,0xC0,0x11,0x00,0x00,0x00,
0x00,0x00,0x11,0x00,0x64,0xC0,0xE4,0xE0,
0xC4,0x60,0x0E,0x00,0x0E,0x00,0x04,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*+   CHAR_2B */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x7F,0xE0,
0x7F,0xE0,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*,   CHAR_2C */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x0C,0x00,0x1E,0x00,0x1E,0x00,
0x0E,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,

/*-   CHAR_2D */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0xE0,
0x7F,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*.   CHAR_2E */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0C,0x00,0x1E,0x00,
0x1E,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,

/*/   CHAR_2F */
0x00,0x00,0x00,0x20,0x00,0x60,0x00,0x40,
0x00,0xC0,0x00,0x80,0x01,0x80,0x01,0x00,
0x03,0x00,0x02,0x00,0x06,0x00,0x04,0x00,
0x0C,0x00,0x08,0x00,0x18,0x00,0x10,0x00,
0x30,0x00,0x20,0x00,0x60,0x00,0x40,0x00,
0xC0,0x00,0x80,0x00,0x00,0x00,0x00,0x00,

/*0   CHAR_30 */
0x00,0x00,0x00,0x00,0x0E,0x00,0x1B,0x00,
0x31,0x80,0x31,0x80,0x60,0xC0,0x60,0xC0,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0x60,0xC0,0x60,0xC0,0x31,0x80,0x31,0x80,
0x1B,0x00,0x0E,0x00,0x00,0x00,0x00,0x00,

/*1   CHAR_31 */
0x00,0x00,0x00,0x00,0x06,0x00,0x1E,0x00,
0x7E,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x3F,0xC0,0x00,0x00,0x00,0x00,

/*2   CHAR_32 */
0x00,0x00,0x00,0x00,0x1F,0x00,0x31,0x80,
0x60,0xC0,0xE0,0x60,0xE0,0x60,0x60,0x60,
0x00,0x60,0x00,0xC0,0x00,0xC0,0x01,0x80,
0x03,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,
0x30,0x00,0x70,0x00,0x60,0x20,0xE0,0x20,
0xFF,0xE0,0xFF,0xE0,0x00,0x00,0x00,0x00,

/*3   CHAR_33 */
0x00,0x00,0x00,0x00,0x0F,0x00,0x39,0xC0,
0x70,0xE0,0x70,0x60,0x30,0x60,0x00,0x60,
0x00,0xC0,0x01,0x80,0x03,0x00,0x0E,0x00,
0x03,0x80,0x00,0xC0,0x00,0x60,0x00,0x60,
0x00,0x60,0x60,0x60,0xE0,0x60,0xE0,0xE0,
0x71,0xC0,0x3F,0x00,0x00,0x00,0x00,0x00,

/*4   CHAR_34 */
0x00,0x00,0x00,0x00,0x01,0x80,0x03,0x80,
0x03,0x80,0x07,0x80,0x05,0x80,0x0D,0x80,
0x09,0x80,0x19,0x80,0x11,0x80,0x31,0x80,
0x21,0x80,0x61,0x80,0x41,0x80,0xC1,0x80,
0xFF,0xF0,0x01,0x80,0x01,0x80,0x01,0x80,
0x01,0x80,0x07,0xE0,0x00,0x00,0x00,0x00,

/*5   CHAR_35 */
0x00,0x00,0x00,0x00,0x7F,0xC0,0x7F,0xC0,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x67,0x00,0x69,0x80,0x70,0xC0,
0x40,0xC0,0x00,0x60,0x00,0x60,0x00,0x60,
0x00,0x60,0x60,0x60,0xE0,0xC0,0xE0,0xC0,
0x71,0x80,0x3F,0x00,0x00,0x00,0x00,0x00,

/*6   CHAR_36 */
0x00,0x00,0x00,0x00,0x1F,0x00,0x33,0x80,
0x61,0xC0,0x61,0xC0,0xC1,0x80,0xC0,0x00,
0xC0,0x00,0xCF,0x00,0xD1,0x80,0xE0,0xC0,
0xC0,0xC0,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0x60,0xC0,0x60,0xC0,
0x31,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,

/*7   CHAR_37 */
0x00,0x00,0x00,0x00,0x7F,0xE0,0xFF,0xE0,
0xC0,0x60,0x80,0x40,0x80,0xC0,0x00,0x80,
0x01,0x80,0x01,0x00,0x03,0x00,0x03,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x0C,0x00,
0x0C,0x00,0x0C,0x00,0x0C,0x00,0x0C,0x00,
0x0C,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,

/*8   CHAR_38 */
0x00,0x00,0x00,0x00,0x1F,0x00,0x31,0x80,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x31,0x80,0x1B,0x00,0x0E,0x00,0x1F,0x00,
0x31,0x80,0x60,0xC0,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0x60,0xC0,
0x31,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,

/*9   CHAR_39 */
0x00,0x00,0x00,0x00,0x1F,0x00,0x31,0x80,
0x60,0xC0,0x60,0xC0,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0x60,0x60,
0x60,0xE0,0x31,0x60,0x1E,0x60,0x00,0x60,
0x00,0x60,0x30,0x60,0x70,0xC0,0x70,0xC0,
0x39,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,

/*:   CHAR_3A */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x00,
0x0F,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x00,
0x0F,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*;   CHAR_3B */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x00,
0x0F,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x00,
0x0F,0x00,0x07,0x00,0x03,0x00,0x06,0x00,
0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*<   CHAR_3C */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,
0x00,0xC0,0x01,0x80,0x03,0x00,0x06,0x00,
0x0C,0x00,0x18,0x00,0x30,0x00,0x60,0x00,
0xC0,0x00,0x60,0x00,0x30,0x00,0x18,0x00,
0x0C,0x00,0x06,0x00,0x03,0x00,0x01,0x80,
0x00,0xC0,0x00,0x60,0x00,0x00,0x00,0x00,

/*=   CHAR_3D */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x7F,0xE0,0x7F,0xE0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x7F,0xE0,0x7F,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*>   CHAR_3E */
0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x00,
0x60,0x00,0x30,0x00,0x18,0x00,0x0C,0x00,
0x06,0x00,0x03,0x00,0x01,0x80,0x00,0xC0,
0x00,0x60,0x00,0xC0,0x01,0x80,0x03,0x00,
0x06,0x00,0x0C,0x00,0x18,0x00,0x30,0x00,
0x60,0x00,0xC0,0x00,0x00,0x00,0x00,0x00,

/*?   CHAR_3F */
0x00,0x00,0x00,0x00,0x0F,0x80,0x18,0xC0,
0x30,0x60,0x70,0x60,0x70,0x60,0x20,0x60,
0x00,0xC0,0x00,0xC0,0x01,0x80,0x01,0x00,
0x03,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x00,0x00,0x00,0x00,0x02,0x00,
0x07,0x00,0x07,0x00,0x02,0x00,0x00,0x00,

/*@   CHAR_40 */
0x00,0x00,0x00,0x00,0x1F,0x00,0x20,0x80,
0x40,0x40,0x80,0x20,0x8F,0x20,0x99,0xA0,
0xB1,0xA0,0xB1,0xA0,0xB1,0xA0,0xB1,0xA0,
0xB1,0xA0,0xB1,0xA0,0xB1,0xA0,0xBF,0xA0,
0x9C,0xC0,0x80,0x00,0x80,0x00,0x40,0x20,
0x20,0x40,0x1F,0x80,0x00,0x00,0x00,0x00,

/*A   CHAR_41 */
0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,
0x0B,0x00,0x0B,0x00,0x09,0x00,0x09,0x00,
0x19,0x80,0x19,0x80,0x10,0x80,0x10,0x80,
0x30,0xC0,0x30,0xC0,0x3F,0xC0,0x30,0xC0,
0x20,0x40,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0xF0,0xF0,0x00,0x00,0x00,0x00,

/*B   CHAR_42 */
0x00,0x00,0x00,0x00,0xFE,0x00,0x63,0x00,
0x61,0x80,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x61,0x80,0x63,0x00,0x7E,0x00,
0x63,0x80,0x60,0xC0,0x60,0xC0,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0xC0,0x60,0xC0,
0x61,0x80,0xFF,0x00,0x00,0x00,0x00,0x00,

/*C   CHAR_43 */
0x00,0x00,0x00,0x00,0x1F,0x20,0x31,0xE0,
0x60,0xE0,0x60,0x60,0xC0,0x20,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC0,0x00,
0xC0,0x00,0xC0,0x60,0x60,0x60,0x60,0xC0,
0x31,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,

/*D   CHAR_44 */
0x00,0x00,0x00,0x00,0xFE,0x00,0x63,0x00,
0x61,0x80,0x60,0xC0,0x60,0xC0,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0xC0,0x60,0xC0,0x61,0x80,
0x63,0x00,0xFE,0x00,0x00,0x00,0x00,0x00,

/*E   CHAR_45 */
0x00,0x00,0x00,0x00,0xFF,0xE0,0x60,0xE0,
0x60,0x60,0x60,0x20,0x60,0x20,0x60,0x00,
0x61,0x00,0x61,0x00,0x63,0x00,0x7F,0x00,
0x63,0x00,0x61,0x00,0x61,0x00,0x60,0x00,
0x60,0x00,0x60,0x20,0x60,0x20,0x60,0x60,
0x60,0xE0,0xFF,0xE0,0x00,0x00,0x00,0x00,

/*F   CHAR_46 */
0x00,0x00,0x00,0x00,0xFF,0xE0,0x60,0xE0,
0x60,0x60,0x60,0x20,0x60,0x20,0x60,0x00,
0x61,0x00,0x61,0x00,0x63,0x00,0x7F,0x00,
0x63,0x00,0x61,0x00,0x61,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,

/*G   CHAR_47 */
0x00,0x00,0x00,0x00,0x1E,0x40,0x33,0xC0,
0x60,0xC0,0x60,0x40,0xC0,0x40,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0xC0,0x00,0xC1,0xE0,
0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0xC0,0xC0,0xC0,0xC0,0x61,0xC0,0x63,0xC0,
0x36,0xC0,0x1C,0xC0,0x00,0x00,0x00,0x00,

/*H   CHAR_48 */
0x00,0x00,0x00,0x00,0xF1,0xE0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x7F,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0xF1,0xE0,0x00,0x00,0x00,0x00,

/*I   CHAR_49 */
0x00,0x00,0x00,0x00,0x1F,0xE0,0x03,0x00,
0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,
0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,
0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,
0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x00,
0x03,0x00,0x1F,0xE0,0x00,0x00,0x00,0x00,

/*J   CHAR_4A */
0x00,0x00,0x00,0x00,0x07,0xE0,0x01,0x80,
0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
0x61,0x80,0xF1,0x80,0xE1,0x80,0xC1,0x80,
0x67,0x00,0x3E,0x00,0x00,0x00,0x00,0x00,

/*K   CHAR_4B */
0x00,0x00,0x00,0x00,0xF3,0xE0,0x61,0x80,
0x61,0x80,0x63,0x00,0x63,0x00,0x66,0x00,
0x66,0x00,0x6C,0x00,0x6C,0x00,0x7C,0x00,
0x7C,0x00,0x76,0x00,0x76,0x00,0x63,0x00,
0x63,0x00,0x61,0x80,0x61,0x80,0x60,0xC0,
0x60,0xC0,0xF1,0xF0,0x00,0x00,0x00,0x00,

/*L   CHAR_4C */
0x00,0x00,0x00,0x00,0xF0,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x20,0x60,0x20,0x60,0x60,
0x60,0xE0,0xFF,0xE0,0x00,0x00,0x00,0x00,

/*M   CHAR_4D */
0x00,0x00,0x00,0x00,0xE0,0x70,0x60,0x60,
0x60,0x60,0x60,0x60,0x70,0xE0,0x70,0xE0,
0x70,0xE0,0x70,0xE0,0x79,0xE0,0x69,0x60,
0x69,0x60,0x6F,0x60,0x66,0x60,0x66,0x60,
0x66,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0xF0,0xF0,0x00,0x00,0x00,0x00,

/*N   CHAR_4E */
0x00,0x00,0x00,0x00,0xE1,0xE0,0x60,0xC0,
0x60,0xC0,0x70,0xC0,0x70,0xC0,0x70,0xC0,
0x78,0xC0,0x68,0xC0,0x6C,0xC0,0x64,0xC0,
0x66,0xC0,0x62,0xC0,0x63,0xC0,0x61,0xC0,
0x61,0xC0,0x61,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0x40,0xF0,0x40,0x00,0x00,0x00,0x00,

/*O   CHAR_4F */
0x00,0x00,0x00,0x00,0x1F,0x00,0x31,0x80,
0x60,0xC0,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0x60,0xC0,
0x31,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,

/*P   CHAR_50 */
0x00,0x00,0x00,0x00,0xFE,0x00,0x63,0x80,
0x60,0xC0,0x60,0xC0,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0xC0,0x60,0xC0,0x63,0x80,
0x7E,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,

/*Q   CHAR_51 */
0x00,0x00,0x00,0x00,0x1F,0x00,0x31,0x80,
0x60,0xC0,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xCC,0x60,0xC6,0x60,0x63,0x60,
0x31,0xC0,0x1E,0xC0,0x00,0x70,0x00,0x00,

/*R   CHAR_52 */
0x00,0x00,0x00,0x00,0xFE,0x00,0x63,0x80,
0x61,0x80,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x61,0x80,0x63,0x80,0x7E,0x00,
0x6C,0x00,0x6C,0x00,0x66,0x00,0x66,0x00,
0x63,0x00,0x63,0x00,0x61,0x80,0x61,0x80,
0x60,0xC0,0xF1,0xE0,0x00,0x00,0x00,0x00,

/*S   CHAR_53 */
0x00,0x00,0x00,0x00,0x1E,0x40,0x73,0xC0,
0xC1,0xC0,0xC0,0xC0,0xC0,0x40,0xE0,0x00,
0x70,0x00,0x38,0x00,0x1C,0x00,0x0E,0x00,
0x07,0x00,0x03,0x80,0x01,0xC0,0x00,0xE0,
0x80,0x60,0xC0,0x60,0xE0,0x60,0xF0,0xC0,
0xD9,0x80,0x8F,0x00,0x00,0x00,0x00,0x00,

/*T   CHAR_54 */
0x00,0x00,0x00,0x00,0xFF,0xF0,0xE6,0x70,
0xC6,0x30,0x86,0x10,0x86,0x10,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,

/*U   CHAR_55 */
0x00,0x00,0x00,0x00,0xF1,0xE0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x31,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,

/*V   CHAR_56 */
0x00,0x00,0x00,0x00,0xF1,0xE0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x31,0x80,0x31,0x80,0x31,0x80,0x31,0x80,
0x31,0x80,0x31,0x80,0x1B,0x00,0x1B,0x00,
0x1B,0x00,0x1B,0x00,0x0E,0x00,0x0E,0x00,
0x0E,0x00,0x04,0x00,0x00,0x00,0x00,0x00,

/*W   CHAR_57 */
0x00,0x00,0x00,0x00,0xF0,0xF0,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0x60,0x26,0x40,0x26,0x40,
0x26,0x40,0x26,0x40,0x26,0x40,0x36,0xC0,
0x36,0xC0,0x36,0xC0,0x19,0x80,0x19,0x80,
0x19,0x80,0x19,0x80,0x00,0x00,0x00,0x00,

/*X   CHAR_58 */
0x00,0x00,0x00,0x00,0xF0,0xF0,0x60,0x60,
0x60,0x60,0x30,0xC0,0x30,0xC0,0x19,0x80,
0x19,0x80,0x0F,0x00,0x0F,0x00,0x06,0x00,
0x06,0x00,0x0F,0x00,0x0F,0x00,0x19,0x80,
0x19,0x80,0x30,0xC0,0x30,0xC0,0x60,0x60,
0x60,0x60,0xF0,0xF0,0x00,0x00,0x00,0x00,

/*Y   CHAR_59 */
0x00,0x00,0x00,0x00,0xF0,0xF0,0x60,0x60,
0x60,0x60,0x30,0xC0,0x30,0xC0,0x30,0xC0,
0x19,0x80,0x19,0x80,0x19,0x80,0x0F,0x00,
0x0F,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,

/*Z   CHAR_5A */
0x00,0x00,0x00,0x00,0xFF,0xC0,0xE0,0xC0,
0xC1,0xC0,0x81,0x80,0x83,0x00,0x03,0x00,
0x06,0x00,0x06,0x00,0x0C,0x00,0x0C,0x00,
0x18,0x00,0x18,0x00,0x30,0x00,0x30,0x00,
0x60,0x00,0x60,0x20,0xC0,0x20,0xC0,0x60,
0xC0,0xE0,0xFF,0xE0,0x00,0x00,0x00,0x00,

/*[   CHAR_5B */
0x07,0xE0,0x04,0x00,0x04,0x00,0x04,0x00,
0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,
0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,
0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,
0x04,0x00,0x04,0x00,0x04,0x00,0x04,0x00,
0x04,0x00,0x04,0x00,0x04,0x00,0x07,0xE0,

/*\   CHAR_5C */
0x00,0x00,0x80,0x00,0xC0,0x00,0x40,0x00,
0x60,0x00,0x20,0x00,0x30,0x00,0x10,0x00,
0x18,0x00,0x08,0x00,0x0C,0x00,0x04,0x00,
0x06,0x00,0x02,0x00,0x03,0x00,0x01,0x00,
0x01,0x80,0x00,0x80,0x00,0xC0,0x00,0x40,
0x00,0x60,0x00,0x20,0x00,0x00,0x00,0x00,

/*]   CHAR_5D */
0x7E,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x02,0x00,
0x02,0x00,0x02,0x00,0x02,0x00,0x7E,0x00,

/*^   CHAR_5E */
0x00,0x00,0x06,0x00,0x0F,0x00,0x19,0x80,
0x30,0xC0,0x60,0x60,0x40,0x20,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*_   CHAR_5F */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xF0,

/*`   CHAR_60 */
0x18,0x00,0x3C,0x00,0x0F,0x00,0x01,0xC0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

/*a   CHAR_61 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x3F,0x00,0x61,0x80,0xF0,0xC0,
0x60,0xC0,0x00,0xC0,0x0F,0xC0,0x38,0xC0,
0x60,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0xE3,0xC0,0x3E,0x60,0x00,0x00,0x00,0x00,

/*b   CHAR_62 */
0x00,0x00,0x00,0x00,0xE0,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x6F,0x00,0x79,0x80,0x70,0xC0,
0x60,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0x60,0x60,0x60,0x60,0xC0,0x70,0xC0,
0x79,0x80,0xCF,0x00,0x00,0x00,0x00,0x00,

/*c   CHAR_63 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x1F,0x00,0x31,0xC0,0x60,0xE0,
0x61,0xE0,0xC0,0xC0,0xC0,0x00,0xC0,0x00,
0xC0,0x00,0xC0,0x00,0x60,0x00,0x60,0x60,
0x30,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00,

/*d   CHAR_64 */
0x00,0x00,0x00,0x00,0x00,0xE0,0x00,0x60,
0x00,0x60,0x00,0x60,0x00,0x60,0x00,0x60,
0x00,0x60,0x1F,0x60,0x31,0xE0,0x60,0xE0,
0x60,0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0x60,0x60,0x60,0xE0,
0x31,0xE0,0x1F,0x20,0x00,0x00,0x00,0x00,

/*e   CHAR_65 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x1F,0x00,0x31,0x80,0x60,0xC0,
0x60,0xC0,0xC0,0x60,0xC0,0x60,0xFF,0xE0,
0xC0,0x00,0xC0,0x00,0x60,0x00,0x60,0x60,
0x30,0xC0,0x1F,0x80,0x00,0x00,0x00,0x00,

/*f   CHAR_66 */
0x00,0x00,0x00,0x00,0x07,0xC0,0x0C,0xE0,
0x18,0xE0,0x18,0x40,0x18,0x00,0x18,0x00,
0x18,0x00,0xFF,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,

/*g   CHAR_67 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x3E,0x60,0x63,0xE0,0xC1,0x80,
0xC1,0x80,0xC1,0x80,0x63,0x00,0x3E,0x00,
0x60,0x00,0x78,0x00,0xCF,0x80,0xC1,0xC0,
0xC0,0x60,0xC0,0x60,0x60,0xC0,0x3F,0x80,

/*h   CHAR_68 */
0x00,0x00,0x00,0x00,0xE0,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x67,0x00,0x6D,0x80,0x78,0xC0,
0x70,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0xF1,0xE0,0x00,0x00,0x00,0x00,

/*i   CHAR_69 */
0x00,0x00,0x00,0x00,0x06,0x00,0x0F,0x00,
0x0F,0x00,0x06,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x1E,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,

/*j   CHAR_6A */
0x00,0x00,0x00,0x00,0x03,0x00,0x07,0x80,
0x07,0x80,0x03,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x07,0x80,0x01,0x80,0x01,0x80,
0x01,0x80,0x01,0x80,0x01,0x80,0x01,0x80,
0x01,0x80,0x01,0x80,0x01,0x80,0x61,0x80,
0xF1,0x80,0xE1,0x80,0x63,0x00,0x3E,0x00,

/*k   CHAR_6B */
0x00,0x00,0x00,0x00,0xE0,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x63,0xE0,0x61,0x80,0x63,0x00,
0x66,0x00,0x6C,0x00,0x7E,0x00,0x76,0x00,
0x63,0x00,0x63,0x00,0x61,0x80,0x61,0x80,
0x60,0xC0,0xF1,0xE0,0x00,0x00,0x00,0x00,

/*l   CHAR_6C */
0x00,0x00,0x00,0x00,0x1E,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x1F,0x80,0x00,0x00,0x00,0x00,

/*m   CHAR_6D */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xED,0xC0,0x7F,0xE0,0x66,0x60,
0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,
0x66,0x60,0x66,0x60,0x66,0x60,0x66,0x60,
0x66,0x60,0xF7,0x70,0x00,0x00,0x00,0x00,

/*n   CHAR_6E */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xE7,0x00,0x6D,0x80,0x78,0xC0,
0x70,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0xF1,0xE0,0x00,0x00,0x00,0x00,

/*o   CHAR_6F */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x1F,0x00,0x31,0x80,0x60,0xC0,
0x60,0xC0,0xC0,0x60,0xC0,0x60,0xC0,0x60,
0xC0,0x60,0xC0,0x60,0x60,0xC0,0x60,0xC0,
0x31,0x80,0x1F,0x00,0x00,0x00,0x00,0x00,

/*p   CHAR_70 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xEF,0x00,0x79,0x80,0x70,0xC0,
0x60,0xC0,0x60,0x60,0x60,0x60,0x60,0x60,
0x60,0xC0,0x70,0xC0,0x79,0x80,0x6F,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0xF0,0x00,

/*q   CHAR_71 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x1E,0x40,0x33,0xC0,0x61,0xC0,
0x60,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,
0x60,0xC0,0x61,0xC0,0x33,0xC0,0x1E,0xC0,
0x00,0xC0,0x00,0xC0,0x00,0xC0,0x01,0xE0,

/*r   CHAR_72 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xE7,0x80,0x6C,0xC0,0x78,0xE0,
0x70,0xC0,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0x60,0x00,0x60,0x00,0x60,0x00,
0x60,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,

/*s   CHAR_73 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x3F,0x00,0x61,0xC0,0xC0,0xC0,
0xC0,0x40,0xE0,0x00,0x78,0x00,0x1F,0x00,
0x03,0xC0,0x00,0xE0,0x80,0x60,0xC0,0x60,
0xE0,0xC0,0x3F,0x80,0x00,0x00,0x00,0x00,

/*t   CHAR_74 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x08,0x00,0x18,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0xFF,0xC0,0x18,0x00,0x18,0x00,
0x18,0x00,0x18,0x00,0x18,0x00,0x18,0x00,
0x18,0x00,0x18,0x00,0x18,0x60,0x18,0x60,
0x0C,0xC0,0x07,0x80,0x00,0x00,0x00,0x00,

/*u   CHAR_75 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xE1,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x60,0xC0,0x60,0xC0,
0x60,0xC0,0x60,0xC0,0x61,0xC0,0x63,0xC0,
0x36,0xC0,0x1C,0xE0,0x00,0x00,0x00,0x00,

/*v   CHAR_76 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xF1,0xE0,0x60,0xC0,0x60,0xC0,
0x31,0x80,0x31,0x80,0x31,0x80,0x1B,0x00,
0x1B,0x00,0x1B,0x00,0x0E,0x00,0x0E,0x00,
0x0E,0x00,0x04,0x00,0x00,0x00,0x00,0x00,

/*w   CHAR_77 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xF0,0xF0,0x60,0x60,0x60,0x60,
0x66,0x60,0x66,0x60,0x36,0xC0,0x36,0xC0,
0x36,0xC0,0x3F,0xC0,0x19,0x80,0x19,0x80,
0x19,0x80,0x19,0x80,0x00,0x00,0x00,0x00,

/*x   CHAR_78 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xF1,0xE0,0x60,0xC0,0x60,0xC0,
0x31,0x80,0x1B,0x00,0x0E,0x00,0x0E,0x00,
0x0E,0x00,0x1B,0x00,0x31,0x80,0x60,0xC0,
0x60,0xC0,0xF1,0xE0,0x00,0x00,0x00,0x00,

/*y   CHAR_79 */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0xF1,0xE0,0x60,0xC0,0x30,0xC0,
0x31,0x80,0x19,0x80,0x19,0x80,0x0F,0x00,
0x0F,0x00,0x07,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0xCC,0x00,0xCC,0x00,0x78,0x00,

/*z   CHAR_7A */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x7F,0xE0,0x40,0x60,0x40,0xC0,
0x01,0x80,0x03,0x00,0x06,0x00,0x0C,0x00,
0x18,0x00,0x30,0x00,0x60,0x20,0xC0,0x20,
0xC0,0x60,0xFF,0xE0,0x00,0x00,0x00,0x00,

/*{   CHAR_7B */
0x01,0xE0,0x03,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x0C,0x00,0x18,0x00,
0x18,0x00,0x0C,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x03,0x00,0x01,0xE0,

/*|   CHAR_7C */
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,

/*}   CHAR_7D */
0x78,0x00,0x0C,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x03,0x00,0x01,0x80,
0x01,0x80,0x03,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x06,0x00,0x06,0x00,
0x06,0x00,0x06,0x00,0x0C,0x00,0x78,0x00,

/*~   CHAR_7E */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x38,0x00,0x6C,0x30,
0xC3,0x60,0x01,0xC0,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

//	CHAR_7F 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x04,0x00,0x04,0x00,0x0E,0x00,0x0A,0x00,
0x1B,0x00,0x11,0x00,0x31,0x80,0x20,0x80,
0x60,0xC0,0x40,0x40,0xC0,0x60,0xFF,0xE0,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00

};


#endif		//__Asc8X16_dot_H
	

