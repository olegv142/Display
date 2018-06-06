#pragma once

#include <stdint.h>

class RGB16DisplayAdaptor;

void draw_circle(
		class RGB16DisplayAdaptor* disp,
		uint16_t cx, uint16_t cy, // center
		uint16_t ra,              // radius
		uint16_t b,               // border thickness, 0 means completely filled
		uint16_t color            // border color
	);

