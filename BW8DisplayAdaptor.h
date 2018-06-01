#pragma once

#include <Arduino.h>

/* The page is the group of 8 consecutive rows */
#define DISP_PG_SZ 8

/*
 * B/W display with 8-pixel height 'pages'
 */
class BW8DisplayAdaptor {
public:
	BW8DisplayAdaptor() {}
	virtual ~BW8DisplayAdaptor() {}

	/* Initialize interface port */
	virtual void begin() {}

	/* Returns display width */
	virtual uint8_t width() const = 0;

	/* Returns display height */
	virtual uint8_t height() const = 0;

	/* Initialize display */
	virtual void init() = 0;

	/* Enable / disable display */
	virtual void enable(bool on) = 0;

	/* Switch all pixels to active state */
	virtual void light_all(bool active) = 0;

	/* Set inverse / normal display */
	virtual void set_inverse(bool active) = 0;

	/* Set brightness value */
	virtual void set_brightness(uint8_t val) = 0;

	/* Clear display memory */
	virtual void clear() = 0;

	/* Clear certain region (note that the row coordinate is in page units) */
	virtual void clear_region(uint8_t col, uint8_t pg, uint8_t w, uint8_t h) = 0;

	/* Write data to display memory (note that the row coordinate is in page units) */
	virtual void write(uint8_t col, uint8_t pg, uint8_t const* data, unsigned len) = 0;
};

