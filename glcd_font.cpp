/*
 * The purpose of this module is to provide support for the GLCD lib fonts since they can be
 * created by freely available tools like MicoElectronica GLCD fonts creator.
 */

#include "glcd_fonts.h"
#include "BW8DisplayAdaptor.h"
#include "RGB16DisplayAdaptor.h"

#define MAX_DATA_CHUNK 16

static uint8_t s_char_buff[MAX_DATA_CHUNK];

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

/*
 * Monochrome display printing routines
 */

/* Put char in the specified position. Note that y is in 8 pixel groups */
static void glcd_draw_char_ex(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, unsigned h, uint8_t const* data, struct glcd_patch const* patch)
{
	unsigned r, c;
	for (r = 0; r < h; ++r, ++data)
	{
		uint8_t const* ptr = data;
		uint8_t off = 0, i = 0, b;
		for (c = 0; c < w; ++c, ++i, ptr += h) {
			if (i >= MAX_DATA_CHUNK) {
				d->write(x + off, y + r, s_char_buff, i);
				off += i;
				i = 0;
			}
			// Retrieve band byte
			b = pgm_read_byte(ptr);
			if (patch) {
				// Apply patch if necessary
				switch (patch->type) {
				case patch_invert:
					b = ~b;
					break;
				case patch_strike:
					if (patch->where == r)
						b ^= patch->param;
					break;
				default:
					;
				}
			}
			s_char_buff[i] = b;
		}
		d->write(x + off, y + r, s_char_buff, i);
	}
}

/* Print string starting from the specified position where y is in 8 pixel groups (pages). If spacing < 0 the font
 * will be treated as mono spacing, otherwise the specified spacing will be used for variable spacing print.
 * Returns the width of the text printed.
 */
int glcd_print_str_ex(BW8DisplayAdaptor* d, unsigned x, unsigned y, const char* str, struct glcd_font const* font, int spacing, struct glcd_patch const* patches)
{
	unsigned h = glcd_font_col_bytes(font);
	unsigned empty_space = spacing > 0 ? spacing : 0;
	unsigned col = x;
	for (;; ++str) {
		char c = *str;
		if (glcd_font_sym_valid(font, c)) {
			uint8_t const* data = glcd_font_sym_data(font, c);
			uint8_t w = spacing < 0 ? font->w : pgm_read_byte(data);
			glcd_draw_char_ex(d, col, y, w, h, data + 1, patches);
			col += w;
			if (empty_space) {
				d->clear_region(col, y, empty_space, h);
				col += empty_space;
			}
			if (patches)
				++patches;
		} else
			break;
	}
	return col - x;
}

/* Print string in given display area. If spacing < 0 the font will be treated as mono spacing, otherwise the specified
 * spacing will be used for variable spacing print. In case the text with is less than print area width w the remaining
 * display area will be erased. Returns the width of the text printed.
 */
int glcd_print_str_w_ex(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing, struct glcd_patch const* patches)
{
	int printed_w = glcd_print_str_ex(d, x, y, str, font, spacing, patches);
	if ((unsigned)printed_w < w) {
		d->clear_region(x + printed_w, y, w - printed_w, glcd_font_col_bytes(font));
	}
	return printed_w;
}

/* Print string right aligned. Returns the offset of the printed text end. */
int glcd_print_str_r_ex(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing, struct glcd_patch const* patches)
{
	unsigned text_w = glcd_printed_len(str, font, spacing);
	if (text_w > w) {
		return glcd_print_str_ex(d, x, y, str, font, spacing, patches);
	} else {
		unsigned off = w - text_w;
		d->clear_region(x, y, off, glcd_font_col_bytes(font));
		return off + glcd_print_str_ex(d, x + off, y, str, font, spacing, patches);
	}
}

int glcd_print_str(BW8DisplayAdaptor* d, unsigned x, unsigned y, const char* str, struct glcd_font const* font, int spacing)
{
	return glcd_print_str_ex(d, x, y, str, font, spacing, NULL);
}

