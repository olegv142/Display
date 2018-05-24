#include "SH1106_I2C_Adaptor.h"
#include "glcd_fonts.h"

SH1106_I2C_Adaptor g_display;
int g_cnt;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("probe ");
  Serial.println(g_display.probe());
  Serial.println(g_display.init());
}

void loop() {
  ++g_cnt;
  String scnt(g_cnt);
  Serial.println(glcd_print_str_r(&g_display, 0, 0, g_display.width(), scnt.c_str(), &g_font_Tahoma44x47D, 1));
  delay(1000);
}

