#pragma once

#include "xcommon.h"

/* The write mode determines the state of the Data/Command interface line during transfer.
 * The data mode is used for display RAM writing. Commands used to have parameters that follow the command byte.
 * Some controllers treat them as the data. They should use mode_cmd_head that ensures that only the first
 * transferred byte is treated as the command. Other controllers treat parameters as commands so the mode_cmd
 * should be used.
 */
typedef enum {
	mode_data,     // data
	mode_cmd,      // commands
	mode_cmd_head, // only heading byte is command
} write_mode_t;

#ifndef SPI_DISPLAY_SETTINGS
#define SPI_DISPLAY_SETTINGS 8000000, MSBFIRST, SPI_MODE0
#endif

class SPIDisplay {
public:
	SPIDisplay(Pin cs, Pin rst, Pin dc)
		: m_settings(SPI_DISPLAY_SETTINGS)
		, m_cs(cs), m_dc(dc), m_rst(rst) {}

	/* Initialize interface port */
	void begin();

	/* Perform hardware reset */
	void reset();

	/* Chip select control helpers */
	void select() {
		digitalWrite(m_cs, LOW);
		SPI.beginTransaction(m_settings);
	}
	void unselect() {
		SPI.endTransaction();
		digitalWrite(m_cs, HIGH);
	}

	/* Transfer byte */
	void transfer(uint8_t byte) { SPI.transfer(byte); }

	/*
	 * Helper routines. The ones with name ending by _ should be enclosed between select and unselect calls.
	 */

	/* Write bytes to the device. The first parameter determines D/C line state during transfer. */
	void write_bytes_(write_mode_t mode, uint8_t const* bytes, uint8_t len, bool pgmem = false);
	void write_bytes(write_mode_t mode, uint8_t const* bytes, uint8_t len, bool pgmem = false) {
		select();
		write_bytes_(mode, bytes, len, pgmem);
		unselect();
	}

	/* Write sequence of commands with mode_cmd_head. Each command bytes should be prefixed by the length byte.
	 * The whole sequence should be terminated by 0 byte. Note that unless command parameters are treated as data.
	 * the write_bytes_ with mode_cmd will suffice.
	 */
	void write_cmds_(uint8_t const* buff, bool pgmem = false);
	void write_cmds(uint8_t const* buff, bool pgmem = false) {
		select();
		write_cmds_(buff, pgmem);
		unselect();
	}

	/* Write single command without parameters */
	void write_cmd_(uint8_t cmd) {
		write_bytes_(mode_cmd, &cmd, 1, false);
	}
	void write_cmd(uint8_t cmd) {
		select();
		write_cmd_(cmd);
		unselect();
	}

private:
	spi_settings_t m_settings;
	Pin     m_cs;
	Pin     m_dc;
	Pin     m_rst;
};
