#ifndef HEAD_GLOB
#define HEAD_GLOB

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "Controls.h"
#include "Menu_Print.h"
#include "Midi_Features.h"

#define SELECT_UP         14
#define SELECT_DOWN       15
#define SELECT_CONFIRM    A0
#define NO_SWITCH_PRESSED 9
#define NUM_BANKS         5
#define NUM_FOOTSWITCH    8
#define MAX_BANK_NAME     14
const byte FOOTSWITCH[NUM_FOOTSWITCH] = { 4, 5, 6, 7, 8, 9, 10, 16 };

struct Bank {
    char Name[MAX_BANK_NAME] = "Bank         ";
    bool toggle[NUM_FOOTSWITCH] = { 0, 0, 0, 0, 0, 0, 0, 0 };                 // this variable keeps track of the state the switch (on/off)
    const char* switches[2][NUM_FOOTSWITCH] = {
              { " A ", " B ", " C ", " D ", " E ", " F ", " G ", " H " },     // how the footswitches are printed in the LCD
              { ">A<", ">B<", ">C<", ">D<", ">E<", ">F<", ">G<", ">H<" } };   // example: " A " toggle off, ">A<" toggle on
    byte CCmessage[NUM_FOOTSWITCH] = { 40, 41, 42, 43, 44, 45, 46, 47 };
    byte MidiValue[NUM_FOOTSWITCH] = { 127, 127, 127, 127, 127, 127, 127, 127 };
    byte Channel[NUM_FOOTSWITCH] = { 0 };
    byte bankNum;
};

#endif