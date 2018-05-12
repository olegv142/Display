
#include "SH1106_Adaptor.h"

/*
 * The SH1106 supports 132x64 resolution while the display has only 128x64 pixels.
 * So the first and last 2 columns are unused.
 */
#define COL_OFFSET 2

static const uint8_t s_init_cmds[] = {
	0xae,       // off
	0xd5, 0x50, // frequency set
	0xa1,       // segments remap
	0xc8,       // reversed scan
	0xd3, 0,    // no offset
	0x40,       // start line 0
	0xda, 0x12, // common pads alternative mode
	0x81, 0xcf, // contrast
	0xdb, 0x40, // vcom deselect level
	0xa8, DISP_H-1, // multiplex ratio
	0x30,       // pump voltage
	0x40,       // display start line
	0xa4,       // display memory
	0xa6,       // no inverse
	0xaf,       // on
};

static const uint8_t s_zbuff[16] = {0};

bool SH1106_Adaptor::probe()
{
	return wr_cmd(0xE3);
}

bool SH1106_Adaptor::enable(bool on)
{
	return wr_cmd(0xAE + uint8_t(on));
}

bool SH1106_Adaptor::init()
{
	return wr_cmds(s_init_cmds, sizeof(s_init_cmds)) && clear();
}

bool SH1106_Adaptor::light_all(bool active)
{
	return wr_cmd(0xA4 + uint8_t(active));
}

bool SH1106_Adaptor::set_inverse(bool active)
{
	return wr_cmd(0xA6 + uint8_t(active));
}

bool SH1106_Adaptor::set_brightness(uint8_t val)
{
	uint8_t cmds[] = {0x81, val};
	return wr_cmds(cmds, sizeof(cmds));
}

bool SH1106_Adaptor::wr_start(uint8_t col, uint8_t pg)
{
	col += COL_OFFSET;
	uint8_t cmds[] = {
		(uint8_t)(col & 0xf),
		(uint8_t)(0x10 | ((col >> 4) & 0xf)),
		(uint8_t)(0xb0 | (pg & 0xf))
	};
	return wr_cmds(cmds, sizeof(cmds));
}

bool SH1106_Adaptor::clear()
{
	for (unsigned p = 0; p < DISP_H/DISP_PG_SZ; ++p)
	{
		if (!wr_start(0, p)) {
			return false;
		}
		for (unsigned j = 0; j < DISP_W; j += sizeof(s_zbuff)) {
			if (!wr_data(s_zbuff, sizeof(s_zbuff))) {
				return false;
			}
		}
	}
	return true;
}
