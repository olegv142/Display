#include "GenRGB16Adaptor.h"
#include "draw.h"

GenRGB16Adaptor g_display(240, 240, 0, 10, 19, 18);

void setup() {
  // put your setup code here, to run once:
  g_display.set_flip(true, false);
  g_display.set_padding(0, 0, 0, 80);
  g_display.set_inversion(true);

  g_display.begin();
  g_display.init();
}

void loop() {
  unsigned ra = random(4, 64);
  unsigned x = random(ra, g_display.width() - ra);
  unsigned y = random(ra, g_display.height() - ra);
  unsigned r = 63 * random(0, 5), g = 63 * random(0, 5), b = 63 * random(0, 5);
  draw_circle(&g_display, x, y, ra, 0, RGB16(r, g, b));
}

