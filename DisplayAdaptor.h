#pragma once

#include <Arduino.h>

/* The page is the group of 8 consecutive rows */
#define DISP_PG_SZ 8

/* 
 * Generic low level display API
 */
class DisplayAdaptor {
public:
	DisplayAdaptor() {}
	virtual ~DisplayAdaptor() {}

	/* Initialize interface port */
	virtual void begin() {}

	/* Check display is present */
	virtual bool probe() = 0;

	/* Initialize display */
	virtual bool init() = 0;

	/* Enable / disable display */
	virtual bool enable(bool on) = 0;

	/* Switch all pixels to active state */
	virtual bool light_all(bool active) = 0;

	/* Set inverse / normal display */
	virtual bool set_inverse(bool active) = 0;

	/* Set brightness value */
	virtual bool set_brightness(uint8_t val) = 0;

	/* Clear display memory */
	virtual bool clear() = 0;

	/* Clear certain region (note that the row coordinate is in page units) */
	virtual bool clear_region(uint8_t col, uint8_t pg, uint8_t w) = 0;

	/* Write data to display memory (note that the row coordinate is in page units) */
	virtual bool write(uint8_t col, uint8_t pg, uint8_t const* data, unsigned len) = 0;
};

