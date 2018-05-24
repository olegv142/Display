
#include "SSD1306_Adaptor.h"

static const uint8_t s_init_cmds[] = {
	0xae,       // off
	0xd5, 0x80, // frequency set
	0xa1,       // segments remap
	0xc8,       // reversed scan
	0xd3, 0,    // no offset
	0x40,       // start line 0
	0x81, 0xcf, // contrast
	0xdb, 0x40, // vcom deselect level
	0x8d, 0x14, // pump on
	0x20, 2,    // set page addressing mode
	0x40,       // display start line
	0xa4,       // display memory
	0xa6,       // no inverse
	0x2e,       // no scroll
	0xaf,       // on
};

/* Set display model. Must be called before init() */
bool SSD1306_Adaptor::set_model(SSD1306_Model_t m)
{
	switch (m) {
	case SSD1306_128x64:
		m_height = 64;
		m_mode = 0x12;
		return true;
	case SSD1306_128x32:
		m_height = 32;
		m_mode = 0x2;
		return true;
	default:
		return false;
	}
}

bool SSD1306_Adaptor::probe()
{
	return wr_cmd(0xE3);
}

bool SSD1306_Adaptor::enable(bool on)
{
	return wr_cmd(0xAE + uint8_t(on));
}

bool SSD1306_Adaptor::init()
{
	uint8_t hmode_cmds[] = {0xa8, (uint8_t)(m_height-1), 0xda, m_mode};
	return	wr_cmds(s_init_cmds, sizeof(s_init_cmds)) &&
		wr_cmds(hmode_cmds, sizeof(hmode_cmds)) &&
		clear();
}

bool SSD1306_Adaptor::light_all(bool active)
{
	return wr_cmd(0xA4 + uint8_t(active));
}

bool SSD1306_Adaptor::set_inverse(bool active)
{
	return wr_cmd(0xA6 + uint8_t(active));
}

bool SSD1306_Adaptor::set_brightness(uint8_t val)
{
	uint8_t cmds[] = {0x81, val};
	return wr_cmds(cmds, sizeof(cmds));
}

bool SSD1306_Adaptor::wr_start(uint8_t col, uint8_t pg)
{
	uint8_t cmds[] = {
		(uint8_t)(col & 0xf),
		(uint8_t)(0x10 | ((col >> 4) & 0xf)),
		(uint8_t)(0xb0 | (pg & 0xf))
	};
	return wr_cmds(cmds, sizeof(cmds));
}

bool SSD1306_Adaptor::write(uint8_t col, uint8_t pg, uint8_t const* data, unsigned len)
{
	if (col >= DISP_W)
		return false;
	if (col + len > DISP_W)
		len = DISP_W - col;
	return wr_start(col, pg) && wr_data(data, len);
}

bool SSD1306_Adaptor::clear_region(uint8_t col, uint8_t pg, uint8_t w, uint8_t h)
{
	for (uint8_t p = 0; p < h; ++p) {
		if (!write(col, pg + p, NULL, w))
			return false;
	}
	return true;
}

