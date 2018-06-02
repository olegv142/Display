#pragma once

/* 16 bit colour value: 5 red | 6 green | 5 blue */
#define RGB16(r, g, b) (((uint16_t)((r) & 0xF8)<<8) | ((uint16_t)((g) & 0xFC)<<3) | ((uint16_t)((b) & 0xF8)>>3))

/* Some RGB16 color definitions                                                 */
#define RGB16Black           RGB16(0,    0,    0)
#define RGB16Navy            RGB16(0,    0,  128)
#define RGB16DarkGreen       RGB16(0,  128,    0)
#define RGB16DarkCyan        RGB16(0,  128,  128)
#define RGB16Maroon          RGB16(128,   0,   0)
#define RGB16Purple          RGB16(128,   0, 128)
#define RGB16Olive           RGB16(128, 128,   0)
#define RGB16LightGrey       RGB16(192, 192, 192)
#define RGB16DarkGrey        RGB16(128, 128, 128)
#define RGB16Blue            RGB16(0,     0, 255)
#define RGB16Green           RGB16(0,   255,   0)
#define RGB16Cyan            RGB16(0,   255, 255)
#define RGB16Red             RGB16(255,   0,   0)
#define RGB16Magenta         RGB16(255,   0, 255)
#define RGB16Yellow          RGB16(255, 255,   0)
#define RGB16White           RGB16(255, 255, 255)
#define RGB16Orange          RGB16(255, 165,   0)
#define RGB16GreenYellow     RGB16(173, 255,  47)
