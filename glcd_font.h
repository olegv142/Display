#pragma once

#include <Arduino.h>

#define MONO_SPACING (-1)

/*
 * The purpose of this module is to provide support for the GLCD lib fonts since they can be
 * created by freely available tools like MicoElectronica GLCD fonts creator.
 */

struct glcd_font {
	uint8_t w;
	uint8_t h;
	uint8_t code_off;
	uint8_t code_num;
	uint8_t const* PROGMEM data;
};

class BW8DisplayAdaptor;

/* Calculate printed text length */
unsigned glcd_printed_len(const char* str, struct glcd_font const* font, int spacing);

/* Print string starting from the specified position where y is in 8 pixel groups (pages). If spacing < 0 the font
 * will be treated as mono spacing, otherwise the specified spacing will be used for variable spacing print.
 * Returns the width of the text printed or -1 in case of error.
 */
int glcd_print_str(BW8DisplayAdaptor* d, unsigned x, unsigned y, const char* str, struct glcd_font const* font, int spacing);

/* Print string in given display area. If spacing < 0 the font will be treated as mono spacing, otherwise the specified
 * spacing will be used for variable spacing print. In case the text with is less than print area width w the remaining
 * display area will be erased. Returns the width of the text printed or -1 in case of error.
 */
int glcd_print_str_w(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing);

/* Print string right aligned. Returns the offset of the printed text end or -1 in case of error. */
int glcd_print_str_r(BW8DisplayAdaptor* d, unsigned x, unsigned y, unsigned w, const char* str, struct glcd_font const* font, int spacing);
