#pragma once

#include <Arduino.h>

#define MONO_SPACING (-1)

/*
 * The purpose of this module is to provide support for the GLCD lib fonts since they can be
 * created by freely available tools like MicoElectronica GLCD fonts creator.
 */

/* The font descriptor */
struct glcd_font {
	uint8_t w;        // width in pixels
	uint8_t h;        // height in pixels
	uint8_t code_off; // the first symbol code
	uint8_t code_num; // the number of symbols present
	uint8_t const* PROGMEM data; // the bitmaps of all symbols stored sequentially 
	// as w x h bytes arrays each prefixed by byte storing the actual symbol width
};

/* Check if font has given symbol */
static inline unsigned glcd_font_sym_valid(struct glcd_font const* font, uint8_t c)
{
	return c && (uint8_t)c >= font->code_off && (uint8_t)c < font->code_off + font->code_num;
}

/* Query the height of the font in 8 pixel groups */
static inline unsigned glcd_font_col_bytes(struct glcd_font const* font)
{
	return (font->h + 7) / 8;
}

/* Get the height of the printing area in pixels */
static inline unsigned glcd_font_height(struct glcd_font const* font)
{
	return glcd_font_col_bytes(font) * 8;
}

/* Query the number of bytes occupied by every symbol */
static inline unsigned glcd_font_sym_bytes(struct glcd_font const* font)
{
	return 1 + glcd_font_col_bytes(font) * font->w;
}

/* Query the data array fot the given symbol. Note that the first byte represents the actual symbol with in pixels */
static inline uint8_t const* glcd_font_sym_data(struct glcd_font const* font, char c)
{
	return font->data + (c - font->code_off) * glcd_font_sym_bytes(font);
}

/* Calculate printed text length */
unsigned glcd_printed_len(const char* str, struct glcd_font const* font, int spacing);

/*
 * Monochrome display printing routines
 */

class BW8DisplayAdaptor;

/* Print string starting from the specified position where y is in 8 pixel groups (pages). If spacing < 0 the font
 * will be treated as mono spacing, otherwise the specified spacing will be used for variable spacing print.
 * Returns the width of the text printed.
 */
int glcd_print_str(BW8DisplayAdaptor* d, unsigned x, unsigned y, const char* str, struct glcd_font const* font, int spacing);

/* Print string in given display area. If spacing < 0 the font will be treated as mono spacing, otherwise the specified
 * spacing will be used for variable spacing print. In case the text with is less than print area width w the remaining
 * display area will be erased. Returns the width of the text printed.
 */
int glcd_print_str_w(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing);

/* Print string right aligned. Returns the offset of the printed text end. */
int glcd_print_str_r(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing);

/*
 * Color display printing routines
 */

class RGB16DisplayAdaptor;

/* Print string starting from the specified position. If spacing < 0 the font
 * will be treated as mono spacing, otherwise the specified spacing will be used for variable spacing print.
 * The colors[1] specifies the text color while the colors[0] will be used for background. 
 * Returns the width of the text printed.
 */
int glcd_print_str(RGB16DisplayAdaptor* d, unsigned x, unsigned y, const char* str, struct glcd_font const* font, int spacing, uint16_t colors[2]);

/* Print string in given display area. If spacing < 0 the font will be treated as mono spacing, otherwise the specified
 * spacing will be used for variable spacing print. In case the text with is less than print area width w the remaining
 * display area will be erased. The colors[1] specifies the text color while the colors[0] will be used for background. 
 * Returns the width of the text printed.
 */
int glcd_print_str_w(RGB16DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing, uint16_t colors[2]);

/* Print string right aligned. 
 * The colors[1] specifies the text color while the colors[0] will be used for background. 
 * Returns the offset of the printed text end.
 */
int glcd_print_str_r(RGB16DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing, uint16_t colors[2]);

/* Print string centred. 
 * The colors[1] specifies the text color while the colors[0] will be used for background. 
 * Returns the offset of the printed text end.
 */
int glcd_print_str_c(RGB16DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing, uint16_t colors[2]);
