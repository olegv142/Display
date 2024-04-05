#include "GenRGB16Adaptor.h"

/* The power-up initialization configuration. Taken from MI0283QT-9A screen datasheet. */
static const uint8_t s_ST7796_init_cmds[] PROGMEM = {
	2, 0x3A, 0x55,          // COLMOD_PIXEL_FORMAT_SET - 16 bit pixel 
	0
};

/* Initialize display */
void GenRGB16Adaptor::init(uint16_t fill_colour)
{
	reset();
	configure();
	fill(fill_colour);
	// turn on RAM displaying
	write_cmd(0x29);
}

/* Perform hard / soft reset and wake up screen from the sleep */
void GenRGB16Adaptor::reset()
{
	// hard reset
	SPIDisplay::reset();
	m_madval = -1;
	// software reset
	write_cmd(0x1);
	delay(5);
	// wake up
	enable(true);
}

/* Push power-up configuration */
void GenRGB16Adaptor::configure()
{
	select();
	write_cmds_(s_ST7796_init_cmds, true);
	if (m_invert)
		write_cmd_(0x21);
	if (0 <= m_gamma && m_gamma < 4) {
		uint8_t gamma_cmd[] = {0x26, (uint8_t)(1 << m_gamma)};
		write_bytes_(mode_cmd_head, gamma_cmd, sizeof(gamma_cmd));
	}
	unselect();
}

/* Enable / disable display */
void GenRGB16Adaptor::enable(bool on)
{
	write_cmd(on ? 0x11 : 0x10);
	if (on) delay(20);
}

/* Set brightness value */
void GenRGB16Adaptor::set_brightness(uint8_t val)
{
	uint8_t bytes[] = {0x51, val};
	write_bytes(mode_cmd_head, bytes, sizeof(bytes));
}

/* Setup memory write window */
void GenRGB16Adaptor::set_write_window_(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
	uint8_t bytes[] = {
		5, 0x2a, (uint8_t)(x0 >> 8), (uint8_t)(x0), (uint8_t)(x1 >> 8), (uint8_t)(x1),
		5, 0x2b, (uint8_t)(y0 >> 8), (uint8_t)(y0), (uint8_t)(y1 >> 8), (uint8_t)(y1),
		1, 0x2c,
		0
	};
	write_cmds_(bytes);
}

/* Setup memory write order according to the screen orientation and optionally flip axis
 * so writing will proceed in the column order.
 */
void GenRGB16Adaptor::set_write_order_(bool flip_axis)
{
	uint8_t o = m_o + m_r;
	uint8_t const mx = 1 << 6;
	uint8_t const my = 1 << 7;
	uint8_t const mv = 1 << 5;
	if (flip_axis) o += 3;
	uint8_t mad;
	switch (o % 4) {
	case 0:
		mad = mx;
		break;
	case 1:
		mad = mv;
		break;
	case 2:
		mad = my;
		break;
	case 3:
		mad = mx | my | mv;
		break;
	}
	if (flip_axis)
		mad ^= mad & mv ? my : mx;
	if (m_xflip)
		mad ^= mx;
	if (m_yflip)
		mad ^= my;
	if (m_bgr)
		mad |= 8;
	if (mad == m_madval)
		return;
	uint8_t bytes[2] = {0x36, mad};
	write_bytes_(mode_cmd_head, bytes, sizeof(bytes));
	m_madval = mad;
}

/* Put pixel */
void GenRGB16Adaptor::put_pixel(uint16_t x, uint16_t y, uint16_t colour)
{
	write_begin(x, y, x, y, false);
	write_pixel_(colour);
	write_end();
}

/* Fill certain region */
void GenRGB16Adaptor::fill_rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour)
{
	write_begin(x0, y0, x1, y1, false);
	for (uint16_t j = y0; j <= y1; ++j) {
		for (uint16_t i = x0; i <= x1; ++i) {
			write_pixel_(colour);
		}
	}
	write_end();
}

/* Setup rectangular writing area */
void GenRGB16Adaptor::write_begin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool col_order)
{
	uint16_t px, py;
	uint8_t const o = m_o + m_r;
    switch (o % 4) {
	case 0:
		px = m_pleft;
		py = m_ptop;
		break;
	case 1:
		px = m_ptop;
		py = m_pright;
		break;
	case 2:
		px = m_pright;
		py = m_pbottom;
		break;
	case 3:
		px = m_pbottom;
		py = m_pleft;
		break;
    }
	select();
	if (col_order) {
		set_write_order_(true);
		set_write_window_(y0 + py, x0 + px, y1 + py, x1 + px);
	} else {
		set_write_order_(false);
		set_write_window_(x0 + px, y0 + py, x1 + px, y1 + py);
	}
}

/* Write pixels */
void GenRGB16Adaptor::write_pixels(uint16_t const* pix_buff, int len, int pgm)
{
	for (; len > 0; --len, ++pix_buff) {
		uint16_t pix = !pgm ? *pix_buff : pgm_read_word(pix_buff);
		write_pixel_(pix);
	}
}

/* Write pixels bitmap */
void GenRGB16Adaptor::write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2], int pgm)
{
	uint8_t bit = 0;
	uint8_t pixels = 0;
	for (; len > 0; --len) {
		bit <<= 1;
		if (!bit) {
			bit = 1;
			pixels = !pgm ? *pix_bm : pgm_read_byte(pix_bm);
			++pix_bm;
		}
		write_pixel_(colours[(pixels & bit) != 0]);
	}
}

/* End writing */
void GenRGB16Adaptor::write_end()
{
	unselect();
}

/* Set scrolling region */
void GenRGB16Adaptor::set_scroll_range(uint16_t fr, uint16_t to)
{
	if (!to)
		to = m_h;
	fr += m_ptop; to += m_ptop;
	// Assume real height is m_h + m_ptop + m_pbottom
	uint16_t sa = to - fr, ba = m_h + m_ptop + m_pbottom - to;
	uint8_t bytes[] = {
		0x33,
		(uint8_t)(fr >> 8), (uint8_t)(fr),
		(uint8_t)(sa >> 8), (uint8_t)(sa),
		(uint8_t)(ba >> 8), (uint8_t)(ba)
	};
	write_bytes(mode_cmd_head, bytes, sizeof(bytes));
}

/* Set scrolling position */
void GenRGB16Adaptor::set_scroll_pos(uint16_t pos)
{
	pos += m_ptop;
	uint8_t bytes[] = {
		0x37,
		(uint8_t)(pos >> 8), (uint8_t)(pos)
	};
	write_bytes(mode_cmd_head, bytes, sizeof(bytes));
}
