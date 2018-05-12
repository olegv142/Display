
#include "SSD1306_I2C_Adaptor.h"

bool SSD1306_I2C_Adaptor::wr_cmd(uint8_t cmd)
{
	Wire.beginTransmission(m_addr);
	Wire.write(0x80);
	Wire.write(cmd);
	return Wire.endTransmission() == 0;
}

bool SSD1306_I2C_Adaptor::wr_cmds(uint8_t const* cmds, unsigned ncmds)
{
	for (unsigned i = 0; i < ncmds; ++i) {
		if (!wr_cmd(cmds[i]))
			return false;
	}
	return true;
}

#define MAX_DATA_CHUNK (BUFFER_LENGTH-2)

bool SSD1306_I2C_Adaptor::wr_data(uint8_t const* data, unsigned nbytes)
{
	for (unsigned i = 0; i < nbytes;)
	{
		Wire.beginTransmission(m_addr);
		Wire.write(0x40);
		for (unsigned j = 0; j < MAX_DATA_CHUNK && i < nbytes; ++i, ++j) {
			Wire.write(data[i]);
		}
		if (Wire.endTransmission() != 0) {
			return false;
		}
	}
	return true;
}
