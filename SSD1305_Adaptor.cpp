
#include "SSD1305_Adaptor.h"

static const uint8_t s_init_cmds[] = {
	0xae,       // off
	0x04, 0x10, // starting column is 4
	0x40,       // starting line is 0
	0x81, 0xcf, // contrast
	0xa0,       // normal segments order
	0xa6,       // no inverse
	0xa8, 0x1f, // MUX = 32
	0xc0,       // COM scan normal
	0xd3, 0,    // no offset
	0xd5, 0xf0, // set frequency to maximum
	0xd8, 0x00, // monochrome mode
	0xd9, 0xc2, // set pre-charge timings
	0xda, 0x12, // alternative COM pins configuration
	0xdb, 0x08, // setup VCOM deselect level
};

void SSD1305_Adaptor::enable(bool on)
{
	wr_cmd(0xAE + uint8_t(on));
}

void SSD1305_Adaptor::init()
{
	wr_cmds(s_init_cmds, sizeof(s_init_cmds));
	clear();
	enable(true);
}

void SSD1305_Adaptor::light_all(bool active)
{
	wr_cmd(0xA4 + uint8_t(active));
}

void SSD1305_Adaptor::set_inverse(bool active)
{
	wr_cmd(0xA6 + uint8_t(active));
}

void SSD1305_Adaptor::set_brightness(uint8_t val)
{
	uint8_t cmds[] = {0x81, val};
	wr_cmds(cmds, sizeof(cmds));
}

void SSD1305_Adaptor::wr_start(uint8_t col, uint8_t pg)
{
	uint8_t cmds[] = {
		(uint8_t)(col & 0xf),
		(uint8_t)(0x10 | ((col >> 4) & 0xf)),
		(uint8_t)(0xb0 | (pg & 0xf))
	};
	return wr_cmds(cmds, sizeof(cmds));
}

void SSD1305_Adaptor::write(uint8_t col, uint8_t pg, uint8_t const* data, unsigned len)
{
	if (col > DISP_W)
		col = DISP_W;
	if (col + len > DISP_W)
		len = DISP_W - col;
	wr_start(col, pg);
	wr_data(data, len);
}

void SSD1305_Adaptor::clear_region(uint8_t col, uint8_t pg, uint8_t w, uint8_t h)
{
	for (uint8_t p = 0; p < h; ++p) {
		write(col, pg + p, NULL, w);
	}
}

