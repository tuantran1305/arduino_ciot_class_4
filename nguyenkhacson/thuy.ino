#include <Arduino.h>
#include <Arduino_GFX_Library.h>
#include "video_frames.h"

#define LCD_CS    5
#define LCD_DC    2
#define LCD_RST   4
#define LCD_MOSI 23
#define LCD_SCK  18
#define LCD_LED  21  // Backlight

Arduino_DataBus *bus = new Arduino_ESP32SPI(LCD_DC, LCD_CS, LCD_SCK, LCD_MOSI, -1);
Arduino_ST7789 *tft = new Arduino_ST7789(bus, LCD_RST, 0, true, 128, 128, 0, 0);

void setup() {
ledcAttach(LCD_LED, 5000, 8); // pin, freq, resolution (API mới ESP32)
ledcWrite(LCD_LED, 255);      // max sáng (0-255)
tft->begin();
}

void loop() {
  for (int i = 0; i < VIDEO_FRAMES; i++) {
   tft->draw16bitRGBBitmap(0, 0, video_data[i], VIDEO_WIDTH, VIDEO_HEIGHT);

    delay(100); // 10 fps
  }
}