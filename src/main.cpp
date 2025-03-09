#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include <Servo.h>
#include <Adafruit_SSD1306.h>
#include <avr/pgmspace.h> // Include PROGMEM library

// Pin definitions
#define fsrPin A0          // FSR sensor connected to A1
#define tempPin1 A1
#define tempPin2 A2
#define button1 A3
#define redPin 4           // Red pin for RGB LED
#define greenPin 5         // Green pin for RGB LED
#define bluePin 6          // Blue pin for RGB LED
#define buzzerPin 7        // Buzzer connected to D7
#define servoPin 8         // Servo connected to digital pin 2

//const int fsrThreshold = 100;       // Threshold for FSR activation

// const float threshold = 0.5;        // Sensitivity for Z-axis acceleration
// const unsigned int readInterval = 5000; // Read every 5 seconds (Z-axis)
// const unsigned int bpmInterval = 15000; // Calculate BPM every 15 seconds

// Variables for BPM calculation
// unsigned int lastPressTime = 0;    // Time when the last press was detected
// float lastBPM = 0.0;                // Store last calculated BPM
// int heartRate = 0;                  // Simulated heart rate (BPM)

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Accelerometer setup
Adafruit_ADXL343 accel = Adafruit_ADXL343(12345);

// Servo setup
Servo myServo;

// Function to run display
// void displayBPM(int heartRate) {
//     static int lastDisplayedBPM = -1; // Store last displayed BPM
//     if (heartRate != lastDisplayedBPM) { // Only update if BPM has changed
//         display.clearDisplay();
//         // display.setTextSize(2);
//         display.setTextColor(WHITE);
//         display.setCursor(0, 20);
//         // display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE);
//         display.setTextSize(3);
//         display.print("BPM: ");
//         display.println(heartRate);
//         display.display();
//         lastDisplayedBPM = heartRate; // Update last displayed BPM
//     }
// }

// Heart icon (16x16 pixels)
const unsigned char Heart_Icon [] PROGMEM = {
    0x00, 0x00, 0x18, 0x30, 0x3c, 0x78, 0x7e, 0xfc, 0xff, 0xfe, 0xff, 0xfe, 0xee, 0xee, 0xd5, 0x56, 
    0x7b, 0xbc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00
};

// Function to set RGB color
void setRGBColor(uint8_t r, uint8_t g, uint8_t b) {
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
}

// Function to read temperature from a sensor
float readTemperature(int pin) {
    int reading = analogRead(pin);
    float voltage = reading * (5.0 / 1024.0); // Convert analog reading to voltage
    float tempC = (voltage - 0.5) * 100;      // Convert voltage to Celsius
    float tempF = tempC * 9 / 5 + 32;         // Convert Celsius to Fahrenheit
    return tempF;                             // Return temperature in Fahrenheit
}

void setup() {
    Serial.begin(9600);

    // Initialize OLED display
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        while (true);
    }

    // Initialize accelerometer
    // if (!accel.begin()) {
    //     Serial.println(F("ADXL343 not detected! Check wiring."));
    //     while (1);
    // }
    //accel.setRange(ADXL343_RANGE_2_G);
    
    // Initialize pins
    pinMode(buzzerPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    
    // Initialize Servo
    //myServo.attach(servoPin);  // Attach the servo to digital pin 2
    //myServo.write(0);          // Initially set the servo to 0 degrees
    
    // Set initial RGB color to Green
    setRGBColor(0, 255, 0);  // Green

    display.clearDisplay(); // Clear buffer
    display.display();
    Serial.println("Setup done!");
}

void loop() {
    int fsrReading = analogRead(fsrPin);
    Serial.print("FSR: ");
    Serial.println(fsrReading);
    if (fsrReading > 638) {
        setRGBColor(0, 255, 255); //Cyan
        delay(100);
        setRGBColor(0, 0, 0);
        delay(100);
        setRGBColor(0, 255, 255); //Cyan
        delay(100);
        setRGBColor(0, 0, 0);
        delay(100);
        setRGBColor(0, 255, 255); //Cyan
        delay(100);
        setRGBColor(0, 0, 0);
        delay(100);
    }

    // Read temperatures from both sensors
    float temp1 = readTemperature(tempPin1);
    Serial.print("Temp Sensor 1: ");
    Serial.println(temp1);
    float temp2 = readTemperature(tempPin2);
    Serial.print("Temp Sensor 2: ");
    Serial.println(temp2);

    // Calculate the difference between the two temperatures
    float tempDiff = abs(temp1 - temp2);
    Serial.print("Temp diff: ");
    Serial.println(tempDiff);

    // Update OLED display with both temperatures
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.drawBitmap(100, 0, Heart_Icon, 16, 16, WHITE); // Draw at (100, 0)
    display.setCursor(0, 0);
    display.print("Temp1: ");
    display.print(temp1);
    display.print(" F");
    display.setCursor(0, 20);
    display.print("Temp2: ");
    display.print(temp2);
    display.print(" F");
    display.setCursor(0, 40);
    display.print("Diff: ");
    display.print(tempDiff);
    display.print(" F");
    display.display();

    // Control RGB LED based on temperature difference
    if (tempDiff < 5.0) {
        setRGBColor(0, 255, 0);  // Green for small difference
        noTone(buzzerPin);
    }else  if (tempDiff >= 5.0 && tempDiff < 10.0) {
        setRGBColor(255, 255, 0); // Yellow for moderate difference
        noTone(buzzerPin);
    }else  if (tempDiff >= 10.0 && tempDiff < 50.0){
        setRGBColor(255, 0, 0);   // Red for large difference
        tone(buzzerPin, 1000);  // Buzzer ON by default (1 kHz tone)

        display.setTextSize(3);
        display.setCursor(100, 30);
        display.print("!!");
        display.display();
        delay(1000);
    }
    else {
        noTone(buzzerPin);
        setRGBColor(255, 255, 255);
    }

    delay(1000);
}