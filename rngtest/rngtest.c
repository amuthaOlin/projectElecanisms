#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "leds.h"
#include "ui.h"
#include "cd.h"
#include "cmd.h"
#include "int.h"
#include "timer.h"

int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_timer();
    init_ui();
    init_pin();
    init_oc();
    init_int();
    init_leds();
    init_cd();
    init_cmd();
    init_rng();

    led_on(&led3);

    printf("Random int: %d\r\n", rng_int());
    printf("Random int: %d\r\n", rng_int());

    while (1) {}
}