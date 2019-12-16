#include "SPIDisplay.h"

/* Initialize interface port */
void SPIDisplay::begin()
{
	SPI.begin();
	// Use fastest clock speed
	SPI.setClockDivider(SPI_CLOCK_DIV2);
	pinMode(m_cs, OUTPUT);
	pinMode(m_dc, OUTPUT);
	pinMode(m_rst, OUTPUT);
	digitalWrite(m_cs, HIGH);
	digitalWrite(m_dc, HIGH);
	reset();
}

/* Perform hardware reset */
void SPIDisplay::reset()
{
	digitalWrite(m_rst, LOW);
	delay(5);
	digitalWrite(m_rst, HIGH);
	delay(5);
}

/* Write bytes to the device. The first parameter determines D/C line state during transfer. */
void SPIDisplay::write_bytes_(write_mode_t mode, uint8_t const* bytes, uint8_t len, bool pgmem)
{
	digitalWrite(m_dc, mode == mode_data ? HIGH : LOW);
	for (; len; --len) {
		uint8_t byte;
		if (bytes != NULL) {
			byte = !pgmem ? *bytes : pgm_read_byte(bytes);
			++bytes;
		} else
			byte = 0;
		transfer(byte);
		if (mode == mode_cmd_head) {
			digitalWrite(m_dc, HIGH);
			mode = mode_data;
		}
	}
	if (mode != mode_data)
		digitalWrite(m_dc, HIGH);
}

/* Write sequence of commands. Each command bytes should be prefixed by the length byte. The whole sequence should be terminated by 0 byte. */
void SPIDisplay::write_cmds_(uint8_t const* buff, bool pgmem)
{
	for (;;) {
		uint8_t len = !pgmem ? *buff : pgm_read_byte(buff);
		if (!len)
			break;
		++buff;
		write_bytes_(mode_cmd_head, buff, len, pgmem);
		buff += len;
	}
}
