/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-display
 */
#include <Arduino.h>
 #include <Wire.h>
 #include <Adafruit_GFX.h>
 #include <Adafruit_SSD1306.h>
 
 #define SCREEN_WIDTH 128 // display display width,  in pixels
 #define SCREEN_HEIGHT 64 // display display height, in pixels
 
 // declare an SSD1306 display object connected to I2C
 Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
 
 void setup() {
   Serial.begin(9600);
 
   // initialize display display with address 0x3C for 128x64
   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
     Serial.println(F("SSD1306 allocation failed"));
     while (true);
   }
 
   delay(2000);         // wait for initializing
   display.clearDisplay(); // clear display
 
   display.setTextSize(1);          // text size
   display.setTextColor(WHITE);     // text color
   display.setCursor(0, 10);        // position to display
   display.println("Hello World!"); // text to display
   display.display();               // show on display
 }
 
 void loop() {

 }
 