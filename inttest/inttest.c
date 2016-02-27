#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "i2c.h"

// void callback() {
//     led_toggle(&led2);
// }

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();

    led_on(&led3);

    INTCON2 = 0x0000; // defaults, all externals rising edge
    IEC0 = 0x0001; // enable interrupt 0
    IFS0 = 0x0001; // fake interupt 0

    led_on(&led1);

    while(1) {}
}
