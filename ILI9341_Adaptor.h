#pragma once

#include "RGB16DisplayAdaptor.h"
#include <SPI.h>

/*
 * ILI9341 display low level interface
 */
class ILI9341_Adaptor : public RGB16DisplayAdaptor {
public:
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

private:
	void reset();
	void configure();
	void select()   { digitalWrite(m_cs, LOW); }
	void unselect() { digitalWrite(m_cs, HIGH); }
	// Helper routines. The ones with name ending by _ should be enclosed between select and unselect calls.
	void write_bytes_(bool cmd, uint8_t const* bytes, uint8_t len, bool pgmem = false);
	void write_bytes(bool cmd, uint8_t const* bytes, uint8_t len, bool pgmem = false) {
		select();
		write_bytes_(cmd, bytes, len, pgmem);
		unselect();
	}
	void write_cmds_(uint8_t const* buff, bool pgmem = false);
	void write_cmds(uint8_t const* buff, bool pgmem = false) {
		select();
		write_cmds_(buff, pgmem);
		unselect();
	}
	void write_cmd(uint8_t cmd) {
		write_bytes(true, &cmd, 1, false);
	}
	void set_write_window_(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	void set_write_pos_(uint16_t x, uint16_t y);
	void set_write_order_(bool flip_axis = false);
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

