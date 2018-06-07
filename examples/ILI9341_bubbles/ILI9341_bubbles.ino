#include "ILI9341_Adaptor.h"
#include "draw.h"

ILI9341_Adaptor g_display(1, 14, 15, 16);

void setup() {
  // put your setup code here, to run once:
  g_display.begin();
  g_display.init();
}

static uint16_t colors[] = {
  RGB16(0xff, 0, 0),
  RGB16(0xff, 0x80, 0),
  RGB16(0xff, 0, 0x80),
  RGB16(0xff, 0x80, 0x80),
  RGB16(0, 0xff, 0),
  RGB16(0x80, 0xff, 0),
  RGB16(0, 0xff, 0x80),
  RGB16(0x80, 0xff, 0x80),
  RGB16(0xff, 0xff, 0),
  RGB16(0xff, 0xff, 0x80),
  RGB16(0x80, 0, 0xff),
  RGB16(0, 0x80, 0xff),
  RGB16(0x80, 0x80, 0xff),
  RGB16(0xff, 0, 0xff),
  RGB16(0xff, 0x80, 0xff),
  RGB16(0, 0xff, 0xff),
  RGB16(0x80, 0xff, 0xff),
  RGB16(0xff, 0xff, 0xff),
};

void loop() {
  uint16_t r = random(4, 64);
  uint16_t x = random(r, g_display.width() - r);
  uint16_t y = random(r, g_display.height() - r);
  uint16_t c = colors[random(0, sizeof(colors)/sizeof(*colors))];
  draw_circle(&g_display, x, y, r, 0, c);
}



