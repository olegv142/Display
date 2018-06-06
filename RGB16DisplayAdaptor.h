#pragma once

#include <Arduino.h>
#include "RGB16.h"

/*
 * RGB display generic interface
 */
class RGB16DisplayAdaptor {
public:
	RGB16DisplayAdaptor() {}
	virtual ~RGB16DisplayAdaptor() {}

	/* Initialize interface port */
	virtual void begin() {}

	/* Returns display width */
	virtual uint16_t width() const = 0;

	/* Returns display height */
	virtual uint16_t height() const = 0;

	/* Initialize display */
	virtual void init(uint16_t fill_colour = RGB16Black) = 0;

	/* Enable / disable display */
	virtual void enable(bool on) = 0;

	/* Set brightness value */
	virtual void set_brightness(uint8_t val) = 0;

	/*
	 * Core drawing API
	 */

	/* Put pixel */
	virtual void put_pixel(uint16_t x, uint16_t y, uint16_t colour) = 0;

	/* Fill certain region */
	virtual void fill_rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour) = 0;

	/*
	 * Display memory writing API
	 */

	/* Setup rectangular writing area */
	virtual void write_begin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool col_order) = 0;

	/* Write pixels */
	virtual void write_pixels(uint16_t const* pix_buff, int len, int pgm = false) = 0;

	/* Write pixels bitmap */
	virtual void write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2], int pgm = false) = 0;

	/* End writing */
	virtual void write_end() = 0;

	/*
	 * Drawing functions implementation
	 */

	/* Fill display memory with given colour */
	void fill(uint16_t colour) {
		fill_rect(0, 0, width() - 1, height() - 1, colour);
	}

	/* Draw horizontal line */
	void hline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour) {
		fill_rect(x, y, x + len - 1, y, colour);
	}

	/* Draw vertical line */
	void vline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour) {
		fill_rect(x, y, x, y + len - 1, colour);
	}
};

