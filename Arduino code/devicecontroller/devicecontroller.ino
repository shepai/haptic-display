#include "haptic_display.h"

// Initialize the class with your specific pins (e.g., pins 3, 5, and 6)
haptic_display myHaptics(3, 5, 6);

// Create a sample array of 100 integers
int motorValues[100];

void setup() {
  Serial.begin(9600);

  // Fill the array with some dummy data
  for (int i = 0; i < 100; i++) {
    motorValues[i] = i; 
  }

  // Use the library functions
  myHaptics.reset();
  myHaptics.send_to_motors(motorValues);
}

void loop() {
  // Your repeating code
}
