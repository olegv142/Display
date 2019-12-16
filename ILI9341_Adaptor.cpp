#include "ILI9341_Adaptor.h"

/* The power-up initialization configuration. Taken from MI0283QT-9A screen datasheet. */
static const uint8_t s_ILI9341_init_cmds[] PROGMEM = {
	4, 0xCF, 0x00, 0x83, 0x30,
	5, 0xED, 0x64, 0x03, 0x12, 0x81,     
	4, 0xE8, 0x85, 0x01, 0x79,
	6, 0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02,
	2, 0xF7, 0x20,
	3, 0xEA, 0x00, 0x00,
	2, 0xC0, 0x26,			// POWER_CONTROL_1 
	2, 0xC1, 0x11,			// POWER_CONTROL_2
	3, 0xC5, 0x35, 0x3E,		// VCOM_CONTROL_1     
	2, 0xC7, 0xBE,			// VCOM_CONTROL_2     
	2, 0x36, 0x48,			// MEMORY_ACCESS_CONTROL     
	2, 0x3A, 0x55,			// COLMOD_PIXEL_FORMAT_SET - 16 bit pixel 
	3, 0xB1, 0x00, 0x1B,		// Frame Rate     
	2, 0xF2, 0x08,			// Gamma Function Disable
	2, 0x26, 0x01,                  // Gamma curve select
	16, 0xE0, 0x1F, 0x1A, 0x18, 0x0A, 0x0F, 0x06, 0x45, 0x87, 0x32, 0x0A, 0x07, 0x02, 0x07, 0x05, 0x00, // positive gamma correction
	16, 0xE1, 0x00, 0x25, 0x27, 0x05, 0x10, 0x09, 0x3A, 0x78, 0x4D, 0x05, 0x18, 0x0D, 0x38, 0x3A, 0x1F, // negative gamma correction
	2, 0xB7, 0x07,			// entry mode     
	5, 0xB6, 0x0A, 0x82, 0x27, 0x00,// display function control
	0
};

/* Initialize display */
void ILI9341_Adaptor::init(uint16_t fill_colour)
{
	reset();
	configure();
	fill(fill_colour);
	// turn on RAM displaying
	write_cmd(0x29);
}

/* Perform hard / soft reset and wake up screen from the sleep */
void ILI9341_Adaptor::reset()
{
	// hard reset
	SPIDisplay::reset();
	// software reset
	write_cmd(0x1);
	delay(5);
	// wake up
	enable(true);
}

/* Push power-up configuration */
void ILI9341_Adaptor::configure()
{
	select();
	write_cmds_(s_ILI9341_init_cmds, true);
	set_write_order_();
	unselect();
}

/* Enable / disable display */
void ILI9341_Adaptor::enable(bool on)
{
	write_cmd(on ? 0x11 : 0x10);
	if (on) delay(20);
}

/* Set brightness value */
void ILI9341_Adaptor::set_brightness(uint8_t val)
{
	uint8_t bytes[] = {0x51, val};
	write_bytes(mode_cmd_head, bytes, sizeof(bytes));
}

/* Setup memory write window */
void ILI9341_Adaptor::set_write_window_(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
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
void ILI9341_Adaptor::set_write_order_(bool flip_axis)
{
	uint8_t o = m_o + m_r;
	uint8_t bytes[2] = {0x36};
	if (flip_axis) o += 3;
	switch (o % 4) {
	case 0:
		bytes[1] = 0x48;
		break;
	case 1:
		bytes[1] = 0x28;
		break;
	case 2:
		bytes[1] = 0x88;
		break;
	case 3:
		bytes[1] = 0xE8;
		break;
	}
	if (flip_axis) {
		bytes[1] ^= bytes[1] & 0x20 ? 0x80 : 0x40;
	}
	write_bytes_(mode_cmd_head, bytes, sizeof(bytes));
}

/* Put pixel */
void ILI9341_Adaptor::put_pixel(uint16_t x, uint16_t y, uint16_t colour)
{
	select();
	set_write_window_(x, y, x, y);
	write_pixel_(colour);
	unselect();
}

/* Fill certain region */
void ILI9341_Adaptor::fill_rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour)
{
	select();
	set_write_window_(x0, y0, x1, y1);
	for (uint16_t j = y0; j <= y1; ++j) {
		for (uint16_t i = x0; i <= x1; ++i) {
			write_pixel_(colour);
		}
	}
	unselect();
}

/* Setup rectangular writing area */
void ILI9341_Adaptor::write_begin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool col_order)
{
	select();
	if (col_order) {
		set_write_order_(true);
		set_write_window_(y0, x0, y1, x1);
	} else {
		set_write_window_(x0, y0, x1, y1);
	}
}

/* Write pixels */
void ILI9341_Adaptor::write_pixels(uint16_t const* pix_buff, int len, int pgm)
{
	for (; len > 0; --len, ++pix_buff) {
		uint16_t pix = !pgm ? *pix_buff : pgm_read_word(pix_buff);
		write_pixel_(pix);
	}
}

/* Write pixels bitmap */
void ILI9341_Adaptor::write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2], int pgm)
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
void ILI9341_Adaptor::write_end()
{
	set_write_order_();
	unselect();
}

/* Set scrolling region */
void ILI9341_Adaptor::set_scroll_range(uint16_t fr, uint16_t to)
{
	uint16_t sa = to - fr, ba = 320 - to;
	uint8_t bytes[] = {
		0x33,
		(uint8_t)(fr >> 8), (uint8_t)(fr),
		(uint8_t)(sa >> 8), (uint8_t)(sa),
		(uint8_t)(ba >> 8), (uint8_t)(ba)
	};
	write_bytes(mode_cmd_head, bytes, sizeof(bytes));
}

/* Set scrolling position */
void ILI9341_Adaptor::set_scroll_pos(uint16_t pos)
{
	uint8_t bytes[] = {
		0x37,
		(uint8_t)(pos >> 8), (uint8_t)(pos)
	};
	write_bytes(mode_cmd_head, bytes, sizeof(bytes));
}
