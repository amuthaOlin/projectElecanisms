#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "uart.h"
#include "int.h"
#include "leds.h"
#include "i2c.h"

#define COLOR_TEST 0,140,255

int16_t bar1_ms = 1e2;
int16_t bar2_ms = 2e2;
int16_t bar3_ms = 3e2;
int16_t barc_ms = 8e2;

volatile float fill = 1;
volatile float space = 0.5;
volatile float fire = 0.25;
void led_loop(_TIMER *timer) {
    fill -= 0.01;
    leds_centerDisplay(&ledcenter, fire, space);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_timer();
    timer_initDelayMicro(&timer5);

    init_uart();
    init_oc();
    init_spi();
    init_rng();
    init_leds();

    led_on(&led3);

    timer_every(&timer1, 1e-2, led_loop);

    while(1) {
        leds_bar(&ledbar1, fill, 0.5);
        leds_bar(&ledbar2, fill, 0.5);
        leds_bar(&ledbar3, fill, 0.5);
    }
}