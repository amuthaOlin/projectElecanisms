#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "int.h"
#include "i2c.h"
#include "uart.h"

void red() {
    led_toggle(&led1);
}

void green() {
    led_toggle(&led2);
}

void blue() {
    led_toggle(&led3);
}

void all() {
    led_on(&led1);
    led_on(&led2);
    led_on(&led3);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_i2c();
    init_uart();

    pin_digitalIn(&D[0]);
    int_attach(&int1, &D[4], 1, red);

    pin_digitalIn(&D[1]);
    int_attach(&int2, &D[6], 1, green);

    pin_digitalIn(&D[2]);
    int_attach(&int3, &D[8], 1, blue);

    pin_digitalIn(&D[3]);
    int_attach(&int4, &D[10], 1, all);

    led_on(&led1);

    while(1) {}
}
