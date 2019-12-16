# Display
Arduino drivers for the following displays:

| Controller | Type  | Resolution | Interface | Comments |
|--|--|--|--|--|
| SH1106 | OLED monochrome | 128x64 | I2C | cheap |
| SSD1305 | OLED monochrome | 128x32 | SPI | reasonably large, low resolution |
| SSD1306 | OLED monochrome | 128x64, 128x32 | I2C | cheap and small |
| SSD1351 | OLED color | 128x128 | SPI+rst+d/c | expensive |
| ILI9341 | TFT color | 240x320 | SPI+rst+d/c | optimal value for money, feature reach, but power hungry back-lit |

The library has the following distinctive features:
- Extensible class hierarchy
- Small memory footprint (no screen buffer)
- Optimized for fast data transfer
- Wide selection of fonts, some supporting Cyrillic symbols
- New fonts can be easily created by freely available tools like MicoElectronica GLCD fonts creator
- Text printing routines optimized for usage without first cleaning the entire screen area

The class hierarchy is designed with the following key ideas in mind:
- Separate interface from the implementation.
- Separate the core display API from the high level drawing and text printing routines which can use abstract display interface for accessing display. Such code separation allows us to reuse most of the drawing / printing code across different display models.

The ILI9341 based displays turns out to be the most flexible and feature reach. They support different display orientations and text printing directions as well as scrolling so you can implement some dynamic data plotting techniques (like waterfall) without needing to store entire data set and frequently refresh display.

## Author

Oleg Volkov (olegv142@gmail.com)

