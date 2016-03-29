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
void update_bar1(_TIMER *timer) {
    leds_bar(&ledbar1, fill1, 0.9);
    if (fill1 > 0.1) {
        fill1 -= .01;
    } else {
        timer_cancel(&timer1);
    }
}

volatile float fill2 = 1;
void update_bar2() {
    leds_bar(&ledbar2, fill2, 0.1);
    if (fill2 > 0.1)
        fill2 -= .02;
}

#define COLOR_TEST 0,140,255

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
    leds_writeRGB(&ledbar3, 0, COLOR_TEST);
    leds_writeRGB(&ledbar3, 2, COLOR_TEST);
    leds_writeRGB(&ledbar3, 4, COLOR_TEST);
    leds_writeRGB(&ledbar3, 6, COLOR_TEST);
    leds_writeRGB(&ledcenter, 0, COLOR_TEST);
    leds_writeRGB(&ledcenter, 9, COLOR_TEST);
    leds_writeRGB(&ledcenter, 19, COLOR_TEST);
    leds_writeRGB(&ledcenter, 29, COLOR_TEST);
    leds_writeRGB(&ledcenter, 39, COLOR_TEST);
    leds_writeRGB(&ledcenter, 49, COLOR_TEST);
    leds_writeRGB(&ledcenter, 59, COLOR_TEST);

    while(1) {}
}
