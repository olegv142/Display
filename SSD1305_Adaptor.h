#pragma once 

#include <Arduino.h>

#include "BW8DisplayAdaptor.h"

/*
 * SSD1305 based display adaptor
 * Tested with only 128x32 display only
 */

#define DISP_W 128
#define DISP_H 32

class SSD1305_Adaptor : public BW8DisplayAdaptor {
public:
	SSD1305_Adaptor() {}

	virtual void init();
	virtual void enable(bool on);
	virtual void light_all(bool active);
	virtual void set_inverse(bool active);
	virtual void set_brightness(uint8_t val);
	virtual void write(uint8_t col, uint8_t pg, uint8_t const* data, unsigned len);

	virtual void clear_region(uint8_t col, uint8_t pg, uint8_t w, uint8_t h);
	virtual void clear() { return clear_region(0, 0, DISP_W, DISP_H/DISP_PG_SZ); }

	virtual uint8_t width() const { return DISP_W; }
	virtual uint8_t height() const { return DISP_H; }

protected:
	void wr_start(uint8_t col, uint8_t pg);
	/*
	 * Protocol implementation is yet to be defined
	 */
	virtual void wr_cmd(uint8_t cmd) = 0;
	virtual void wr_cmds(uint8_t const* cmds, unsigned ncmds) = 0;
	virtual void wr_data(uint8_t const* data, unsigned nbytes) = 0;
};
