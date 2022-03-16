#include "Headers_Globals.h"

/* The print functions only create a 80 character string based
    on the current data in the Bank struct using sprintf. The string
    is actualy printed to the lcd in the function lcd_update() */

/******LCD macros and object declaration******/
#define ADDRESS         0x27 
#define COLUMNS         20
#define LINES           4
#define LCD_MAX_CH      (COLUMNS * LINES)
LiquidCrystal_I2C lcd(ADDRESS, COLUMNS, LINES);
/*********************************************/

static void lcd_update(char str[LCD_MAX_CH + 1]);

void init_lcd() {
    lcd.init();
    lcd.backlight();
}

void print_main_menu(struct Bank* bank) {

    /*the purpose of these variables is to have a smaller name to be used in the sprintf */
    char switches[NUM_FOOTSWITCH][4];
    for (byte i = 0; i < NUM_FOOTSWITCH; i++) {
        strcpy(switches[i], bank->switches[bank->toggle[i]][i]);
    }
    /*************************************************************************************/
    
    char menu_to_print[LCD_MAX_CH + 1];
    snprintf(menu_to_print, LCD_MAX_CH, " %s  %s  %s  %s  Name: %s Num: %d              %s  %s  %s  %s ",
            switches[0], switches[1], switches[2], switches[3],
            bank->Name, bank->bankNum,
            switches[4], switches[5], switches[6], switches[7]
    );

    lcd_update(menu_to_print);
}

void print_options_menu(struct Bank* bank, byte selector) {
    char menu_to_print[LCD_MAX_CH + 1];
    char line_selected[4] = { ' ', ' ', ' ', ' '};
    line_selected[selector] = '>';
    
    snprintf(menu_to_print, LCD_MAX_CH, "%cSet midi values    %cRename Bank        %cSave               %cExit               ",
        line_selected[0], line_selected[1], line_selected[2], line_selected[3]);
    lcd_update(menu_to_print);
}

void print_set_midi_values(struct Bank* bank, byte n, byte selector) {
    char menu_to_print[LCD_MAX_CH + 1];
    char line_selected[3] = { ' ', ' ', ' ' };
    line_selected[selector] = '>';

    snprintf(menu_to_print, LCD_MAX_CH, " Switch:%s         %cControl Change: %3d%cValue: %3d         %cChannel: %2d         ",
        bank->switches[0][n], line_selected[0], bank->CCmessage[n], line_selected[1],
        bank->MidiValue[n], line_selected[2], bank->Channel[n]);
    lcd_update(menu_to_print);
}

void print_rename_bank_menu(char* new_name, byte selector) {
    char menu_to_print[LCD_MAX_CH + 1];
    char char_selected[MAX_BANK_NAME] = "             ";
    char_selected[selector] = '^';
    snprintf(menu_to_print, LCD_MAX_CH, "New Name:              %s       %s                      ", new_name, char_selected);
    lcd_update(menu_to_print);
}

void print_save_menu(byte selector) {
    char menu_to_print[LCD_MAX_CH + 1];
    char option[2] = { ' ', ' ' };
    option[selector] = '>';
    snprintf(menu_to_print, LCD_MAX_CH, "Are you sure you    want to overide the curent settings?        %cNo      %cYes   ", option[0], option[1]);
    lcd_update(menu_to_print);
}

static void lcd_update(char str[LCD_MAX_CH + 1]) {
    /* This function is used to print to the lcd only the changes made
       since the last print. */

    // prev_str is the string printed the last time this function was called
    static char prev_str[LCD_MAX_CH + 1] = { ' ' };
    byte line = 0, col = 0;
    lcd.setCursor(col, line);

    /* this section compares each character of the new string with the one printed before,
       and moves the cursor's position along with the characters being compared in the
       lcd. */
    for (byte i = 0; str[i] != '\0'; i++) {
        if (col == COLUMNS) {
            col = 0;
            line++;
            if (line == LINES) {
                col = 0;
                line = 0;
            }
        }
        
        if (prev_str[i] != str[i]) {
            lcd.setCursor(col, line);
            lcd.write(str[i]);
        }
        col++;
    }
    // update the prev_str
    strcpy(prev_str, str);
}