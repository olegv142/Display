#pragma once 

#include <Arduino.h>
#include <SPIDisplay.h>

#include "SSD1306_Adaptor.h"

/*
 * SSD1306/9 based display adaptor with SPI communication protocol
 */

class SSD1306_SPI_Adaptor : public SSD1306_Adaptor, protected SPIDisplay {
public:
	SSD1306_SPI_Adaptor(uint8_t cs, uint8_t rst, uint8_t dc) 
		: SPIDisplay(cs, rst, dc)
			{}
	virtual void begin() { SPIDisplay::begin(); }

protected:
	bool wr_cmd(uint8_t cmd) {
			SPIDisplay::write_cmd(cmd);
			return true;
		}
	bool wr_data(uint8_t const* data, unsigned nbytes) {
			SPIDisplay::write_bytes(mode_data, data, nbytes);
			return true;
		}
	bool wr_cmds(uint8_t const* cmds, unsigned ncmds) {
			SPIDisplay::write_bytes(mode_cmd, cmds, ncmds);
			return true;
		}
};
