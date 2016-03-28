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
    init_delay();
    led_on(&led3);

    init_lcd();
    led_on(&led2);

    lcd_write(&lcd1, 'S');
    lcd_write(&lcd1, 'P');
    lcd_write(&lcd1, 'A');
    lcd_write(&lcd1, 'C');
    lcd_write(&lcd1, 'E');
    lcd_write(&lcd1, ' ');
    lcd_write(&lcd1, 'T');
    lcd_write(&lcd1, 'E');
    lcd_write(&lcd1, 'A');
    lcd_write(&lcd1, 'M');
    lcd_write(&lcd1, '!');
    i2c_stop(lcd1.i2c);

    while(1) {
        delayMicroseconds(500);
        led_toggle(&led2);
    }
}
