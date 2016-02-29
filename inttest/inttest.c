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

void blue() {
    led_toggle(&led3);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_i2c();
    init_uart();

    INTCON2 = 0x0000; // defaults, all externals rising edge

    pin_digitalIn(&D[0]);
    int_attach(&int1, &D[0], blue);

    led_on(&led1);

    while(1) {}
}
