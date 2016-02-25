#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "i2c.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();

    led_on(&led3);

    i2c_open(&i2c3, 1e3);
    i2c_start(&i2c3);
    i2c_putc(&i2c3, 0x4E);
    i2c_idle(&i2c3);
    i2c_putc(&i2c3, 0xF4);
    i2c_idle(&i2c3);
    i2c_putc(&i2c3, 0xF4);
    i2c_idle(&i2c3);
    i2c_putc(&i2c3, 0xF4);
    i2c_stop(&i2c3);
    i2c_close(&i2c3);

    while(1) {}
}
