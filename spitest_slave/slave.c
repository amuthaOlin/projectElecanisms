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

volatile WORD32 res, cmd;
volatile WORD state;

WORD32 recieve_and_send_spi() {
    // printf("Receive and Sending SPI\n\r");
    // printf("cmd:%x%x\n\r",cmd.w[1],cmd.w[0]);
    // printf("BUFFER:%x\n\r",*(spi1.SPIxBUF));
    res.b[3] = spi_transfer_slave(&spi1, cmd.b[3]);
    res.b[2] = spi_transfer_slave(&spi1, cmd.b[2]);
    // res.b[1] = spi_transfer_slave(&spi1, cmd.b[1]);
    // res.b[0] = spi_transfer_slave(&spi1, cmd.b[0]);
    return res;
}

void handle_CSn(_INT *intx) {

    if (!sw_read(&sw2)){
        return;
    }
    printf("CS called\n\r");
    led_toggle(&led1);
    res = recieve_and_send_spi();
    //printf("res:%x%x\n\r",res.w[1],res.w[0]);
    if (res.ul == 0xA1B2C3D4){
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
    check_slider()
}


void pull_changes(){
    WORD last_state = state;
    pull_state();    
    printf("last:%u\n\r",last_state.w);
    printf("state:%u\n\r",state.w);
    if (last_state.w!= state.w){
        led_toggle(&led1); 
    }
}

void init_slave_comms(void) {
    spi_open_slave(&spi1, MOSI, MISO, SCK, 1);
    pin_digitalOut(Sint);
    pin_clear(Sint);
    pin_digitalIn(CSn);
    int_attach(&int1, CSn, 0, handle_CSn);
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

    cmd.ul = 0xBEEFFACE;
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
        if (sw_read(&sw3) != switch_state3){
            //printf("switch_press");
            switch_state3 = sw_read(&sw3);
            cmd.ul = 0x567890EF;
            pin_set(Sint);
            pin_clear(Sint);
        }
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            pull_changes();
        }
        //     //res = spi_transfer_slave(&spi1, 0x5A, Sint);

        //     printf("Slave sent: 0x%x\r\n", 0x5A);
        //     printf("Slave received: 0x%x\r\n", res);
        
    }
}
