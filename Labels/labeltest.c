#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "i2c.h"
#include "timer.h"
#include "lcd.h" 
#include "strm.h"
#include "labc.h"

// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 ?  E  RW RS

void console_update_lcds(void) {
    level.lab_numb[0] = 30;
    level.lab_numb[1] = 1;
    level.lab_numb[2] = 2;
    level.lab_numb[3] = 3;
    level.lab_numb[4] = 4;
    level.lab_numb[5] = 5;
    level.lab_theme = 1;
    level.mod = 8;
    level.arg_freq = 3;
    level.arg_shift = 2;

    labc_setup();
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
    init_timer();
    timer_initDelayMicro(&timer5);

    init_lcd(3);
    lcd_clear(&lcd[0]);
    lcd_clear(&lcd[1]);
    lcd_clear(&lcd[2]);


    console_update_lcds();
    
    // char string1[17]="Go";
    // char* strptr1=string1;
    // char string2[17]="Space";
    // char* strptr2=string2;
    // char string3[17]="Team";
    // char* strptr3=string3;

    // lcd_print1(&lcd[0],strptr1);
    // lcd_print1(&lcd[1],strptr2);
    // lcd_print1(&lcd[2],strptr3);
    // lcd_print1(&lcd1,strptr2);
    // Brnrd(strptr2,0,0);
    // labc_print(&level);
    //lcd_print1(&lcd2,strptr2);
    // lcd_print2(&lcd1, strptr1,strptr2);
    // strcpy(strptr1,strptr2);
    // lcd_print2(&lcd3, strptr1, strptr2);
    lcd_stop(&lcd[1]);

    while(1) {
        //
    }
}