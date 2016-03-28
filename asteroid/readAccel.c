#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    pin_digitalIn(&D[0]);

    uint16_t shake=0;



    while(1){
    shake=pin_read(&D[0]);

    if (shake==1){
        led_on(&led1);
    }
    else {
        led_off(&led1);
    }
    }
}