#pragma once

#include <Arduino.h>
#include <SPI.h>

class SPIDisplay {
public:
	SPIDisplay(uint8_t cs, uint8_t rst, uint8_t dc)
		: m_cs(cs), m_dc(dc), m_rst(rst) {}

	/* Initialize interface port */
	void begin();

	/* Perform hardware reset */
	void reset();

	/* Chip select control helpers */
	void select()   { digitalWrite(m_cs, LOW); }
	void unselect() { digitalWrite(m_cs, HIGH); }

	/* Transfer byte */
	void transfer(uint8_t byte) __attribute__((always_inline)) { SPI.transfer(byte); }

	/*
	 * Helper routines. The ones with name ending by _ should be enclosed between select and unselect calls.
	 */

	/* Write bytes to the device. If cmd is true the first byte is the command */
	void write_bytes_(bool cmd, uint8_t const* bytes, uint8_t len, bool pgmem = false);
	void write_bytes(bool cmd, uint8_t const* bytes, uint8_t len, bool pgmem = false) {
		select();
		write_bytes_(cmd, bytes, len, pgmem);
		unselect();
	}
	/* Write sequence of commands. Each command bytes should be prefixed by the length byte. The whole sequence should be terminated by 0 byte. */
	void write_cmds_(uint8_t const* buff, bool pgmem = false);
	void write_cmds(uint8_t const* buff, bool pgmem = false) {
		select();
		write_cmds_(buff, pgmem);
		unselect();
	}
	/* Write single command without parameters */
	void write_cmd(uint8_t cmd) {
		write_bytes(true, &cmd, 1, false);
	}

private:
	uint8_t  m_cs;
	uint8_t  m_dc;
	uint8_t  m_rst;
};
