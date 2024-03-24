# Display library for Arduino

## Motivation
Why yet another display library? There are a lot of display 'drivers' available for various I2C and SPI displays. Most of them are just the single class implementing interface to the particular display and drawing and character printing capabilities. So what if I want to support another display? I need to create another class that differ on just several lines of code responsible for display initialization. All other stuff related to drawing / printing should be just copy/pasted to the new class. One should not be OOP guru to understand that such design is pathological. We'd better split such display 'driver' onto two classes. The one responsible for display interaction and the one responsible for drawing / printing. Since the second task typically does not require maintaining context its better be implemented as just functions outside of the class.
The second problem with the most of existing implementations is memory handling. They just create frame buffer for the entire image and transfer it to display on any update. For reasonably large displays such frame buffer takes a lot of space in RAM prohibiting usage of that displays in micro-controllers with small RAM size. But in fact there is no need to keep frame buffer in RAM at all. The display already have frame buffer for entire screen where we can create the image drawing and printing directly to that buffer.

The above two ideas are key points of this display library. It have two class hierarchies for display 'adapters' providing interface for various displays and implementing just the minimum set of drawing primitives. The first set of classes supports monochrome (OLED) displays. They receive image as bitmap with single bit representing single pixel on the screen. The second set of classes supports color displays representing every pixel by RGB values packed into 16 bits.

Arduino drivers for the following displays:

| Controller | Type  | Resolution | Interface | Comments |
|--|--|--|--|--|
| SH1106 | OLED monochrome | 128x64 | I2C, SPI+rst+d/c | cheap |
| SSD1305 | OLED monochrome | 128x32 | SPI+rst+d/c | reasonably large, low resolution |
| SSD1306 | OLED monochrome | 128x64, 128x32 | I2C | cheap and small |
| SSD1309 | OLED monochrome | 128x64 | SPI+rst+d/c | reasonably large, protocol-compatible with 1306 |
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

