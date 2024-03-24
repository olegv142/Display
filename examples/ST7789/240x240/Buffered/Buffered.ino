/*
 * Example illustrates using image buffer
 */

#include "GenRGB16Adaptor.h"
#include "RGB16BuffAdaptor.h"
#include "glcd_fonts.h"

#define BUFF_W 32
#define BUFF_H 16

uint16_t im_buff_data[BUFF_W*BUFF_H];
RGB16BuffAdaptor im_buff(im_buff_data, BUFF_W, BUFF_H);
GenRGB16Adaptor g_display(240, 240, 0, 10, 19, 18);

void print_text(uint8_t o)
{
  uint16_t colors[] = {RGB16Black, RGB16Green};
  String s = "o";
  s += o;
  glcd_print_str(&im_buff, 0, 0, s.c_str(), &g_font_Tahoma15x16, 1, colors);
  
  g_display.rotate_axis(o);

  g_display.write_begin(20, 4, 20 + BUFF_W - 1, 4 + BUFF_H - 1, true);
  g_display.write_pixels(im_buff_data, BUFF_W*BUFF_H);
  g_display.write_end();
}

void setup() {
  // put your setup code here, to run once:
  g_display.set_flip(true, false);
  g_display.set_padding(0, 0, 0, 80);
  g_display.set_inversion(true);

  g_display.begin();
  g_display.init();
  
  im_buff.init();

  print_text(0);
  print_text(1);
  print_text(2);
  print_text(3);  
}

void loop() {
}
