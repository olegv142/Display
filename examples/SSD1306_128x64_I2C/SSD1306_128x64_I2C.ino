#include "SSD1306_I2C_Adaptor.h"
#include "glcd_fonts.h"

SSD1306_I2C_Adaptor g_display;
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
  g_display.clear();
  Serial.println(glcd_print_str(&g_display, 0, 0, scnt.c_str(), &g_font_Tahoma44x47D, 1));
  delay(1000);
}

