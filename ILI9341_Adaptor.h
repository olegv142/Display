#pragma once

#include "RGB16DisplayAdaptor.h"
#include "SPIDisplay.h"

/*
 * ILI9341 display low level interface
 */
class ILI9341_Adaptor : public RGB16DisplayAdaptor, protected SPIDisplay {
public:
	/*
	 * Create display adaptor instance given the screen orientation and control lines pin numbers.
	 */
	ILI9341_Adaptor(uint8_t orientation, uint8_t cs, uint8_t rst, uint8_t dc)
		: SPIDisplay(cs, rst, dc)
		, m_o(orientation), m_r(0)
			{}

	/* Initialize interface port */
	virtual void begin() { SPIDisplay::begin(); }

	/* Returns display width */
	virtual uint16_t width() const { return (m_o + m_r) & 1 ? 320 : 240; }

	/* Returns display height */
	virtual uint16_t height() const { return (m_o + m_r) & 1 ? 240 : 320; }

	/* Initialize display */
	virtual void init(uint16_t fill_colour = RGB16Black);

	/* Enable / disable display */
	virtual void enable(bool on);

	/*
	 * Set brightness value. May not work on the particular hardware in case
	 * PWM output of the controller is left unconnected.
	 */
	virtual void set_brightness(uint8_t val);

	/*
	 * Core drawing API
	 */

	/* Put pixel */
	virtual void put_pixel(uint16_t x, uint16_t y, uint16_t colour);

	/* Fill certain region */
	virtual void fill_rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour);

	/*
	 * Display memory writing API
	 */

	/* Setup rectangular writing area */
	virtual void write_begin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool col_order);

	/* Write pixels */
	virtual void write_pixels(uint16_t const* pix_buff, int len, int pgm = false);

	/* Write pixels bitmap */
	virtual void write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2], int pgm = false);

	/* End writing */
	virtual void write_end();

	/*
	 * Scrolling API
	 */

	/*
	 * Set scrolling range.
	 * The scrolling can only take place along the long axis (320 pixs).
	 * One can reserve fixed areas at both sides of the scrolling area (in the scroll direction)
	 * by specifying scrolling range start and end offsets.
	 */
	void set_scroll_range(uint16_t begin, uint16_t end);

	/*
	 * Set scrolling position. This is the index of the line (starting from the screen top) which will be displayed at the top
	 * of the scrolling area. Note that pos should be in the range begin <= pos < end where begin and end are scrolling area
	 * boundaries. Note that the direction of the pos axis is independent on the screen orientation parameter
	 * (passed to the constructor).
	 */
	void set_scroll_pos(uint16_t pos);

	/*
	 * Axis rotation may be used to print text in non-horizontal direction. The rotation parameter is in the same
	 * units as orientation parameter of the constructor. Those 2 number are just added.
	 */
	void rotate_axis(uint8_t r) { m_r = r; set_write_order(); }

private:
	/* Perform hard / soft reset and wake up screen from the sleep */
	void reset();
	/* Push power-up configuration */
	void configure();

	/*
	 * Helper routines. The ones with name ending by _ should be enclosed between select and unselect calls.
	 */

	/* Setup memory write window */
	void set_write_window_(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	/* Setup memory write order according to the screen orientation and optionally flip axis
	 * so writing will proceed in the column order.
	 */
	void set_write_order_(bool flip_axis = false);
	void set_write_order(bool flip_axis = false) {
		select();
		set_write_order_(flip_axis);
		unselect();
	}
	/* Write pixel RGB data */
	void write_pixel_(uint16_t colour) {
		transfer((uint8_t)(colour >> 8));
		transfer((uint8_t)(colour));
	}

	uint8_t  m_o;
	uint8_t  m_r;
};

