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

volatile int16_t count = 0;
void game_loop(_TIMER *timer) {
    count = count+1;
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_timer();
    init_uart();
    init_oc();
    init_leds();

    led_on(&led3);

    timer_every(&timer1, 1e-2, game_loop);

    while(1) {
        leds_bar(&ledbar1, (bar1_ms-count)/(float)bar1_ms, 0.5);
        leds_bar(&ledbar2, (bar2_ms-count)/(float)bar2_ms, 0.5);
        leds_bar(&ledbar3, (bar3_ms-count)/(float)bar3_ms, 0.5);
        leds_bar(&ledcenter, (barc_ms-count)/(float)barc_ms, 0.5);
    }
}