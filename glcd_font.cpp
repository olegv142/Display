/*
 * The purpose of this module is to provide support for the GLCD lib fonts since they can be
 * created by freely available tools like MicoElectronica GLCD fonts creator.
 */

#include "glcd_fonts.h"
#include "DisplayAdaptor.h"

#define MAX_DATA_CHUNK 16

static uint8_t s_char_buff[MAX_DATA_CHUNK];

static inline unsigned glcd_font_sym_valid(struct glcd_font const* font, uint8_t c)
{
	return c && (uint8_t)c >= font->code_off && (uint8_t)c < font->code_off + font->code_num;
}

static inline unsigned glcd_font_col_bytes(struct glcd_font const* font)
{
	return (font->h + 7) / 8;
}

static inline unsigned glcd_font_sym_bytes(struct glcd_font const* font)
{
	return 1 + glcd_font_col_bytes(font) * font->w;
}

static inline uint8_t const* glcd_font_sym_data(struct glcd_font const* font, char c)
{
	return font->data + (c - font->code_off) * glcd_font_sym_bytes(font);
}

/* Put char in the specified position. Note that y is in 8 pixel groups */
static bool glcd_draw_char(DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, unsigned h, uint8_t const* data)
{
	unsigned r, c;
	for (r = 0; r < h; ++r, ++data)
	{
		uint8_t const* ptr = data;
		uint8_t off = 0, i = 0;
		for (c = 0; c < w; ++c, ++i, ptr += h) {
			if (i >= MAX_DATA_CHUNK) {
				if (!d->write(x + off, y + r, s_char_buff, i)) {
					return false;
				}
				off += i;
				i = 0;
			}
			s_char_buff[i] = pgm_read_byte(ptr);
		}
		if (!d->write(x + off, y + r, s_char_buff, i)) {
			return false;
		}
	}
	return true;
}

/* Print string starting from the specified position where y is in 8 pixel groups (pages). If spacing < 0 the font
 * will be treated as mono spacing, otherwise the specified spacing will be used for variable spacing print.
 * Returns the width of the text printed or -1 in case of error.
 */
int glcd_print_str(DisplayAdaptor* d, unsigned x, unsigned y, const char* str, struct glcd_font const* font, int spacing)
{
	unsigned h = glcd_font_col_bytes(font);
	unsigned empty_space = spacing > 0 ? spacing : 0;
	unsigned col = x;
	for (;; ++str) {
		char c = *str;
		if (glcd_font_sym_valid(font, c)) {
			uint8_t const* data = glcd_font_sym_data(font, c);
			uint8_t w = spacing < 0 ? font->w : pgm_read_byte(data);
			if (!glcd_draw_char(d, col, y, w, h, data + 1)) {
				return -1;
			}			
			col += w;
			if (empty_space) {
				if (!d->clear_region(col, y, empty_space, h))
					return -1;
				col += empty_space;
			}
		} else
			break;
	}
	return col - x;
}

/* Calculate printed text length */
unsigned glcd_printed_len(const char* str, struct glcd_font const* font, int spacing)
{
	unsigned len = 0, empty_space = spacing > 0 ? spacing : 0;
	for (;; ++str) {
		char c = *str;
		if (glcd_font_sym_valid(font, c)) {
			uint8_t const* data = glcd_font_sym_data(font, c);
			uint8_t w = spacing < 0 ? font->w : pgm_read_byte(data);
			len += w + empty_space;
		} else
			break;
	}
	return len;
}

/* Print string in given display area. If spacing < 0 the font will be treated as mono spacing, otherwise the specified
 * spacing will be used for variable spacing print. In case the text with is less than print area width w the remaining
 * display area will be erased. Returns the width of the text printed or -1 in case of error.
 */
int glcd_print_str_w(DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing)
{
	int printed_w = glcd_print_str(d, x, y, str, font, spacing);
	if (printed_w < 0)
		return -1;
	if (printed_w < w) {
		if (!d->clear_region(x + printed_w, y, w - printed_w, glcd_font_col_bytes(font)))
			return -1;
	}
	return printed_w;
}

/* Print string right aligned. Returns the offset of the printed text end or -1 in case of error. */
int glcd_print_str_r(DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing)
{
	unsigned text_w = glcd_printed_len(str, font, spacing);
	if (text_w > w) {
		return glcd_print_str(d, x, y, str, font, spacing);
	} else {
		unsigned off = w - text_w;
		if (!d->clear_region(x, y, off, glcd_font_col_bytes(font)))
			return -1;
		int printed_w = glcd_print_str(d, x + off, y, str, font, spacing);
		if (printed_w < 0)
			return -1;
		return off + printed_w;
	}
}
