#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "i2c.h"
#include "timer.h"
#include "lcd.h"
#include "strmanip.h"

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

    char string[17]="space team";
    char* strptr=string;
    CamelCase(strptr);
    Caesar(strptr,1,2);
    lcd_print(&lcd1, strptr);
    lcd_stop(&lcd1);

    while(1) {
        //
    }
}