int glcd_print_str_w(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing)
{
	return glcd_print_str_w_ex(d, x, y, w, str, font, spacing, NULL);
}

int glcd_print_str_r(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing)
{
	return glcd_print_str_r_ex(d, x, y, w, str, font, spacing, NULL);
}

/*
 * Color display printing routines
 */

/* Put char in the specified position */
static void glcd_draw_char(RGB16DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, unsigned h, uint8_t const* data, uint16_t colors[2])
{
	d->write_begin(x, y, x + w - 1, y + h * 8 - 1, true);
	d->write_pixels_bm(data, w * h * 8, colors, true);
	d->write_end();
}

/* Print string starting from the specified position. If spacing < 0 the font
 * will be treated as mono spacing, otherwise the specified spacing will be used for variable spacing print.
 * The colors[1] specifies the text color while the colors[0] will be used for background. 
 * Returns the width of the text printed.
 */
int glcd_print_str(RGB16DisplayAdaptor* d, unsigned x, unsigned y, const char* str, struct glcd_font const* font, int spacing, uint16_t colors[2])
{
	unsigned h = glcd_font_col_bytes(font);
	unsigned empty_space = spacing > 0 ? spacing : 0;
	unsigned col = x;
	for (;; ++str) {
		char c = *str;
		if (glcd_font_sym_valid(font, c)) {
			uint8_t const* data = glcd_font_sym_data(font, c);
			uint8_t w = spacing < 0 ? font->w : pgm_read_byte(data);
			glcd_draw_char(d, col, y, w, h, data + 1, colors);
			col += w;
			if (empty_space) {
				d->fill_rect(col, y, col + empty_space - 1, y + h * 8 - 1, colors[0]);
				col += empty_space;
			}
		} else
			break;
	}
	return col - x;
}

/* Print string in given display area. If spacing < 0 the font will be treated as mono spacing, otherwise the specified
 * spacing will be used for variable spacing print. In case the text with is less than print area width w the remaining
 * display area will be erased. The colors[1] specifies the text color while the colors[0] will be used for background. 
 * Returns the width of the text printed.
 */
int glcd_print_str_w(RGB16DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing, uint16_t colors[2])
{
	int printed_w = glcd_print_str(d, x, y, str, font, spacing, colors);
	if ((unsigned)printed_w < w) {
		unsigned h = glcd_font_col_bytes(font);
		d->fill_rect(x + printed_w, y, x + w - 1, y + h * 8 - 1, colors[0]);
	}
	return printed_w;
}

/* Print string right aligned. 
 * The colors[1] specifies the text color while the colors[0] will be used for background. 
 * Returns the offset of the printed text end.
 */
int glcd_print_str_r(RGB16DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing, uint16_t colors[2])
{
	unsigned text_w = glcd_printed_len(str, font, spacing);
	if (text_w > w) {
		return glcd_print_str(d, x, y, str, font, spacing, colors);
	} else {
		unsigned off = w - text_w;
		unsigned h = glcd_font_col_bytes(font);
		d->fill_rect(x, y, x + off - 1, y + h * 8 - 1, colors[0]);
		return off + glcd_print_str(d, x + off, y, str, font, spacing, colors);
	}
}

/* Print string centred. 
 * The colors[1] specifies the text color while the colors[0] will be used for background. 
 * Returns the offset of the printed text end.
 */
int glcd_print_str_c(RGB16DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing, uint16_t colors[2])
{
	unsigned text_w = glcd_printed_len(str, font, spacing);
	if (text_w > w) {
		return glcd_print_str(d, x, y, str, font, spacing, colors);
	} else {
		unsigned off = (w - text_w) / 2;
		unsigned h = glcd_font_col_bytes(font);
		d->fill_rect(x, y, x + off - 1, y + h * 8 - 1, colors[0]);
		unsigned end = off + glcd_print_str(d, x + off, y, str, font, spacing, colors);
		d->fill_rect(x + end, y, x + w - 1, y + h * 8 - 1, colors[0]);
		return end;
	}
}

