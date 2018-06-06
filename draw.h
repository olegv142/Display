#pragma once

#include <stdint.h>

class RGB16DisplayAdaptor;

void draw_circle(
		class RGB16DisplayAdaptor* disp,
		uint16_t cx, uint16_t cy, // center
		uint16_t ra,              // radius
		uint16_t b,               // border thickness, 0 means completely filled
		uint16_t color            // color
	);

void draw_rect(
		class RGB16DisplayAdaptor* disp,
		uint16_t x0, uint16_t y0, // top left corner
		uint16_t x1, uint16_t y1, // bottom right corner
		uint16_t b,               // border thickness, 0 means completely filled
		uint16_t color            // color
	);

void draw_rect_rounded(
		class RGB16DisplayAdaptor* disp,
		uint16_t x0, uint16_t y0, // top left corner
		uint16_t x1, uint16_t y1, // bottom right corner
		uint16_t ra,              // the corner radius
		uint16_t color            // fill color
	);
