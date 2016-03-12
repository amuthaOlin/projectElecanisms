#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "leds.h"
#include "i2c.h"

volatile float fill = 1;
void updateBar(_TIMER *timer) {
    leds_bar(&leds, fill, 1);
    fill -= .01;
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_timer();
    init_oc();
    init_leds();

    led_on(&led3);

    timer_every(&timer1, .05, updateBar);

    while(1) {}
}
