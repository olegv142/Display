# Display library for Arduino

## Motivation
Why yet another display library? There are a lot of display 'drivers' available for various I2C and SPI displays. Most of them are just the single class implementing interface to the particular display and drawing and character printing capabilities. So what if I want to support another display? I need to create another class that differ on just several lines of code responsible for display initialization. All other stuff related to drawing / printing should be just copy/pasted to the new class. One should not be OOP guru to understand that such design is pathological. We'd better split such display 'driver' onto two classes. The one responsible for display interaction and the one responsible for drawing / printing. Since the second task typically does not require maintaining context its better be implemented as just functions outside of the class.

The second problem with the most of existing implementations is memory handling. They just create frame buffer for the entire image and transfer it to display on any update. For reasonably large displays such frame buffer takes a lot of space in RAM prohibiting usage of that displays in micro-controllers with small RAM size. But in fact there is no need to keep frame buffer in RAM at all. The display already have frame buffer for entire screen where we can create the image drawing and printing directly to that buffer. With such technique one can drive arbitrary large display using only several dozen bytes in RAM. In case you hit the problem unsolvable using direct drawing to display approach there is an adapter class to create image in the memory buffer. You can create buffer for entire display or just for the small part of it depending on your needs and RAM space available. Once you create image in the buffer it may be transferred to display. Such technique may be used for example for printing text with transparent background so the text may be placed on the top of another image.

## Implementation details
The above two ideas are key points of this display library. It has two class hierarchies for display 'adapters' providing interface for various displays and implementing just the minimum set of drawing primitives. The first set of classes supports monochrome (OLED) displays. They receive image as bitmap with single bit representing single pixel on the screen. The second set of classes supports color displays representing every pixel by RGB values packed into 16 bits.

Most implementations send a lot of commands at the initialization stage configuring every bit of low level details such as power, gamma curve and LCD driving voltages. For my opinion such detailed initialization code has dubious usefulness. Display controllers have reasonable defaults for low level display parameters. They even have programmable storage for some of them. Library can't and should not be aware of the low level parameters of the particular LCD panel. The After some experimenting I keep only absolutely necessary initialization sequence that turns to be the same for several display controllers. So my library has just single generic interface adapter class for all color displays. It supports arbitrary orientation, printing text in arbitrary direction and some advanced techniques like creating waterfall display using scrolling so you don't have to actually move data in the display buffer. You can find code using scrolling in the *examples* folder.

Another part of the library is drawing / printing code utilizing the low level drawing primitives implemented by display adapter classes. The printing routines use GLCD style fonts so you can easily add yet another font with the help of Microe GLCD Font Creator.

## Displays supported

| Controller | Type  | Resolution | Interface | Comments |
|--|--|--|--|--|
| SH1106 | OLED monochrome | 128x64 | I2C, SPI+rst+d/c | cheap |
| SSD1305 | OLED monochrome | 128x32 | SPI+rst+d/c | reasonably large, low resolution |
| SSD1306 | OLED monochrome | 128x64, 128x32 | I2C | cheap and small |
| SSD1309 | OLED monochrome | 128x64 | SPI+rst+d/c | reasonably large, protocol-compatible with 1306 |
| SSD1351 | OLED color | 128x128 | SPI+rst+d/c | expensive color OLED screen |
| ILI9341 | TFT color | 240x320 | SPI+rst+d/c | old inexpensive displays of various sizes |
| ST7735  | TFT color | 128x160 | SPI+rst+d/c | mediocre quality low resolution screen, not recommended |
| ST7789  | IPS color | 240x320 | SPI+rst+d/c | very good colors and viewing angles |
| ST7796  | TFT color | 320x480 | SPI+rst+d/c | good inexpensive high resolution screen |

If you need just small resolution display and don't need colored image then OLED is probably the best choice. The only drawback of OLED displays is burning out. If your OLED display is on 24/7 it won't last longer than a couple of years. For medium sized color displays the best choices are ST7789 based displays. They typically have best of the class color rendition and excellent viewing angles. They look exactly how IPS displays should look unlike other displays that are IPS only on paper. If you need 320x480 resolution the only display supported so far is ST7796. It is not expensive and reasonably good though not that good as ST7789 based displays. The ILI9488 display happens to have some not yet understood incompatibility with generic RGB16 adapter so its not supported by the library.

Note that some small displays may be sold without CS pin on external connector. Don't ever buy / use them. Without CS pin SPI interface has no chances to recover once it turns out of sync.

## Compatibility

The library was originally written for AVR micro-controllers like ATmega328P and ATmega32U4. They don't have enough RAM to maintain frame buffer but can drive any display with this library. Lately it was used in several production projects with STM32 micro-controllers. The code in *compat* folder allows for using the library code in non-Arduino projects without modifications (hopefully).

## Author

Oleg Volkov (olegv142@gmail.com)

