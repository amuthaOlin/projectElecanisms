#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "i2c.h"
#include "LCD.h"
#include "timer.h"

// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 ?  E  RW RS

int16_t main(void) {
    led_on(&led1);
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
    init_timer();
    led_off(&led1);
    init_delay();
    // led_on(&led3);


    init_lcd(&i2c3, 1e3);
    led_on(&led2);
    lcd_write(&i2c3, 'S');
    lcd_write(&i2c3, 'P');
    lcd_write(&i2c3, 'A');
    lcd_write(&i2c3, 'C');
    lcd_write(&i2c3, 'E');
    lcd_write(&i2c3, ' ');
    lcd_write(&i2c3, 'T');
    lcd_write(&i2c3, 'E');
    lcd_write(&i2c3, 'A');
    lcd_write(&i2c3, 'M');
    lcd_write(&i2c3, '!');
    i2c_stop(&i2c3);

    while(1) {
        delayMicroseconds(500);
        led_toggle(&led2);


    }
}
