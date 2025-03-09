#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>

const int forceSensorPin = A0;  // Force sensor connected to A0
const int servoPin = 7;         // Servo motor connected to pin 9

Servo myServo;

// Function to send force sensor data to master
void requestEvent() {
    int forceValue = analogRead(forceSensorPin);
    Wire.write((forceValue >> 8) & 0xFF);  // Send high byte
    Wire.write(forceValue & 0xFF);         // Send low byte
}

void setup() {
    Wire.begin(8);              // Initialize I2C communication with address 8
    Serial.begin(9600); 
    Wire.onRequest(requestEvent); // Register event for data request
    pinMode(forceSensorPin, INPUT);
    myServo.attach(servoPin);
}

void loop() {
    int forceValue = analogRead(forceSensorPin);  // Read force sensor

    // Control servo motor based on force sensor value
    int servoAngle = map(forceValue, 0, 1023, 0, 180);  // Map force value to servo angle
    myServo.write(servoAngle);

    delay(100);  // Small delay for stability
}