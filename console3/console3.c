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
#include "spacecomms.h"
#include "console.h"

void poll_state(_CONSOLE *self) {
    //led_toggle(&led3);
    self->state.s3.red_button = (uint8_t)pin_read(&D[5]);
    self->state.s3.hotsystem1 = (uint8_t)pin_read(&D[6]);
    self->state.s3.hotsystem2 = (uint8_t)pin_read(&D[7]);
    self->state.s3.triangle1 = (uint8_t)pin_read(&D[8]);
    self->state.s3.triangle2 = (uint8_t)pin_read(&D[9]);
    self->state.s3.triangle3 = (uint8_t)pin_read(&D[10]);
    self->state.s3.pin_11 = (uint8_t)pin_read(&D[11]);
    self->state.s3.wormhole1 = (uint8_t)pin_read(&D[12]);
    self->state.s3.wormhole2 = (uint8_t)pin_read(&D[13]);
    //led_write(&led2, self->state.s0.red_button);
}

void handle_CSn(_INT *intx) {
    console.res = spi_read_slave(console.spi);
    //printf("res:%x%x\n\r",res.l[1],res.l[0]);
}

void console1_poll(_TIMER *timer) {
    console_poll_changes(&console);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    init_uart();
    init_console();

    console_attach_poll(&console, poll_state);
    int_attach(&int1, console.spi->SSn, 1, handle_CSn);

    timer_every(&timer4, 1e-2, console1_poll);

    while(1) {
        // printf("Slave sent: 0x%x\r\n", 0x5A);
        // printf("Slave received: 0x%x\r\n", res);
    }
}