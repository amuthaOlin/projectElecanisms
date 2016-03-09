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



_PIN *SCK, *MISO, *MOSI, *SSN, *SLAVE_INT;
volatile WORD cmd = (WORD) 0x0F0F;
volatile WORD result;

void recieve_spi() {
    
    result = (WORD)(*(spi1.SPIxBUF));
    printf("result = %x\n\r,result.w");

}

void send_spi(){

    led_toggle(&led1);
    result.b[1] = spi_slave_transfer(&spi1, cmd.b[1]);
    result.b[0] = spi_slave_transfer(&spi1, cmd.b[0]);
    printf("SPIxSTAT = %x\n\r",*(spi1.SPIxSTAT)); 


    pin_set(SLAVE_INT);
    pin_clear(SLAVE_INT);

    printf("cmd = %x\n\r",cmd.w);    
    //printf("spiBuf = %x\n\r",SPI1BUF);
}


int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_ui();
    init_pin();
    init_timer();
    init_int();

    MISO = &D[1];
    MOSI = &D[0];
    SCK = &D[2];
    SSN = &D[3];
    SLAVE_INT = &D[4];

    pin_digitalOut(SLAVE_INT);
    pin_clear(SLAVE_INT);
    spi_open_slave(&spi1, MISO, MOSI, SCK, SSN, 0);



    timer_setPeriod(&timer2, .5);
    timer_start(&timer2); 

    //int_attach(&int1,SSN,0,recieve_spi);




    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            send_spi();

        }
        led_toggle(&led2);
    }
}

