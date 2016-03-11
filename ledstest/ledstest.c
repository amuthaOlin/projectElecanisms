#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "i2c.h"

void red() {
    led_toggle(&led1);
}

void green() {
    led_toggle(&led2);
}

void blue() {
    led_toggle(&led3);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_timer();
    init_oc();
    init_leds();

    led_on(&led3);

    while(1) {}
}
