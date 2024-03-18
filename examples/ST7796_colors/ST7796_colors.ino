#include "GenRGB16Adaptor.h"

GenRGB16Adaptor g_display(320, 480, 0, 10, 19, 18);

void setup() {
  // put your setup code here, to run once:
  g_display.set_bgr(true);
  g_display.begin();
  g_display.init();
  
  unsigned const h = g_display.height();
  unsigned const w = g_display.width();
  unsigned const b = h / 5;

  unsigned i;
  for (i = 0; i < b; ++i)
    g_display.hline(0, i, w, RGB16(255, 255*i/b, 0));
  for (i = 0; i < b; ++i)
    g_display.hline(0, b + i, w, RGB16(255*(b-i)/b, 255, 0));
  for (i = 0; i < b; ++i)
    g_display.hline(0, b + b + i, w, RGB16(0, 255, 255*i/b));
  for (i = 0; i < b; ++i)
    g_display.hline(0, b + b + b + i, w, RGB16(0, 255*(b-i)/b, 255));
  for (i = 0; i < b; ++i)
    g_display.hline(0, b + b + b + b + i, w, RGB16(255*i/b, 0, 255));
}

void loop() {
}




