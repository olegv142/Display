#include "SSD1351_Adaptor.h"

#define REMAP_CONFIG 0x74

/* The power-up initialization configuration */
static const uint8_t s_SSD1351_init_cmds[] PROGMEM = {
	2, 0xfd, 0x12, // unlock
	2, 0xfd, 0xb1, // all commands
	2, 0xa0, REMAP_CONFIG, // remap / color depth
	2, 0xa2, 0,    // first line offset
	2, 0xb3, 0xf0, // maximum frequency to reduce flicker
	0
};

/* Initialize display */
void SSD1351_Adaptor::init(uint16_t fill_colour)
{
	reset();
	configure();
	fill(fill_colour);
	enable(true);
}

/* Push power-up configuration */
void SSD1351_Adaptor::configure()
{
	write_cmds(s_SSD1351_init_cmds, true);
}

/* Enable / disable display */
void SSD1351_Adaptor::enable(bool on)
{
	write_cmd(on ? 0xaf : 0xae);
}

/* Switch all pixels to active state */
void SSD1351_Adaptor::light_all(bool active)
{
	write_cmd(active ? 0xa5 : 0xa6);
}

/* Set brightness value */
void SSD1351_Adaptor::set_brightness(uint8_t val)
{
	uint8_t bytes[] = {0xc7, (uint8_t)(val >> 4)};
	write_bytes(mode_cmd_head, bytes, 2);
}

/* Setup memory write window */
void SSD1351_Adaptor::set_write_window_(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t bytes[] = {
		3, 0x15, x0, x1,
		3, 0x75, y0, y1,
		1, 0x5c,
		0
	};
	write_cmds_(bytes);
}

/* Setup memory write order according to the screen orientation and optionally flip axis
 * so writing will proceed in the column order.
 */
void SSD1351_Adaptor::set_write_order_(bool vertical)
{
	uint8_t bytes[] = {0xa0, REMAP_CONFIG};
	if (vertical)
		bytes[1] |= 1;
	write_bytes_(mode_cmd_head, bytes, sizeof(bytes));
}

/* Put pixel */
void SSD1351_Adaptor::put_pixel(uint16_t x, uint16_t y, uint16_t colour)
{
	select();
	set_write_window_(x, y, x, y);
	write_pixel_(colour);
	unselect();
}

/* Fill certain region */
void SSD1351_Adaptor::fill_rect(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t colour)
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
void SSD1351_Adaptor::write_begin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, bool col_order)
{
	select();
	if (col_order)
		set_write_order_(true);
	set_write_window_(x0, y0, x1, y1);
}

/* Write pixels */
void SSD1351_Adaptor::write_pixels(uint16_t const* pix_buff, int len, int pgm)
{
	for (; len > 0; --len, ++pix_buff) {
		uint16_t pix = !pgm ? *pix_buff : pgm_read_word(pix_buff);
		write_pixel_(pix);
	}
}

/* Write pixels bitmap */
void SSD1351_Adaptor::write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2], int pgm)
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
void SSD1351_Adaptor::write_end()
{
	set_write_order_();
	unselect();
}
