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

uint8_t read_tri_state(){
    uint16_t tri_state_in = (uint16_t)pin_read(&A[0]);
    uint8_t tri_state_out;
    // printf("%u\n\r",tri_state_in);
    if (tri_state_in<1000){
        tri_state_out = 1;
    }
    else if(tri_state_in>60000){
        tri_state_out = 2;
    }
    else if (tri_state_in>30000 && tri_state_in<35000){
        tri_state_out = 0;
    }
    return tri_state_out;
}

uint8_t read_slider(uint8_t slider_out){

    uint16_t slider_in = (uint16_t)pin_read(&A[1]);
    if (slider_in < 1000){
        slider_out = 0;
    }
    else if(slider_in < 42000 && slider_in > 37000){
        slider_out = 1;
    }
    else if(slider_in < 52000 && slider_in > 49000 ){
        slider_out = 2;
    }
    else if(slider_in > 60000){
        slider_out = 3;
    }
    return slider_out;
}


void poll_state(_CONSOLE *self) {
    //led_toggle(&led3);
    self->state.s2.red_button = (uint8_t)!pin_read(&D[5]);
    self->state.s2.green_button1 = (uint8_t)pin_read(&D[7]);
    self->state.s2.green_button2 = (uint8_t)pin_read(&D[8]);
    self->state.s2.green_button3 = (uint8_t)pin_read(&D[9]);
    self->state.s2.green_button4 = (uint8_t)pin_read(&D[10]);
    self->state.s2.wormhole1 = (uint8_t)pin_read(&D[12]);
    self->state.s2.wormhole2 = (uint8_t)pin_read(&D[13]);
    self->state.s2.tri_state = read_tri_state();
    self->state.s2.slider = read_slider(self->state.s2.slider);
    self->state.s2.hotsystem1 = (uint8_t)pin_read(&A[2]);
    self->state.s2.hotsystem2 = (uint8_t)pin_read(&A[3]);
    self->state.s2.hotsystem3 = (uint8_t)pin_read(&A[4]);
    self->state.s2.hotsystem4 = (uint8_t)pin_read(&A[5]);  
    //led_write(&led2, self->state.s0.red_button);

    printf("State: %08lx\r\n", (unsigned long)self->state.ul);
}

void handle_CSn(_INT *intx) {
    console.res = spi_read_slave(console.spi);
    //printf("res:%x%x\n\r",res.l[1],res.l[0]);
}

void console1_poll(_TIMER *timer) {
    console_poll_changes(&console);
}

void console2_init(){
    pin_digitalIn(&A[2]);
    pin_digitalIn(&A[3]);
    pin_digitalIn(&A[4]);
    pin_digitalIn(&A[5]);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    timer_initDelay(&timer5);

    init_uart();
    init_console();

    init_i2c();
    init_lcd(2);

    console2_init();

    console_attach_poll(&console, poll_state);
    int_attach(&int1, console.spi->SSn, 1, handle_CSn);

    timer_every(&timer4, 1e-2, console1_poll);

    while(1) {
        // printf("Slave sent: 0x%x\r\n", 0x5A);
        // printf("Slave received: 0x%x\r\n", res);
    }
}
