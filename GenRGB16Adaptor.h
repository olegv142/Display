#pragma once

#include "RGB16DisplayAdaptor.h"
#include "SPIDisplay.h"

/*
 * ST7796 display low level interface
 */
class GenRGB16Adaptor : public RGB16DisplayAdaptor, protected SPIDisplay {
public:
	/*
	 * Create display adaptor instance given the dimensions, screen orientation and control lines pin numbers.
	 * The width (w) argument should correspond to smaller axis, while the height (h) should correspond to the longer one.
	 */
	GenRGB16Adaptor(uint16_t w, uint16_t h, uint8_t orientation, Pin cs, Pin rst, Pin dc)
		: SPIDisplay(cs, rst, dc)
		, m_w(w), m_h(h), m_o(orientation), m_r(0)
		, m_pleft(0), m_pright(0), m_ptop(0), m_pbottom(0)
		, m_bgr(false), m_18bit(false)
		, m_xflip(false), m_yflip(false)
		, m_invert(false), m_gamma(-1)
		, m_madval(-1)
			{}
	/*
	 * The following configuration functions should be called before init()
	 * otherwise they may not take effect.
	 */

	/* Set display area padding */
	void set_padding(uint16_t x_padding, uint16_t y_padding) {
		m_pleft = m_pright = x_padding;
        m_ptop = m_pbottom = y_padding;
	}
	void set_padding(uint16_t left, uint16_t right, uint16_t top, uint16_t bottom) {
		m_pleft = left; m_pright = right;
        m_ptop = top; m_pbottom = bottom;
	}
	/* Configure rgb/bgr physical display mode (default is rgb / false) */
	void set_bgr(bool flag) {
		m_bgr = flag;
	}
	/* Configure 18 bit colors (default is 16 bit / false) */
	void set_18bit_color(bool flag) {
		m_18bit = flag;
	}
	/* Configure physical display flipping */
	void set_flip(bool xflip, bool yflip) {
		m_xflip = xflip; m_yflip = yflip;
	}
	/* Setup colors inversion */
	void set_inversion(bool flag) {
		m_invert = flag;
	}
	/* Set gamma. Valid values are 0..3. The effect may depend on inversion setting. Default is 0. */
	void set_gamma(uint8_t gamma) {
		m_gamma = gamma;
	}

	/* Initialize interface port */
	virtual void begin() { SPIDisplay::begin(); }

	/* Returns display width */
	virtual uint16_t width() const { return (m_o + m_r) & 1 ? m_h : m_w; }

	/* Returns display height */
	virtual uint16_t height() const { return (m_o + m_r) & 1 ? m_w : m_h; }

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
	 * The scrolling can only take place along the long axis.
	 * One can reserve fixed areas at both sides of the scrolling area (in the scroll direction)
	 * by specifying scrolling range start and end offsets. Note that the end is not included in the scrolling area.
	 * Called with default arguments set_scroll_range() will select entire screen to scroll.
	 */
	void set_scroll_range(uint16_t begin = 0, uint16_t end = 0);

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
	void rotate_axis(uint8_t r) { m_r = r; }

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
		if (m_18bit) {
			transfer((uint8_t)((colour >> 8) & 0xF8));
			transfer((uint8_t)((colour >> 3) & 0xFC));
			transfer((uint8_t)((colour << 3) & 0xF8));
		} else {
			transfer((uint8_t)(colour >> 8));
			transfer((uint8_t)(colour));
		}
	}

	uint16_t m_w;
	uint16_t m_h;
	uint8_t  m_o;
	uint8_t  m_r;
	uint16_t m_pleft;
	uint16_t m_pright;
	uint16_t m_ptop;
	uint16_t m_pbottom;
	bool     m_bgr;
	bool     m_18bit;
	bool     m_xflip;
	bool     m_yflip;
	bool     m_invert;
	int8_t   m_gamma;
	int16_t  m_madval;
};

