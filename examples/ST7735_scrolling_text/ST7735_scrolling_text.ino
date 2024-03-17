#include "GenRGB16Adaptor.h"
#include "glcd_fonts.h"

/*
 * Print the scrolling list of numbers with the most recent line at the top
 */

#define TEXT_MARGIN 16
#define TEXT_SPACING 3

GenRGB16Adaptor g_display(128, 160, 0, 10, 19, 18);

int g_cnt;
int g_line_h;
int g_last_y;
int g_text_area_h;
struct glcd_font const* g_font = &g_font_Tahoma15x16;
uint16_t g_colors[] = {RGB16Black, RGB16Green};

void setup() {
  g_display.set_padding(2, 1);
  g_display.set_flip(true, false);
  g_display.set_bgr(false);

  g_display.begin();
  g_display.init();

  g_line_h = glcd_font_height(g_font);
  g_last_y = g_text_area_h = g_line_h * (g_display.height() / g_line_h);
  g_display.set_scroll_range(0, g_text_area_h);
}

void loop() {
  ++g_cnt;
  String text(g_cnt);
  if (!g_last_y)
    g_last_y = g_text_area_h;
  g_last_y -= g_line_h;
  g_display.set_scroll_pos(g_last_y);
  glcd_print_str(&g_display, TEXT_MARGIN, g_last_y, text.c_str(), g_font, TEXT_SPACING, g_colors);
  delay(1000);
}



