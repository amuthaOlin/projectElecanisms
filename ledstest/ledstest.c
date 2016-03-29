#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "leds.h"
#include "i2c.h"

volatile float fill1 = 1;
void update_bar1() {
    leds_bar(&ledbar1, fill1, 0.1);
    fill1 -= .01;
}

volatile float fill2 = 1;
void update_bar2() {
    leds_bar(&ledbar2, fill2, 0.1);
    fill2 -= .02;
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_timer();
    init_oc();
    init_leds();

    led_on(&led3);

    timer_every(&timer1, 0.05, update_bar1);
    timer_every(&timer2, 0.05, update_bar2);
    leds_writeRGB(&ledbar3, 4, 60,0,80);
    leds_writeRGB(&ledcenter, 0, 60,0,80);
    leds_writeRGB(&ledcenter, 1, 60,0,80);
    leds_writeRGB(&ledcenter, 2, 60,0,80);
    leds_writeRGB(&ledcenter, 3, 60,0,80);
    leds_writeRGB(&ledcenter, 4, 60,0,80);

    while(1) {}
}
