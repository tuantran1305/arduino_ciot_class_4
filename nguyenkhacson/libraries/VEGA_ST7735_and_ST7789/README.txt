This is a library for Adafruit displays SPECIFICALLY FOR USE WITH VEGA ARIES Boards

The Adafruit_ST7735_and_ST7789_Library for Adafruit displays has been forked and 
made compatible for working with VEGA ARIES Boards.

ARIES Pinmap: https://vegaprocessors.in/files/PINOUT_ARIES%20V2.0_.pdf

Check out the above link for ARIES Pinmap. 
 
 *** 1*3 Round_LCD(Colour IPS LCD) ***
   Connections:
   Round_LCD     Aries Board
   VCC          -   3.3V
   GND          -   GND
   CS           -   GPIO-10
   SCK          -   SCLK0
   MOSI         -   MOSI0
   DC           -   GPIO-8
 * For connecting to port 1 of aries board use the default variable SPIClass SPI(1) instead of SPIClass SPI(0);

Other Useful links:

 * Official site: https://vegaprocessors.in/
 * YouTube channel: https://www.youtube.com/@VEGAProcessors


This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618
 
Check out the links above for our tutorials and wiring diagrams.
These displays use SPI to communicate, 4 or 5 pins are required to
interface (RST is optional).

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.
MIT license, all text above must be included in any redistribution.

Recent Arduino IDE releases include the Library Manager for easy installation. Otherwise, to download, click the DOWNLOAD ZIP button, uncompress and rename the uncompressed folder Adafruit_ST7735. Confirm that the Adafruit_ST7735 folder contains Adafruit_ST7735.cpp, Adafruit_ST7735.h and related source files. Place the Adafruit_ST7735 library folder your ArduinoSketchFolder/Libraries/ folder. You may need to create the Libraries subfolder if its your first library. Restart the IDE.

Also requires the Adafruit_GFX library for Arduino.
