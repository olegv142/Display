#include "GenRGB16Adaptor.h"
#include "glcd_fonts.h"

GenRGB16Adaptor g_display(170, 320, 0, 10, 19, 18);

void print_text(uint8_t o)
{
  uint16_t colors[] = {RGB16Black, RGB16Green};
  String s = "Orientation ";
  s += o;
  g_display.rotate_axis(o);
  glcd_print_str(&g_display, 10, 20, s.c_str(), &g_font_Tahoma15x16, 1, colors);
}

void setup() {
  // put your setup code here, to run once:
  g_display.set_flip(true, false);
  g_display.set_padding(35, 35, 0, 0);
  g_display.set_inversion(true);

  g_display.begin();
  g_display.init();

  print_text(0);
  print_text(1);
  print_text(2);
  print_text(3);  
}

void loop() {
}




