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

_PIN *CSn = &D[3];
_PIN *Sint = &D[4];

uint8_t res = 0;

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    init_uart();

    spi_open_slave(&spi1, &D[0], &D[1], &D[2], 1);

    pin_digitalOut(Sint);
    pin_clear(Sint);

    pin_digitalIn(CSn);

    timer_setPeriod(&timer1, 0.5);
    timer_start(&timer1);

    while(1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);

            *(spi1.SPIxBUF) = 0x005A; // set Tx data
            pin_set(Sint); // pulse Sint to request transaction
            pin_clear(Sint); // trigger on falling edge
            while (bitread(spi1.SPIxSTAT, 0)==0) {} // wait for master to transact
            res = (uint8_t)(*(spi1.SPIxBUF)); // read result from buffer

            led_toggle(&led3);

            printf("Slave sent: 0x%x\r\n", 0x5A);
            printf("Slave received: 0x%x\r\n", res);
        }
    }
}
