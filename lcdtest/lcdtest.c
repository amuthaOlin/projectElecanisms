#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "i2c.h"
#include "timer.h"
#include "lcd.h"

// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 ?  E  RW RS

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
    init_timer();
    timer_initDelayMicro(&timer5);

    led_on(&led3);

    init_lcd();
    led_on(&led2);

    lcd_print(&lcd1, "SPACE TEAM!");
    lcd_stop(&lcd1);

    while(1) {
        //
    }
}
