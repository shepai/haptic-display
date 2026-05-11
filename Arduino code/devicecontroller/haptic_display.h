#ifndef HAPTIC_DISPLAY_H
#define HAPTIC_DISPLAY_H

#include <Arduino.h>

class haptic_display {
  private:
    int _p1, _p2, _p3;

  public:
    // Constructor to set the pins
    haptic_display(int pin1, int pin2, int pin3) {
        _p1 = pin1;
        _p2 = pin2;
        _p3 = pin3;
    }

    // Empty function for reset
    void reset() {
        //Reset the rig till all the buttons are in
    }

    // Function taking an int array of size 100
    void send_to_motors(int data[100]) {
        // Code to process the array goes here
    }

    
};

#endif
