#include "haptic_display.h"

// Initialize the class with your specific pins (e.g., pins 3, 5, and 6)
haptic_display myHaptics(3, 5, 6);

// Create a sample array of 100 integers
int motorValues[100];
Image img = {
    {
        {0,0,1,1,1,1,0,0,0,0},
        {0,1,0,0,0,0,1,0,0,0},
        {1,0,0,0,0,0,0,1,0,0},
        {1,0,1,0,0,1,0,1,0,0},
        {1,0,0,0,0,0,0,1,0,0},
        {1,0,1,1,1,1,0,1,0,0},
        {0,1,0,0,0,0,1,0,0,0},
        {0,0,1,1,1,1,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0}
    }
};
void setup() {
  Serial.begin(9600);
  // Use the library functions
  myHaptics.reset();
 
}

void loop() {
  // Your repeating code
  myHaptics.setImage(img);
  delay(5000);
  myHaptics.reset()
}
