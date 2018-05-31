#pragma once 

#include <Arduino.h>

#include "BW8DisplayAdaptor.h"

/*
 * SH1106 based display adaptor
 */

#define DISP_W 128

typedef enum {
	SSD1306_128x64,
	SSD1306_128x32
} SSD1306_Model_t;

class SSD1306_Adaptor : public BW8DisplayAdaptor {
public:
	SSD1306_Adaptor() : m_height(64), m_mode(0x12) {}

	/* Set display model. Must be called before init() */
	bool set_model(SSD1306_Model_t m);

	virtual bool probe();
	virtual bool init();
	virtual bool enable(bool on);
	virtual bool light_all(bool active);
	virtual bool set_inverse(bool active);
	virtual bool set_brightness(uint8_t val);
	virtual bool write(uint8_t col, uint8_t pg, uint8_t const* data, unsigned len);

	virtual bool clear_region(uint8_t col, uint8_t pg, uint8_t w, uint8_t h);
	virtual bool clear() { return clear_region(0, 0, DISP_W, m_height/DISP_PG_SZ); }

	virtual uint8_t width() const { return DISP_W; }
	virtual uint8_t height() const { return m_height; }

protected:
	bool wr_start(uint8_t col, uint8_t pg);
	/* Protocol implementation is yet to be defined */
	virtual bool wr_cmd(uint8_t cmd) = 0;
	virtual bool wr_cmds(uint8_t const* cmds, unsigned ncmds) = 0;
	virtual bool wr_data(uint8_t const* data, unsigned nbytes) = 0;
	
	uint8_t m_height;
	uint8_t m_mode;
};
