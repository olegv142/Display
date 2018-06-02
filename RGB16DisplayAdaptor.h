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

	/* Put pixel */
	virtual void put_pixel(uint16_t x, uint16_t y, uint16_t colour) = 0;

	/* Draw horizontal line */
	virtual void hline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour) = 0;

	/* Draw vertical line */
	virtual void vline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour) = 0;
	
	/* Setup rectangular writing area */
	virtual void write_begin(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool col_order) = 0;

	/* Write pixels */
	virtual void write_pixels(uint16_t const* pix_buff, int len) = 0;

	/* Write pixels bitmap */
	virtual void write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2]) = 0;

	/* End writing */
	virtual void write_end() = 0;

	/* Fill certain region */
	virtual void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour) = 0;

	/* Fill display memory with given colour */
	virtual void fill(uint16_t colour) {
		fill_rect(0, 0, width(), height(), colour);
	}
};

