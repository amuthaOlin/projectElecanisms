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

uint8_t read_joystick(){
    uint8_t j1 = pin_read(&D[8]);
    uint8_t j3 = pin_read(&D[9]);
    uint8_t j5 = pin_read(&D[10]);
    uint8_t j7 = pin_read(&A[5]); // pin 11 isn't working

    return j1 + (j3<<1) + (j5<<2) + (j7<<3);
}


void poll_state(_CONSOLE *self) {
    //led_toggle(&led3);
    self->state.s1.red_button = (uint8_t)pin_read(&D[5]);
    if (self->state.s1.red_button)
        led_on(&led3);
    else
        led_off(&led3);
    self->state.s1.toggle1 = (uint8_t)pin_read(&D[6]);
    self->state.s1.toggle2 = (uint8_t)pin_read(&D[7]);
    self->state.s1.joystick = read_joystick();
    self->state.s1.wormhole1 = (uint8_t)pin_read(&D[12]);
    self->state.s1.wormhole2 = (uint8_t)pin_read(&D[13]);   
    self->state.s1.hotsystem = (uint8_t)pin_read(&A[0]);
    //led_write(&led2, self->state.s0.red_button);
}

void handle_CSn(_INT *intx) {
    console.res = spi_read_slave(console.spi);
    //printf("res:%x%x\n\r",res.l[1],res.l[0]);
}

void console1_poll(_TIMER *timer) {
    console_poll_changes(&console);
}

void console1_init(){
    pin_digitalIn(&A[0]);
    pin_digitalIn(&A[5]);
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

    console1_init();
    console_attach_poll(&console, poll_state);
    int_attach(&int1, console.spi->SSn, 1, handle_CSn);

    timer_every(&timer4, 1e-2, console1_poll);

    while(1) {
        // printf("Slave sent: 0x%x\r\n", 0x5A);
        // printf("Slave received: 0x%x\r\n", res);
    }
}
