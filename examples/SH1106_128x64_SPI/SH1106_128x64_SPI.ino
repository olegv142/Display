#include "SH1106_SPI_Adaptor.h"
#include "glcd_fonts.h"

#define CS_PIN  14
#define DC_PIN  15
#define RST_PIN 16

SH1106_SPI_Adaptor g_display(CS_PIN, RST_PIN, DC_PIN);
int g_cnt;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  g_display.begin();
  Serial.print("probe ");
  Serial.println(g_display.probe());
  g_display.init();
}

void loop() {
  ++g_cnt;
  String scnt(g_cnt);
  Serial.println(glcd_print_str_r(&g_display, 0, 0, g_display.width(), scnt.c_str(), &g_font_Tahoma44x47D, 1));
  delay(1000);
}
