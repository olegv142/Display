/*
 * The purpose of this module is to provide support for the GLCD lib fonts since they can be
 * created by freely available tools like MicoElectronica GLCD fonts creator.
 */

#include "glcd_fonts.h"
#include "DisplayAdaptor.h"

#define MAX_CHAR_W 64

static uint8_t s_char_buff[MAX_CHAR_W];

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
	if (w > MAX_CHAR_W) {
		return false;
	}
	for (r = 0; r < h; ++r, ++data)
	{
		uint8_t const* ptr = data;
		for (c = 0; c < w; ++c, ptr += h) {
			s_char_buff[c] = pgm_read_byte(ptr);
		}
		if (!d->write(x, y + r, s_char_buff, w)) {
			return false;
		}
	}
	return true;
}

/* Print string starting from the specified position. If spacing < 0 the font will be treated as mono spacing,
 * otherwise the specified spacing will be used for variable spacing print.
 */
bool glcd_print_str(DisplayAdaptor* d, unsigned x, unsigned y, const char* str, struct glcd_font const* font, int spacing)
{
	unsigned h = glcd_font_col_bytes(font);
	unsigned empty_space = spacing > 0 ? spacing : 0;
	for (;; ++str) {
		char c = *str;
		if (glcd_font_sym_valid(font, c)) {
			uint8_t const* data = glcd_font_sym_data(font, c);
			uint8_t w = spacing < 0 ? font->w : pgm_read_byte(data);
			if (!glcd_draw_char(d, x, y, w, h, data + 1)) {
				return false;
			}
			x += w + empty_space;
		} else
			break;
	}
	return true;
}

/* Calculate printed text length */
unsigned glcd_printed_len(const char* str, struct glcd_font const* font, int spacing)
{
	unsigned len = 0, empty_space = spacing > 0 ? spacing : 0;
	for (;; ++str) {
		char c = *str;
		if (glcd_font_sym_valid(font, c)) {
			uint8_t const* data = glcd_font_sym_data(font, c);
			uint8_t w = spacing < 0 ? font->w : *data;
			len += w + empty_space;
		} else
			break;
	}
	return len;
}

