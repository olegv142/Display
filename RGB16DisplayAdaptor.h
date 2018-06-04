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
	virtual void init() = 0;

	/* Enable / disable display */
	virtual void enable(bool on) = 0;

	/*
	 * Core drawing API
	 */

	/* Begin / end drawing operations */
	virtual void draw_begin() = 0;
	virtual void draw_end() = 0;

	/*
	 * The following drawing operations with names ended with _ should be enclosed
	 * between draw_begin(), draw_end() calls
	 */

	/* Put pixel */
	virtual void put_pixel_(uint16_t x, uint16_t y, uint16_t colour) = 0;

	/* Draw horizontal line */
	virtual void hline_(uint16_t x, uint16_t y, uint16_t len, uint16_t colour) = 0;

	/* Draw vertical line */
	virtual void vline_(uint16_t x, uint16_t y, uint16_t len, uint16_t colour) = 0;

	/* Fill certain region */
	virtual void fill_rect_(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour) = 0;

	/*
	 * Display RAM writing API
	 */

	/* Setup rectangular writing area */
	virtual void write_begin_(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool col_order) = 0;

	/* Write pixels */
	virtual void write_pixels(uint16_t const* pix_buff, int len) = 0;

	/* Write pixels bitmap */
	virtual void write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2]) = 0;

	/* End writing */
	virtual void write_end_() = 0;

	/*
	 * Drawing routines implementation
	 */

	/* Setup rectangular writing area */
	virtual void write_begin(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool col_order) {
		draw_begin();
		write_begin_(x, y, w, h, col_order);
	}

	/* End writing */
	virtual void write_end() {
		write_end_();
		draw_end();
	}

	/* Put pixel */
	void put_pixel(uint16_t x, uint16_t y, uint16_t colour) {
		draw_begin();
		put_pixel_(x, y, colour);
		draw_end();
	}

	/* Draw horizontal line */
	void hline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour) {
		draw_begin();
		hline_(x, y, len, colour);
		draw_end();
	}

	/* Draw vertical line */
	void vline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour) {
		draw_begin();
		vline_(x, y, len, colour);
		draw_end();
	}

	/* Fill certain region */
	void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour) {
		draw_begin();
		fill_rect_(x, y, w, h, colour);
		draw_end();
	}

	/* Fill display memory with given colour */
	void fill(uint16_t colour) {
		fill_rect(0, 0, width(), height(), colour);
	}
};

