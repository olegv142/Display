#include "SSD1351_Adaptor.h"
#include "draw.h"

SSD1351_Adaptor g_display(14, 15, 16);

void setup() {
  // put your setup code here, to run once:
  g_display.begin();
  g_display.init();

  for (int i = 0; i < 6; ++i)
    draw_circle(&g_display, 10+i*20, 10, i, 0, RGB16Red);

  for (int i = 0; i < 6; ++i)
    draw_circle(&g_display, 10+i*20, 30, i, 1, RGB16Red);

  draw_circle(&g_display, 64, 82, 1,  0, RGB16Green);
  draw_circle(&g_display, 64, 82, 10, 1, RGB16Green);
  draw_circle(&g_display, 64, 82, 20, 2, RGB16Green);
  draw_circle(&g_display, 64, 82, 30, 3, RGB16Green);
  draw_circle(&g_display, 64, 82, 40, 4, RGB16Green);
}

void loop() {
}

