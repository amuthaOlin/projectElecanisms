#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "leds.h"
#include "i2c.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_timer();
    init_oc();
    init_leds();

    led_on(&led3);

    leds_bar(&ledbar1, 0.5, 1);
    leds_bar(&ledbar2, 0.75, 1);

    while(1) {}
}
