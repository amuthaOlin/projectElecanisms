#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "spi.h"
#include "timer.h"
#include "uart.h"

void red() {
    led_toggle(&led1);
}

void green() {
    led_toggle(&led2);
}

void blue() {
    led_toggle(&led3);
}

uint8_t res = 0;

void handle_sint(_INT *intx) {
    res = spi_transfer(&spi1, 0xF0);
    printf("Master sent: 0x%x\r\n", 0xF0);
    printf("Master received: 0x%x\r\n", res);
}

_PIN *CSn = &D[3];
_PIN *Sint = &D[4];

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    init_uart();

    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1);

    pin_digitalIn(Sint);

    pin_digitalOut(CSn);
    pin_clear(CSn);

    int_attach(&int1, CSn, 0, handle_sint);

    while(1) {}
}
