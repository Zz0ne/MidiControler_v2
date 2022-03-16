#include "Headers_Globals.h"

byte switch_select() {
    for (byte i = 0; i < NUM_FOOTSWITCH; i++) {
        if (!digitalRead(FOOTSWITCH[i])) {
            // prevents the footswitch state from changing if the user is holding the switch
            while (!digitalRead(FOOTSWITCH[i])); 
            // Debouncing delay
            delay(50);
            return i;
        }
    }
    return NO_SWITCH_PRESSED;
}

char encoder_movement() {
    /*
        'r' -> right
        'l' -> left
        'i' -> idle
        'b' -> button press
        'c' -> long press
    */

    bool aState = digitalRead(ENCODER_A);
    if (!aState) {
        if (aState != digitalRead(ENCODER_B)) {
            //Serial.println('r');
            delay(50);
            return 'r';
        }
        else {
            //Serial.println('l');
            delay(50);
            return 'l';
        }
    }
    if (!digitalRead(ENCODER_C)) {
        int timePressed = 0, time = millis();

        while (!digitalRead(ENCODER_C)) {
            timePressed = millis();
        }
        timePressed -= time;
        if (timePressed > 600)
            return 'c';
        return 'b';
    }
    return 'i';
}

bool switch_press(struct Bank* bank) {
    bool switches_pressed[NUM_FOOTSWITCH] = { false };
    bool confirm_press = false;
    byte still_pressed = 0;
    
    for (byte i = 0; i < NUM_FOOTSWITCH; i++) {
        switches_pressed[i] = !digitalRead(FOOTSWITCH[i]);
    }
    for (byte i = 0; i < NUM_FOOTSWITCH; i++) {
        if (switches_pressed[i]) {
            bank->toggle[i] = !bank->toggle[i];
            confirm_press = true;
            still_pressed = i;
        }
    }

    // prevents the footswitch state from changing if the user is holding the switch
    while (!digitalRead(FOOTSWITCH[still_pressed]));
    // Debouncing delay
    delay(50);
    
    return confirm_press;
}