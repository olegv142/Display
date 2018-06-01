#pragma once 

#include <Arduino.h>

#include "BW8DisplayAdaptor.h"

/*
 * SH1106 based display adaptor
 */

#define DISP_W 128
#define DISP_H 64

class SH1106_Adaptor : public BW8DisplayAdaptor {
public:
	bool probe();

	virtual void init();
	virtual void enable(bool on);
	virtual void light_all(bool active);
	virtual void set_inverse(bool active);
	virtual void set_brightness(uint8_t val);
	virtual void write(uint8_t col, uint8_t pg, uint8_t const* data, unsigned len);

	virtual void clear_region(uint8_t col, uint8_t pg, uint8_t w, uint8_t h);
	virtual void clear() { clear_region(0, 0, DISP_W, DISP_H/DISP_PG_SZ); }

	virtual uint8_t width() const { return DISP_W; }
	virtual uint8_t height() const { return DISP_H; }

protected:
	bool wr_start(uint8_t col, uint8_t pg);
	/* Protocol implementation is yet to be defined */
	virtual bool wr_cmd(uint8_t cmd) = 0;
	virtual bool wr_cmds(uint8_t const* cmds, unsigned ncmds) = 0;
	virtual bool wr_data(uint8_t const* data, unsigned nbytes) = 0;
};
