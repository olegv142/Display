#pragma once 

#include <Arduino.h>
#include <Wire.h>

#include "SH1106_Adaptor.h"

/*
 * SH1106 based display adaptor with I2C communication protocol
 */

#define SH1106_I2C_DEF_ADDR 0x3c

class SH1106_I2C_Adaptor : public SH1106_Adaptor {
public:
	SH1106_I2C_Adaptor(uint8_t addr = SH1106_I2C_DEF_ADDR) : m_addr(addr) {}
	virtual void begin() { Wire.begin(); }

protected:
	bool wr_cmd(uint8_t cmd);
	bool wr_data(uint8_t const* data, unsigned nbytes);
	bool wr_cmds(uint8_t const* cmds, unsigned ncmds);

	uint8_t	m_addr;
};
