#ifndef MENU_PRINT_H
#define MENU_PRINT_H

void init_lcd();
void print_main_menu(struct Bank* bank);
void print_options_menu(struct Bank* bank, byte n);
void print_set_midi_values(struct Bank* bank, byte n, byte selector);
void print_rename_bank_menu(char* new_name, byte selector);
void print_save_menu(byte selector);

#endif