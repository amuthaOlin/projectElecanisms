#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "ui.h"
#include "timer.h"
#include "int.h"



_PIN *CSn = &D[3];
_PIN *Sint = &D[4];
_PIN *MISO = &D[1];
_PIN *MOSI = &D[0];
_PIN *SCK = &D[2];


uint8_t recieve_spi(void) {
    
    uint8_t res;
    res = spi_read_slave(&spi1);
    return res;
}

void send_spi(uint8_t cmd){

    spi_write_slave(&spi1,cmd);
    pin_set(Sint); // pulse Sint to request transaction
    pin_clear(Sint); // trigger on falling edge
}

uint8_t recieve_and_send_spi(uint8_t cmd){

    send_spi(cmd);
    uint8_t res;
    res = recieve_spi();
    return res;
}


int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    init_uart();




    spi_open_slave(&spi1, MOSI, MISO, SCK, 1);

    pin_digitalOut(Sint);
    pin_clear(Sint);

    pin_digitalIn(CSn);

    timer_setPeriod(&timer1, 0.5);
    timer_start(&timer1);

    uint8_t res = 0;
    uint8_t cmd = 0;


    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            recieve_and_send_spi(cmd);
            
            //led_toggle(&led2);
            //printf("Slave sent: 0x%x\r\n", 0x5A);
            //printf("Slave received: 0x%x\r\n", res);
        }

    }
}

