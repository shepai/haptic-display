#ifndef HAPTIC_DISPLAY_H
#define HAPTIC_DISPLAY_H

#include <Arduino.h>
struct Image { //image expeced as 0-255
    int state[10][10];
};
class haptic_display {
  private:
    int _motorData;
    int _motorClock;
    int _motorLatch;

    uint8_t rowPins[10] = {13,12,11,10,9,8,7,6,5,4};
    uint8_t colPins[10] = {3,2,14,15,16,17,18,19,20,21};
  public:
    struct ButtonMatrix {
            bool state[10][10];
        };
    enum MotorDirection {
        OFF = 0,
        FORWARD = 1,
        REVERSE = 2,
        BRAKE = 3
    };
    void update_shift_registers() {
        digitalWrite(_latchPin, LOW);

        // furthest chip first
        for (int i = 24; i >= 0; i--) {
            shiftOut(_dataPin, _clockPin, MSBFIRST, _motorState[i]);
        }

        digitalWrite(_latchPin, HIGH);
    }
    // Constructor to set the pins
    haptic_display() {
        _motorData  = 45;
        _motorClock = 42;
        _motorLatch = 43;
        pinMode(_motorData, OUTPUT);
        pinMode(_motorClock, OUTPUT);
        pinMode(_motorLatch, OUTPUT);
        for (int i = 0; i < 10; i++) {
            _rowPins[i] = rowPins[i];
            _colPins[i] = colPins[i];
        }

        // Configure matrix pins
        for (int i = 0; i < 10; i++) {
            pinMode(_rowPins[i], OUTPUT);
            digitalWrite(_rowPins[i], HIGH);   // inactive row

            pinMode(_colPins[i], INPUT_PULLUP);
        }

        // Clear button matrix
        memset(_buttons, false, sizeof(_buttons));
        update_shift_registers();
    }
    void set_motor(uint8_t motor, MotorDirection dir) {
            if (motor >= 100) return;

            // 4 motors per chip
            uint8_t chip = motor / 4;

            // 2 bits per motor
            uint8_t motorInChip = motor % 4;
            uint8_t bitPos = motorInChip * 2;

            // clear previous 2 bits
            _motorState[chip] &= ~(0b11 << bitPos);

            // set new state
            _motorState[chip] |= ((uint8_t)dir << bitPos);

            update_shift_registers();
        }
    MotorDirection get_motor_state(uint8_t motor) {
        if (motor >= 100) return OFF;

        uint8_t chip = motor / 4;
        uint8_t motorInChip = motor % 4;
        uint8_t bitPos = motorInChip * 2;

        uint8_t val = (_motorState[chip] >> bitPos) & 0b11;
        return (MotorDirection)val;
    }
    void all_motors_off() {
        memset(_motorState, 0, sizeof(_motorState));
        update_shift_registers();
    }
    void brake_all() {
        for (int i = 0; i < 25; i++) {
            _motorState[i] = 0xFF; // all 2-bit pairs = 11
        }
        update_shift_registers();
    }
    ButtonMatrix check_buttons() {
        ButtonMatrix matrix;

        for (int row = 0; row < 10; row++) {

            // Activate current row
            digitalWrite(_rowPins[row], LOW);

            // Read all columns
            for (int col = 0; col < 10; col++) {
                _buttons[row][col] = !digitalRead(_colPins[col]);
                matrix.state[row][col] = _buttons[row][col];
            }

            // Deactivate row
            digitalWrite(_rowPins[row], HIGH);
        }

        return matrix;
    }
    // Empty function for reset
    void reset() {
        //Reset the rig till all the buttons are in
        bool finished=false;
        ButtonMatrix button;
        while(!finished) {
            buttons = check_buttons();
            finished=true;
            for(int i=0;i<10;i++) { //loop through all buttons and set motor
                for(int j=0;j<10;j++) {
                    int count = i * 10 + j;
                    if(buttons[i][j]==0) {
                        set_motor(count,haptic_display::REVERSE);
                        finished=false;
                    }else{
                        set_motor(count,haptic_display::OFF);
                    }
                }
            }
        }
    }
    void setImage(Image image) {
        for(int i=0;i<10;i++) { //loop through all buttons and set motor
            for(int j=0;j<10;j++) {
                int count = i * 10 + j;
                int time = image[i][j];
                set_motor(count,haptic_display::FOWARD);
                delay(time);
                set_motor(count,haptic_display::OFF);
                }
            }
    }

};

#endif
