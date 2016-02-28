#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "i2c.h"

void __attribute__((interrupt,auto_psv)) _INT1Interrupt() {
    IFS1 = 0x0000; // clear interrupt 16+4 = 20 (INT1)
    led_toggle(&led3);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();

    INTCON2 = 0x0000; // defaults, all externals rising edge
    IEC1 = 0x0010; // enable interrupt 16+4 = 20 (INT1)

    pin_digitalIn(&D[0]);
    RPINR0 = 0x1400; // Map pin 0 (RP20) to EXT1?

    led_on(&led1);

    while(1) {}
}
