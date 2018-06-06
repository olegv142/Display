#include "draw.h"
#include "RGB16DisplayAdaptor.h"

void draw_circle_filled(
		class RGB16DisplayAdaptor* disp,
		uint16_t cx, uint16_t cy, // center
		uint16_t ra,              // radius
		uint16_t color            // fill color
	)
{
	uint16_t y = ra;
	int16_t E = -1 - ra / 4;

	disp->vline(cx, cy - y, 2*y + 1, color);
	for (uint16_t x = 1; x <= ra; ++x)
	{
		E += 2*x - 1;
		while (E > 0) {
			E -= 2*y - 1;
			--y;
		}
		disp->vline(cx + x, cy - y, 2*y + 1, color);
		disp->vline(cx - x, cy - y, 2*y + 1, color);
	}
}

void draw_circle_thin(
		class RGB16DisplayAdaptor* disp,
		uint16_t cx, uint16_t cy, // center
		uint16_t ra,              // radius
		uint16_t color            // border color
	)
{
	uint16_t y = ra, last_y = ra;
	int16_t E = -1 - ra / 4;
	uint16_t l, r, u, d, len;

	for (uint16_t x = 1; x <= ra; ++x)
	{
		E += 2*x - 1;
		while (E > 0) {
			E -= 2*y - 1;
			--y;
		}

		len = last_y - y;
		if (!len) len = 1;
		l = cx - x + 1;
		r = cx + x - 1;
		u = cy - last_y;
		d = cy + last_y - len + 1;

		disp->vline(l, u, len, color);
		disp->vline(l, d, len, color);
		disp->vline(r, u, len, color);
		disp->vline(r, d, len, color);

		last_y = y;
	}

	u = cy - y;
	len = 2*y + 1;
	disp->vline(cx + ra, u, len, color);
	disp->vline(cx - ra, u, len, color);
}

void draw_circle_thick(
		class RGB16DisplayAdaptor* disp,
		uint16_t cx, uint16_t cy, // center
		uint16_t ra,              // radius
		uint16_t b,               // border thickness
		uint16_t color            // border color
	)
{
	uint16_t y = ra;
	int16_t yi = ra - b;
	int16_t E = -1 - ra / 4;
	int16_t Ei = -yi / 4;
	uint16_t l, r, u, d, len;

	disp->vline(cx, cy - ra, b, color);
	disp->vline(cx, cy + ra - b + 1, b, color);

	for (uint16_t x = 1; x <= ra; ++x)
	{
		E  += 2*x - 1;
		Ei += 2*x - 1;
		while (E > 0) {
			E -= 2*y - 1;
			--y;
		}
		while (Ei > 0 && yi >= 0) {
			Ei -= 2*yi - 1;
			--yi;
		}
		l = cx - x;
		r = cx + x;
		if (yi >= 0) {
			len = y - yi;
			u = cy - y;
			d = cy + y - len + 1;
			disp->vline(l, u, len, color);
			disp->vline(l, d, len, color);
			disp->vline(r, u, len, color);
			disp->vline(r, d, len, color);
		} else {
			len = 2*y + 1;
			u = cy - y;
			disp->vline(l, u, len, color);
			disp->vline(r, u, len, color);
		}
	}
}

void draw_circle(
		class RGB16DisplayAdaptor* disp,
		uint16_t cx, uint16_t cy, // center
		uint16_t ra,              // radius
		uint16_t b,               // border thickness, 0 means completely filled
		uint16_t color            // border color
	)
{
	if (!b || b > ra) {
		draw_circle_filled(disp, cx, cy, ra, color);
	} else if (b == 1) {
		draw_circle_thin(disp, cx, cy, ra, color);
	} else {
		draw_circle_thick(disp, cx, cy, ra, b, color);
	}
}
