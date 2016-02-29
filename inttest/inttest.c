#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "int.h"
#include "i2c.h"
#include "uart.h"

void __attribute__((interrupt,auto_psv)) _INT1Interrupt() {
    IFS1 = 0x0000; // clear interrupt 16+4 = 20 (INT1)
    led_toggle(&led3);
}

void blue() {
    led_toggle(&led3);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_i2c();
    init_uart();

    INTCON2 = 0x0000; // defaults, all externals rising edge

    pin_digitalIn(&D[0]);
    int_attach(&int1, &D[0], blue);

    asm volatile (
       "mov #0x742,w1 \n"
       "mov #0x46,w2 \n"
       "mov #0x57,w3 \n"
       "mov.b w2,[w1] \n"
       "mov.b w3,[w1] \n"
       "bclr 0x742,#6 \n"
    );
    // uint16_t tmp = RPINR0;
    RPINR0 = D[0].rpnum<<8; // Map pin 0 (RP20) to EXT1
    asm volatile (
       "mov #0x742,w1 \n"
       "mov #0x46,w2 \n"
       "mov #0x57,w3 \n"
       "mov.b w2,[w1] \n"
       "mov.b w3,[w1] \n"
       "bset 0x742,#6 \n"
    );

    led_on(&led1);

    while(1) {}
}
