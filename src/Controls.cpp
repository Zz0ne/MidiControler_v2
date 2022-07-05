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

char menu_navigation() {
    /*
        'u' -> up
        'd' -> down
        'i' -> idle
        's' -> select
        'l' -> long select
    */

    if (!digitalRead(SELECT_UP)) {
        while (!digitalRead(SELECT_UP));
        delay(50);
        return 'u';
    }

    if (!digitalRead(SELECT_DOWN)) {
        while (!digitalRead(SELECT_DOWN));
        delay(50);
        return 'd';
    }

    if (!digitalRead(SELECT_CONFIRM)) {
        int timePressed = 0, time = millis();

        while (!digitalRead(SELECT_CONFIRM)) {
            timePressed = millis();
        }
        timePressed -= time;
        if (timePressed > 600)
            return 'l';
        return 's';
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