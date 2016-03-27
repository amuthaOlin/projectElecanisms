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

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *CSn   = &D[3];
_PIN *Sint  = &D[4];

volatile WORD32 res, cmd, state;

void handle_CSn(_INT *intx) {
    led_toggle(&led1);
    res = spi_read_slave(&spi1);
    //printf("res:%x%x\n\r",res.w[1],res.w[0]);
    if (res.ul == 0xA1B2C3D4) {
        led_toggle(&led2);
    }
}

void init_slave_comms(void) {
    spi_open_slave(&spi1, MOSI, MISO, SCK, CSn, 1, 1);
    pin_digitalOut(Sint);
    pin_clear(Sint);
    pin_digitalIn(CSn);
    int_attach(&int1, CSn, 1, handle_CSn);
}

void slave_tx() {
    spi_queue_slave(&spi1, cmd);
    pin_set(Sint);
    pin_clear(Sint);
}

void init_slave(void){
    init_slave_comms();
    pull_state();
}


WORD32 pull_changes(){
    
}

pull_state(){
    pin_read(&D[5]);
    

}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    init_uart();

    cmd.ul = 0x567890EF;
    init_slave_comms();

    timer_setPeriod(&timer1, 0.5);
    timer_start(&timer1);

    uint16_t switch_state3 = sw_read(&sw3);
    

    while(1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            led_toggle(&led3);

            slave_tx();
        }
        // if (sw_read(&sw3) != switch_state3){
        //     switch_state3 = sw_read(&sw3);
        //     slave_tx();
        // }
        //     //res = spi_transfer_slave(&spi1, 0x5A, Sint);

        //     printf("Slave sent: 0x%x\r\n", 0x5A);
        //     printf("Slave received: 0x%x\r\n", res);
        
    }
}
