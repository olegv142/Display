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
}

/* Perform hardware reset */
void SPIDisplay::reset()
{
	digitalWrite(m_rst, LOW);
	delay(5);
	digitalWrite(m_rst, HIGH);
	delay(5);
}

/* Write bytes to the device. If cmd is true the first byte is the command */
void SPIDisplay::write_bytes_(bool cmd, uint8_t const* bytes, uint8_t len, bool pgmem)
{
	digitalWrite(m_dc, cmd ? LOW : HIGH);
	for (; len; --len, ++bytes) {
		uint8_t byte = !pgmem ? *bytes : pgm_read_byte(bytes);
		transfer(byte);
		if (cmd) {
			digitalWrite(m_dc, HIGH);
			cmd = false;
		}
	}
}

/* Write sequence of commands. Each command bytes should be prefixed by the length byte. The whole sequence should be terminated by 0 byte. */
void SPIDisplay::write_cmds_(uint8_t const* buff, bool pgmem)
{
	for (;;) {
		uint8_t len = !pgmem ? *buff : pgm_read_byte(buff);
		if (!len)
			break;
		++buff;
		write_bytes_(true, buff, len, pgmem);
		buff += len;
	}
}
