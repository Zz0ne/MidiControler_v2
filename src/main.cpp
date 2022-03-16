#include "Headers_Globals.h"
#include <EEPROM.h>

Bank bank[NUM_BANKS];
byte selectedBank = 0;

void options_menu(struct Bank* cur_bank);
void set_midi_values(struct Bank* cur_bank);
void rename_bank_menu(struct Bank* cur_bank);
void save();
void load();
void clear_eeprom();

void setup() {
  Serial1.begin(9600);
  while (!Serial1);
  /********Set input and output pins********/
  pinMode(ENCODER_A, INPUT);
  pinMode(ENCODER_B, INPUT);
  pinMode(ENCODER_C, INPUT_PULLUP);
  for (byte i = 0; i < NUM_FOOTSWITCH; i++)
    pinMode(FOOTSWITCH[i], INPUT_PULLUP);
  /*****************************************/
  /***********Set bank numbers**************/
  for (byte i = 0; i < NUM_BANKS; i++) {
    bank[i].bankNum = i + 1;
  }
  /*****************************************/
  //clear_eeprom();
  load();
  init_lcd();
  print_main_menu(&bank[selectedBank]);
  //Serial1.println("End of setup");
}


void loop() {
  //Serial1.println("Main loop");

  if (switch_press(&bank[selectedBank])) {
    send_midi_cc(&bank[selectedBank]);
    print_main_menu(&bank[selectedBank]);
  }
  
  switch (encoder_movement())
  {
  case 'r':
    selectedBank < NUM_BANKS - 1 ? selectedBank++ : selectedBank = 0;
    print_main_menu(&bank[selectedBank]);
    break;
  case 'l':
    selectedBank > 0 ? selectedBank-- : selectedBank = NUM_BANKS - 1;
    print_main_menu(&bank[selectedBank]);
    break;
  case 'b':
    options_menu(&bank[selectedBank]);
    break;
  }
}

void options_menu(struct Bank* cur_bank) {
  byte selector = 0;
  print_options_menu(cur_bank, selector);

  while (1) {
    switch (encoder_movement())
    {
    case 'r':
      selector < 3 ? selector++ : selector = 0;
      print_options_menu(cur_bank, selector);
      break;
    case 'l':
      selector > 0 ? selector-- : selector = 3;
      print_options_menu(cur_bank, selector);
      break;
    case 'b':
      
      switch (selector) {
      case 0:
        set_midi_values(cur_bank);
        break;
      case 1:
        rename_bank_menu(cur_bank);
        break;
      case 2:
        save();
        print_options_menu(cur_bank, selector);
        break;
      case 3:
        print_main_menu(cur_bank);
        return;
      }
    }
  }
}

void set_midi_values(struct Bank* cur_bank) {
  byte switch_selected = 0, selector = 0, function_output;
  print_set_midi_values(cur_bank, switch_selected, selector);
  
  while (1) {
    if ((function_output = switch_select()) != NO_SWITCH_PRESSED) {
      switch_selected = function_output;
      print_set_midi_values(cur_bank, switch_selected, selector);
    }
    switch (encoder_movement())
    {
    case 'r':
      if (selector == 0) {
        cur_bank->CCmessage[switch_selected] < 128 ?
          cur_bank->CCmessage[switch_selected]++ : cur_bank->CCmessage[switch_selected] = 1;
        print_set_midi_values(cur_bank, switch_selected, selector);
      }
      else if (selector == 1) {
        cur_bank->MidiValue[switch_selected] < 128 ?
          cur_bank->MidiValue[switch_selected]++ : cur_bank->MidiValue[switch_selected] = 0;
        print_set_midi_values(cur_bank, switch_selected, selector);
      }
      else if (selector == 2) {
        cur_bank->Channel[switch_selected] < 10 ?
          cur_bank->Channel[switch_selected]++ : cur_bank->Channel[switch_selected] = 0;
        print_set_midi_values(cur_bank, switch_selected, selector);
      }
      break;
    case 'l':
      if (selector == 0) {
        cur_bank->CCmessage[switch_selected] >= 1 ?
          cur_bank->CCmessage[switch_selected]-- : cur_bank->CCmessage[switch_selected] = 127;
        print_set_midi_values(cur_bank, switch_selected, selector);
      }
      else if (selector == 1) {
        cur_bank->MidiValue[switch_selected] > 0 ?
          cur_bank->MidiValue[switch_selected]-- : cur_bank->MidiValue[switch_selected] = 127;
        print_set_midi_values(cur_bank, switch_selected, selector);
      }
      else if (selector == 2) {
        cur_bank->Channel[switch_selected] > 0 ?
          cur_bank->Channel[switch_selected]-- : cur_bank->Channel[switch_selected] = 10;
        print_set_midi_values(cur_bank, switch_selected, selector);
      }
      break;
    case 'b':
      selector == 2 ? selector = 0 : selector++;
      print_set_midi_values(cur_bank, switch_selected, selector);
      break;
    case 'c':
      print_options_menu(cur_bank, 0);
      return;
    }
  }
}

void rename_bank_menu(struct Bank* cur_bank) {
  char new_name[MAX_BANK_NAME];
  strcpy(new_name, cur_bank->Name);
  byte selected_char = 0;

  print_rename_bank_menu(new_name, selected_char);

  while (1) {
    switch (encoder_movement())
    {
    case 'r':
      new_name[selected_char] < 126 ? new_name[selected_char]++ : new_name[selected_char] = 32;
      print_rename_bank_menu(new_name, selected_char);
      break;
    case 'l':
      new_name[selected_char] > 31 ? new_name[selected_char]-- : new_name[selected_char] = 126;
      print_rename_bank_menu(new_name, selected_char);
      break;
    case 'b':
      selected_char < MAX_BANK_NAME - 2 ? selected_char++ : selected_char = 0;
      print_rename_bank_menu(new_name, selected_char);
      break;
    case 'c':
      strcpy(cur_bank->Name, new_name);
      print_options_menu(cur_bank, 0);
      return;
    default:
      continue;
    }
  }
}

void save() {
  byte selector = 0;

  while (1) {
    print_save_menu(selector);

    switch (encoder_movement())
    {
    case 'r':
      selector < 1 ? selector++ : selector = 0;
      print_save_menu(selector);
      break;
    case 'l':
      selector > 0 ? selector-- : selector = 1;
      print_save_menu(selector);
      break;
    case 'b':
      if (selector)
        EEPROM.put(0, bank);
      return;
    }
  }
}

void load() {
  EEPROM.get(0, bank);
}

void clear_eeprom() {
  for (unsigned int i = 0; i < EEPROM.length(); i++) {
    EEPROM.write(i, 0);
  }
}