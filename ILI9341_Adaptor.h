#pragma once

#include "RGB16DisplayAdaptor.h"
#include <SPI.h>

/*
 * ILI9341 display low level interface
 */
class ILI9341_Adaptor : public RGB16DisplayAdaptor {
public:
	/*
	 * Create display adaptor instance given the screen orientation and control lines pin numbers.
	 */
	ILI9341_Adaptor(uint8_t orientation, uint8_t cs, uint8_t rst, uint8_t dc);

	/* Initialize interface port */
	virtual void begin();

	/* Returns display width */
	virtual uint16_t width() const { return m_w; }

	/* Returns display height */
	virtual uint16_t height() const { return m_h; }

	/* Initialize display */
	virtual void init();

	/* Enable / disable display */
	virtual void enable(bool on);

	/* Put pixel */
	virtual void put_pixel(uint16_t x, uint16_t y, uint16_t colour);

	/* Draw horizontal line */
	virtual void hline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour);

	/* Draw vertical line */
	virtual void vline(uint16_t x, uint16_t y, uint16_t len, uint16_t colour);

	/* Setup rectangular writing area */
	virtual void write_begin(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool col_order);

	/* Write pixels */
	virtual void write_pixels(uint16_t const* pix_buff, int len);

	/* Write pixels bitmap */
	virtual void write_pixels_bm(uint8_t const* pix_bm, int len, uint16_t colours[2]);

	/* End writing */
	virtual void write_end();

	/* Fill certain region */
	virtual void fill_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour);

	/*
	 * Scrolling API
	 */

	/*
	 * Set scrolling range.
	 * The scrolling can only take place along the long axis (320 pixs).
	 * One can reserve fixed areas at both sides of the scrolling area (in the scroll direction)
	 * by specifying scrolling range start and end offsets.
	 */
	void set_scroll_range(uint16_t begin, uint16_t end);

	/*
	 * Set scrolling position. This is the index of the line (starting from the screen top) which will be displayed at the top
	 * of the scrolling area. Note that pos should be in the range begin <= pos < end where begin and end are scrolling area
	 * boundaries. Note that the direction of the pos axis is independent on the screen orientation parameter
	 * (passed to the constructor).
	 */
	void set_scroll_pos(uint16_t pos);

private:
	/* Perform hard / soft reset and wake up screen from the sleep */
	void reset();
	/* Push power-up configuration */
	void configure();
	/* Chip select control helpers */
	void select()   { digitalWrite(m_cs, LOW); }
	void unselect() { digitalWrite(m_cs, HIGH); }

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

	/* Setup memory write window */
	void set_write_window_(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	/* Setup memory write position */
	void set_write_pos_(uint16_t x, uint16_t y);
	/* Setup memory write order according to the screen orientation and optionally flip axis
	 * so writing will proceed in the column order.
	 */
	void set_write_order_(bool flip_axis = false);
	/* Write pixel RGB data */
	void write_pixel_(uint16_t colour) {
		SPI.transfer((uint8_t)(colour >> 8));
		SPI.transfer((uint8_t)(colour));
	}

	uint8_t  m_o;
	uint16_t m_w;
	uint16_t m_h;
	uint8_t  m_cs;
	uint8_t  m_dc;
	uint8_t  m_rst;
};

