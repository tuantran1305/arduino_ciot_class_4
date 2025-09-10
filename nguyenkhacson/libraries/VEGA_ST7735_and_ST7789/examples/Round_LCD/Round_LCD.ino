/*
  @file Round_LCD.ino
  @brief Interfacing Round LCD to ARIES V2 Board
  @detail This demo is used to display clock using Round LCD and RTC
  
   Reference aries board: https://vegaprocessors.in/blog/interfacing-8x8-led-dot-matrix-to-aries-v2-board/
   
   Libraries used:
   *  Library Name    : Adafruit_GFX Librry
   *  Library Version : 1.11.5  
   *  Library Name    : DS1307RTC
   *  Library Version : 1.4.1 
   *  Library Name    : Time
   *  Library Version : 1.6.1 
   *  Library Name    : VEGA_ST7735_and_ST7789
   *  Library Version : 1.0.0 
   
   *** 1*3 Round_LCD(Colour IPS LCD) ***
   Connections:
   Round_LCD     Aries Board
   VCC          -   3.3V
   GND          -   GND
   CS           -   GPIO-10
   SCK          -   SCLK0
   MOSI         -   MOSI0
   DC           -   GPIO-8

   DS1307 RTC 
 * Device address -0x77 & 0x68
 * Connections:
 * DS1307     Aries Board
 * VCC      -   3.3V
 * GND      -   GND
 * SDA      -   SDA0
 * SCL      -   SCL0
 
   Note:
 * For connecting to port 1 (SCL1 and SDA1) of aries board use the default variable TwoWire Wire(1) instead of TwoWire Wire(0); 
 * For connecting to port 1 (MOSI 1 and SCLK 1) of aries board use the default variable SPIClass SPI(1) instead of SPIClass SPI(0);  
 * Use the Dip Switch-1(DIP_SW-1/GPIO-16) to change display from analog to digital clock and vice versa.
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <DS1307RTC.h>

#define TFT_CS        10    //Connect CS pin of Round LCD to GPIO-10
#define TFT_RST        9    //Or set to -1 and connect to Aries RESET pin
#define TFT_DC         8   //Connect DC pin of Round LCD to GPIO-8

SPIClass SPI(0);
TwoWire Wire(0);

Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

const float pi = 3.14159267 ;
const int clock_center_x=120;
const int clock_center_y=120;

// variables   used to calculate coordinates of points on the circle
int x;
int y;
int x1;
int y11;

// variables to store previous values read off RTC module
int seconds=0;
int minutes;
int hours;

int temp[6];
int i=0;

#define PIN1 16     //GPIO 16 in aries V2 board
#define HIGH 1
#define LOW 0

void setup() {
  draw_clock_face();

  Serial.begin(115200);
  delay(200);
  Serial.println("DS1307RTC Read Test");
  Serial.println("-------------------");
  display.init(240, 240);           // Init ST7789 240x240
  display.fillScreen(ST77XX_BLACK);

  pinMode(PIN1, INPUT);

  delay(1000);
}

void draw_second(int second, int mode){
  y= (64*cos(pi-(2*pi)/60*second))+clock_center_y;
  x =(64*sin(pi-(2*pi)/60*second))+clock_center_x;
  if(mode==1)
    display.drawLine(clock_center_x,clock_center_y,x,y,ST77XX_WHITE); 
  else
    display.drawLine(clock_center_x,clock_center_y,x,y,ST77XX_BLACK);
}

void draw_hour(int hour, int minute, int mode){ 
  y= (38*cos(pi-(2*pi)/12*hour-(2*PI)/720*minute))+clock_center_y;
  x =(38*sin(pi-(2*pi)/12*hour-(2*PI)/720*minute))+clock_center_x;
  y11=(38*cos(pi-(2*pi)/12*hour-(2*PI)/720*minute))+clock_center_y+1;
  x1=(38*sin(pi-(2*pi)/12*hour-(2*PI)/720*minute))+clock_center_x+1;
  
  if (mode==1){
    display.drawLine(clock_center_x,clock_center_y,x,y,ST77XX_WHITE);
    display.drawLine(clock_center_x+1,clock_center_y+1,x1,y11,ST77XX_WHITE);
  }
  else{
    display.drawLine(clock_center_x,clock_center_y,x,y,ST77XX_BLACK);
    display.drawLine(clock_center_x+1,clock_center_y+1,x1,y11,ST77XX_BLACK);
  }  
}

void draw_minute(int minute, int mode){
  y= (64*cos(pi-(2*pi)/60*minute))+clock_center_y;
  x =(64*sin(pi-(2*pi)/60*minute))+clock_center_x;
  if (mode==1)
    display.drawLine(clock_center_x,clock_center_y,x,y,ST77XX_WHITE);   
  else 
    display.drawLine(clock_center_x,clock_center_y,x,y,ST77XX_BLACK);
}

void draw_clock_face(void){
  // draw the center of the clock
  display.drawCircle(clock_center_x,   clock_center_y,3, ST77XX_WHITE);
  display.fillCircle(clock_center_x, clock_center_y,3,   ST77XX_WHITE);
  
  // draw hour pointers around the face of a clock
  for (int   i=0;i<12;i++){
    y= (102*cos(pi-(2*pi)/12*i))+clock_center_y;
    x =(102*sin(pi-(2*pi)/12*i))+clock_center_x;
    y11= (98*cos(pi-(2*pi)/12*i))+clock_center_y;
    x1 =(98*sin(pi-(2*pi)/12*i))+clock_center_x;
    display.drawLine(x1,y11,x,y,ST77XX_WHITE);
  }

  // print string "12" at   the top of the face of the clock  
  display.drawCircle(26*sin(pi)+clock_center_x,   (26*cos(pi))+clock_center_y, 6, ST77XX_BLACK);
  display.fillCircle(26*sin(pi)+clock_center_x,   (26*cos(pi))+clock_center_y, 5, ST77XX_BLACK);
  display.setTextSize(3);             //   Normal 1:1 pixel scale
  display.setTextColor(ST77XX_WHITE); // Draw   white text
  // print string "12" at   the top of the face of the clock 
  display.setCursor(105,4); // Start at top-left   corner
  display.println(F("12"));
}

void redraw_clock_face_elements(void){
  // draw hour pointers around the face of a clock
  for (int   i=0;i<12;i++){
    y= (102*cos(pi-(2*pi)/12*i))+clock_center_y;
    x =(102*sin(pi-(2*pi)/12*i))+clock_center_x;
    y11= (98*cos(pi-(2*pi)/12*i))+clock_center_y;
    x1 =(98*sin(pi-(2*pi)/12*i))+clock_center_x;
    display.drawLine(x1,y11,x,y,ST77XX_WHITE);
  }  
  display.drawCircle(clock_center_x, clock_center_y,3, ST77XX_WHITE);
  display.fillCircle(clock_center_x,   clock_center_y,3, ST77XX_WHITE);
  display.setCursor(105,4); // Start at top-left   corner
  display.println(F("12"));
}

void loop(){
  tmElements_t tm;

   if (RTC.read(tm)) {
        /*-------------------Analog Clock implementation---------------------*/
    if(digitalRead(PIN1) == HIGH){
      if(i == 1){
        display.fillScreen(ST77XX_BLACK);
        i = 0;        
      }
      Serial.println(i);

      if(tm.Second != seconds){
        draw_second(seconds,0);    
        draw_second(tm.Second,1);
  

        if(tm.Second == 00){
          draw_minute(minutes,0);
        }
        draw_minute(tm.Minute,1);
  
        for(int i = 0; i < 10; i++){
          if(((tm.Minute == 10)||(tm.Minute == 20)||(tm.Minute == 30)||(tm.Minute == 40)||(tm.Minute == 50)||(tm.Minute == 00))&&(tm.Second == 00))
            draw_hour(hours,minutes - i,0);  
        }
  
        draw_hour(tm.Hour,tm.Minute,1);
  
        seconds=tm.Second;
        minutes=tm.Minute;
        hours=tm.Hour;
        redraw_clock_face_elements();
        Serial.println("Analog");
      }
    }
      
    /*-------------------Digital Clock implementation---------------------*/

  else{
    if(i == 0){
      display.fillScreen(ST77XX_BLACK);
      i = 1;        
    }
    Serial.println(i);
    temp[0] = tm.Hour % 10;
    temp[1] = tm.Hour / 10;
    display.drawChar(60,100,temp[1] + 48,ST77XX_WHITE, ST77XX_BLACK,3);
    display.drawChar(80,100,temp[0] + 48,ST77XX_WHITE, ST77XX_BLACK,3);
    display.drawCircle(102, 103, 2, ST77XX_WHITE);
    display.drawCircle(102, 118, 2, ST77XX_WHITE);

    temp[2] = tm.Minute % 10;
    temp[3] = tm.Minute / 10;
    display.drawChar(110,100,temp[3] + 48,ST77XX_WHITE, ST77XX_BLACK,3);
    display.drawChar(130,100,temp[2] + 48,ST77XX_WHITE, ST77XX_BLACK,3);
    display.drawCircle(153, 103, 2, ST77XX_WHITE);
    display.drawCircle(153, 118, 2, ST77XX_WHITE);
    
    temp[4] = tm.Second % 10;
    temp[5] = tm.Second / 10;
    display.drawChar(160,100,temp[5] + 48,ST77XX_WHITE, ST77XX_BLACK,3);
    display.drawChar(180,100,temp[4] + 48,ST77XX_WHITE, ST77XX_BLACK,3);
    Serial.println("Digital");
  }   
 }
  else{
    if(RTC.chipPresent()){
      Serial.println("The DS1307 is stopped.  Please run the SetTime");
      Serial.println("example to initialize the time and begin running.");
      Serial.println();
    }
    else{
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(9000);
  }
}
