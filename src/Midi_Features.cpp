#include "Headers_Globals.h"

static void controlChange(byte channel, byte control, byte value) {
    midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
    MidiUSB.sendMIDI(event);
}

void send_midi_cc(struct Bank* cur_bank) {
    static byte prev_switch_state[NUM_FOOTSWITCH] = { 0 };
    
    for (byte i = 0; i < NUM_FOOTSWITCH; i++) {
        if (prev_switch_state[i] != cur_bank->toggle[i]) {
            if (cur_bank->toggle[i]) {
                controlChange(cur_bank->Channel[i], cur_bank->CCmessage[i], cur_bank->MidiValue[i]);
                MidiUSB.flush();
            }
            else {
                controlChange(cur_bank->Channel[i], cur_bank->CCmessage[i], 0);
                MidiUSB.flush();
            }
            prev_switch_state[i] = cur_bank->toggle[i];
        }
    }
}