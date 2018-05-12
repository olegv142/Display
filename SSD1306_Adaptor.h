#pragma once 

#include <Arduino.h>

#include "DisplayAdaptor.h"

/*
 * SH1106 based display adaptor
 */

#define DISP_W 128
#define DISP_H 64
#define DISP_PG_SZ 8

class SSD1306_Adaptor : public DisplayAdaptor {
public:
	virtual bool probe();
	virtual bool init();
	virtual bool enable(bool on);
	virtual bool light_all(bool active);
	virtual bool set_inverse(bool active);
	virtual bool set_brightness(uint8_t val);
	virtual bool clear();
	virtual bool write(uint8_t col, uint8_t pg, uint8_t const* data, unsigned len) {
		return wr_start(col, pg) && wr_data(data, len);
	}

protected:
	bool wr_start(uint8_t col, uint8_t pg);
	/* Protocol implementation is yet to be defined */
	virtual bool wr_cmd(uint8_t cmd) = 0;
	virtual bool wr_cmds(uint8_t const* cmds, unsigned ncmds) = 0;
	virtual bool wr_data(uint8_t const* data, unsigned nbytes) = 0;
};
