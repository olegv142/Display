#pragma once

#include "RGB16DisplayAdaptor.h"

class RGB16BuffAdaptor : public RGB16DisplayAdaptor {
public:
	/*
	 * The display adapter implementation writing pixels to in memory bitmap.
	 * Use it whenever you need to prepare some portion of the displayed image
	 * before writing it to the physical display.
	 */
	RGB16BuffAdaptor(
			// We assume buffer has column order of pixels
			uint16_t* buff, uint16_t w, uint16_t h,
			// The following parameter may be used to specify transparent color.
			// Writes of such color does not modify buffer content.
			int32_t transparent = -1
		)
		: m_buff(buff)
		, m_w(w), m_h(h)
		, m_transparent(transparent) {}

	virtual uint16_t width()  const { return m_w; }
	virtual uint16_t height() const { return m_h; }

	virtual void init(uint16_t fill_colour = RGB16Black) {
		uint32_t const pixels = m_w * m_h;
		for (uint32_t i = 0; i < pixels; ++i)
			m_buff[i] = fill_colour;
	}

	virtual void enable(bool on) {};
	virtual void set_brightness(uint8_t val) {}

	void put_pixel_(uint16_t x, uint16_t y, uint16_t colour) {
		if (colour != m_transparent && x < m_w && y < m_h)
			m_buff[(uint32_t)m_h * x + y] = colour; // Assume column order
	}

	virtual void put_pixel(uint16_t x, uint16_t y, uint16_t colour) {
		put_pixel_(x, y, colour);
	}

	virtual void fill_rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour) {
		for (uint16_t x = x0; x <= x1; ++x)
			for (uint16_t y = y0; y <= y1; ++y)
				put_pixel_(x, y, colour);
	}

	/* Setup rectangular writing area */
	virtual void write_begin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool col_order) {
		m_wr_x0 = x0; m_wr_y0 = y0;
		m_wr_x1 = x1; m_wr_y1 = y1;
		m_col_order = col_order;
	}

	/* Write pixels */
	virtual void write_pixels(uint16_t const* pix_buff, int len, int pgm = false) {
		if (m_col_order)
			for (uint16_t x = m_wr_x0; x <= m_wr_x1; ++x)
				for (uint16_t y = m_wr_y0; y <= m_wr_y1 && len > 0; ++y, --len)
					put_pixel_(x, y, *pix_buff++);
		else
			for (uint16_t y = m_wr_y0; y <= m_wr_y1; ++y)
				for (uint16_t x = m_wr_x0; x <= m_wr_x1 && len > 0; ++x, --len)
					put_pixel_(x, y, *pix_buff++);
	}

	/* Write pixels bitmap */
	virtual void write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2], int pgm = false) {
		int i = 0;
		if (m_col_order)
			for (uint16_t x = m_wr_x0; x <= m_wr_x1; ++x)
				for (uint16_t y = m_wr_y0; y <= m_wr_y1 && i < len; ++y, ++i) {
					uint16_t colour = colours[1 & (pix_bm[i/8] >> (i%8))];
					put_pixel_(x, y, colour);
				}
		else
			for (uint16_t y = m_wr_y0; y <= m_wr_y1; ++y)
				for (uint16_t x = m_wr_x0; x <= m_wr_x1 && i < len; ++x, ++i) {
					uint16_t colour = colours[1 & (pix_bm[i/8] >> (i%8))];
					put_pixel_(x, y, colour);
				}
	}

	virtual void write_end() {}

private:
	uint16_t* m_buff;
	uint16_t  m_w, m_h;
	int32_t   m_transparent;
	uint16_t  m_wr_x0, m_wr_y0, m_wr_x1, m_wr_y1;
	bool      m_col_order;
};
