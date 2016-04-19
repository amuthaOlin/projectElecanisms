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

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
    init_timer();
    timer_initDelayMicro(&timer5);

    init_lcd();
    lcd_clear(&lcd1);
    lcd_clear(&lcd2);
    lcd_clear(&lcd3);


    _LABC level;

    labc_recieve(&level,0);
    labc_setup(&level);
    char string1[17]="Go";
    char* strptr1=string1;
    char string2[17]="Space";
    char* strptr2=string2;
    char string3[17]="Team";
    char* strptr3=string3;
    // lcd_print1(&lcd1,strptr2);
    // Brnrd(strptr2,0,0);
    labc_print(&level);
    //lcd_print1(&lcd2,strptr2);
    // lcd_print2(&lcd1, strptr1,strptr2);
    // strcpy(strptr1,strptr2);
    // lcd_print2(&lcd3, strptr1, strptr2);
    lcd_stop(&lcd1);

    while(1) {
        //
    }
}