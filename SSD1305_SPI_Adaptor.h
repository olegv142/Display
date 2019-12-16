#pragma once 

#include <Arduino.h>
#include <SPIDisplay.h>

#include "SSD1305_Adaptor.h"

/*
 * SSD1305 based display adaptor with SPI communication protocol
 */

class SSD1305_SPI_Adaptor : public SSD1305_Adaptor, protected SPIDisplay {
public:
	SSD1305_SPI_Adaptor(uint8_t cs, uint8_t rst, uint8_t dc) 
		: SPIDisplay(cs, rst, dc)
			{}
	virtual void begin() { SPIDisplay::begin(); }

protected:
	void wr_cmd(uint8_t cmd) {
			SPIDisplay::write_cmd(cmd);
		}
	void wr_data(uint8_t const* data, unsigned nbytes) {
			SPIDisplay::write_bytes(mode_data, data, nbytes);
		}
	void wr_cmds(uint8_t const* cmds, unsigned ncmds) {
			SPIDisplay::write_bytes(mode_cmd, cmds, ncmds);
		}
};
