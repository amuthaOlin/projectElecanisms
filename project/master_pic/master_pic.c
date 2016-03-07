#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "ui.h"
#include "int.h"



uint16_t val1, val2;
_PIN *SCK, *MISO, *MOSI, *SSN, *SLAVE_INT;
volatile WORD result;


void recieve_or_send_spi(WORD cmd) {
    //printf("cmd = %u\n\r",cmd.w);
    //printf("spiXSTAT = %x\n\r",*(spi1.SPIxSTAT));
    //printf("spiXBUFF = %x\n\r",*(spi1.SPIxBUF));
    led_on(&led1);
    pin_clear(SSN);

    result.b[1] = spi_transfer(&spi1, cmd.b[1]);
    result.b[0] = spi_transfer(&spi1, cmd.b[0]);

    pin_set(SSN);
    led_on(&led2);

    printf("result = %x\n\r",result);

}

void send_spi(WORD cmd){
    recieve_or_send_spi(cmd);
}

void recieve_spi(){

    WORD cmd = (WORD) 0xFFFF;
    recieve_or_send_spi(cmd);

}


int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_ui();
    init_pin();
    init_int();

    MISO = &D[1];
    MOSI = &D[0];
    SCK = &D[2];
    SSN = &D[3];
    SLAVE_INT = &D[4];

    pin_digitalIn(SLAVE_INT);
    pin_digitalOut(SSN);
    pin_set(SSN);

    spi_open(&spi1, MISO, MOSI, SCK, 2e6 ,0);

    int_attach(&int1,SLAVE_INT,1,recieve_spi);

    while (1) {
        
        if (result.w == 0x0F0F){

        }
        else{
            
        }
    led_on(&led3); 
    }
}

