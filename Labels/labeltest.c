#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "i2c.h"
#include "timer.h"
#include "lcd.h"
#include "../lcdtest/strmanip.h"
#include "labels_console.h"


int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
    init_timer();
    timer_initDelayMicro(&timer5);

    init_lcd();


    char string1[17]="Go";
    char* strptr1=string1;
    char string2[17]="engage starship";
    char* strptr2=string2;
    char string3[17]="Team";
    char* strptr3=string3;
    lcd_print1(&lcd1,strptr2);
    Reverse(strptr2,0,0);
    lcd_print1(&lcd3,strptr2);
    // lcd_print2(&lcd1, strptr1,strptr2);
    // strcpy(strptr1,strptr2);
    // lcd_print2(&lcd3, strptr1, strptr2);
    lcd_stop(&lcd3);

    while(1) {
        //
    }
}
