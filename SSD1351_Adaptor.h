#pragma once

#include "RGB16DisplayAdaptor.h"
#include "SPIDisplay.h"

/*
 * SSD1351 128x128 color OLED display low level interface
 */
class SSD1351_Adaptor : public RGB16DisplayAdaptor, protected SPIDisplay {
public:
	/*
	 * Create display adaptor instance given the screen orientation and control lines pin numbers.
	 */
	SSD1351_Adaptor(uint8_t cs, uint8_t rst, uint8_t dc)
		: SPIDisplay(cs, rst, dc)
			{}

	/* Initialize interface port */
	virtual void begin() { SPIDisplay::begin(); }

	/* Returns display width */
	virtual uint16_t width() const { return 128; }

	/* Returns display height */
	virtual uint16_t height() const { return 128; }

	/* Initialize display */
	virtual void init(uint16_t fill_colour = RGB16Black);

	/* Enable / disable display */
	virtual void enable(bool on);

	/*
	 * Set brightness value. May not work on the particular hardware in case
	 * PWM output of the controller is left unconnected.
	 */
	virtual void set_brightness(uint8_t val);

	/* Put pixel */
	virtual void put_pixel(uint16_t x, uint16_t y, uint16_t colour);

	/* Draw horizontal line */
	virtual void hline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour);

	/* Draw vertical line */
	virtual void vline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour);

	/* Setup rectangular writing area */
	virtual void write_begin(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool col_order);

	/* Write pixels */
	virtual void write_pixels(uint16_t const* pix_buff, int len);

	/* Write pixels bitmap */
	virtual void write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2]);

	/* End writing */
	virtual void write_end();

	/* Fill certain region */
	virtual void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour);

	/*
	 * Other non standard functions
	 */

	/* Switch all pixels to active state */
	void light_all(bool active);

private:
	/* Push power-up configuration */
	void configure();

	/*
	 * Helper routines. The ones with name ending by _ should be enclosed between select and unselect calls.
	 */

	/* Setup memory write window */
	void set_write_window_(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
	/* Setup memory write position */
	void set_write_pos_(uint8_t x, uint8_t y);
	/* Setup memory write order */
	void set_write_order_(bool vertical = false);
	/* Write pixel RGB data */
	void write_pixel_(uint16_t colour) {
		transfer((uint8_t)(colour >> 8));
		transfer((uint8_t)(colour));
	}
};
