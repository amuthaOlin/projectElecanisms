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

int16_t main(void) {
    led_on(&led1);
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
    init_timer();
    led_off(&led1);
    // led_on(&led3);


    init_lcd(&i2c3, 1e3);
    led_on(&led2);
    // lcd_send(&i2c3,0xE,INTERNAL_WRITE);
    // lcd_send(&i2c3,0x6,INTERNAL_WRITE);
    // lcd_send(&i2c3,0x45,DR_WRITE);
    // lcd_write(&i2c3, 0x45);
    led_on(&led1);
    // i2c_stop(&i2c3);

    // led_toggle(&led1);

    while(1) {
        lcd_write(&i2c3, 0x45);
    }
}
