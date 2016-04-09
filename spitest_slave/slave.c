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

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *CSn   = &D[3];
_PIN *Sint  = &D[4];

volatile WORD res, cmd;
volatile WORD state;

void handle_CSn(_INT *intx) {
    res = spi_read_slave(&spi1);
    //printf("res:%x%x\n\r",res.w[1],res.w[0]);
    if (res.w == 0xA1B2) {
        led_toggle(&led2);
    }
}

void check_slider(){
    uint16_t slider = pin_read(&A[0]);
    if(slider<=100){
        state.s.slider = 0;
    }
    else if(36000<=slider && slider <=42000){
        state.s.slider = 1;
    }
    else if (49000<=slider && slider <=52000){
        state.s.slider = 2;
    }
    else if (60000<=slider){
        state.s.slider = 3;
    }
}

void pull_state(){
    state.s.red_button = pin_read(&D[5]);
    state.s.green_button = pin_read(&D[5]);
    check_slider();
}

void slave_tx() {
    spi_queue_slave(&spi1, cmd);
    pin_set(Sint);
    pin_clear(Sint);
}

WORD last_state;
void pull_changes(){
    last_state = state;
    pull_state();
    printf("last:%x\n\r",last_state.w);
    printf("state:%x\n\r",state.w);
    if (last_state.w != state.w){
        led_toggle(&led1); 
        slave_tx();
    }
    cmd.w = state.w;
}

void init_slave_comms(void) {
    spi_open_slave(&spi1, MOSI, MISO, SCK, CSn, 1, 1);
    pin_digitalOut(Sint);
    pin_clear(Sint);
    pin_digitalIn(CSn);
    int_attach(&int1, CSn, 1, handle_CSn);
}

void init_slave(void){
    init_slave_comms();
    pull_state();
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    init_uart();

    cmd.w = 0xBEEF;
    init_slave();


    timer_setPeriod(&timer1, 0.5);
    timer_start(&timer1);

    timer_setPeriod(&timer2, 0.01);
    timer_start(&timer2);

    uint16_t switch_state3 = sw_read(&sw3);
    

    while(1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            led_toggle(&led3);
        }
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            pull_changes();
        }
        //     printf("Slave sent: 0x%x\r\n", 0x5A);
        //     printf("Slave received: 0x%x\r\n", res);
        
    }
}
