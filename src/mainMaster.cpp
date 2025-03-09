#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin setup
const int tempSensorPin = A0;   // TMP36 sensor connected to A1
const int buzzerPin = 13;       // Piezo buzzer connected to digital pin 13

// Accelerometer setup (ADXL326 connected to A5, A4, A3)
int accelXPin = A1;  // X-axis
int accelYPin = A2;  // Y-axis
int accelZPin = A3;  // Z-axis

//A4, A5 to SCL and SDA for OLED Display

// RGB LED Pins
const int redPin = 7;
const int greenPin = 6;
const int bluePin = 5;

const int threshold = 100;      // Force sensor activation threshold
int heartRate = 0;              // Variable to store simulated heart rate

void setup() {
    Wire.begin();               // Initialize I2C communication as master
    Serial.begin(9600);         // Start serial communication

    pinMode(buzzerPin, OUTPUT);
    pinMode(redPin, OUTPUT);
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);

    noTone(buzzerPin); // Ensure buzzer starts OFF

    // SPI for Display
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
    }
    delay(2000);

    display.clearDisplay(); //clear buffer
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    // Display static text
    display.println("Hello");
    display.display(); 
}

// Function to run display
void displayBPM(int heartRate) {
    display.clearDisplay();
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    // Display static text
    display.print("BPM: ");
    display.println(heartRate);
    display.display(); 
}

// Function to set RGB LED color
void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

// Function to blink Yellow (Red + Green)
void blinkYellow() {
  for (int i = 0; i < 3; i++) {
      setColor(255, 255, 0); // 🟡 Yellow
      delay(300);
      setColor(0, 0, 0); // Turn OFF
      delay(300);
  }
}

void loop() {
    // Request force sensor data from slave Arduino
    Wire.requestFrom(8, 2); // Request 2 bytes from slave with address 8
    int forceValue = 0;
    if (Wire.available()) {
        forceValue = Wire.read() << 8 | Wire.read(); // Combine two bytes into an integer
    }

    int tempValue = analogRead(tempSensorPin);    // Read temperature sensor
    setColor(0, 0, 255);
    // Convert TMP36 reading to temperature (Celsius)
    float voltage = tempValue * (5.0 / 1023.0);
    float temperatureC = (voltage - 0.5) * 100.0;
    float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;

    // Read accelerometer data from ADXL326
    int accelX = analogRead(accelXPin);  // X-axis data
    int accelY = analogRead(accelYPin);  // Y-axis data
    int accelZ = analogRead(accelZPin);  // Z-axis data

    // Here, we focus on the Z-axis (upward and downward chest movement)
    int accelValue = accelZ;  // Z-axis data

    // Calculate heart rate (simulated based on accelerometer data)
    // Example: detect peaks in the accelerometer data and map to heart rate
    heartRate = map(accelValue, 0, 1023, 50, 140);  // Adjust mapping as necessary

    // Determine LED color based on heart rate
    if (heartRate > 100) {  // High heart rate
        setColor(255, 0, 0);  // 🔴 Red
    } else if (heartRate >= 60 && heartRate <= 100) {  // Normal range
        setColor(0, 255, 0);  // 🟢 Green
    } else if (heartRate < 60) {  // Low heart rate
        setColor(0, 0, 255);  // 🔵 Blue
    }

    // If no force detected (no heartbeat), trigger alarm & blink yellow LED
    if (forceValue < threshold) {
        display.clearDisplay();
        Serial.println("NO HEARTBEAT DETECTED!");
        tone(buzzerPin, 1000); // Turn on buzzer
        blinkYellow();  // Blink yellow LED
    } else {
        noTone(buzzerPin);
        displayBPM(heartRate);
    }

    // Print heart rate, temperature, and accelerometer data
    Serial.print("Heart Rate: ");
    Serial.print(heartRate);
    Serial.println(" BPM");

    // Serial.print("Temperature: ");
    // Serial.print(temperatureC);
    // Serial.print(" °C / ");
    Serial.print(temperatureF);
    Serial.println(" °F");

    Serial.print("Accelerometer X: ");
    Serial.print(accelX);
    Serial.print(" Y: ");
    Serial.print(accelY);
    Serial.print(" Z: ");
    Serial.println(accelZ);

    delay(500);
}


