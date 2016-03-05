#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "ui.h"




uint16_t val1, val2;
_PIN *SCK, *MISO, *MOSI, *SSN, *SLAVE_INT;



WORD recieve_or_send_spi(WORD cmd) {
    WORD result;
    //printf("cmd = %u\n\r",cmd.w);


    pin_clear(SSN);
    printf("spiXSTAT = %x\n\r",*(spi1.SPIxSTAT));
    printf("spiXBUFF = %x\n\r",*(spi1.SPIxBUF));
    result.b[1] = spi_transfer(&spi1, cmd.b[1]);
    printf("spiXSTAT = %x\n\r",*(spi1.SPIxSTAT));
    printf("spiXBUFF = %x\n\r",*(spi1.SPIxBUF));
    result.b[0] = spi_transfer(&spi1, cmd.b[0]);
    printf("spiXSTAT = %x\n\r",*(spi1.SPIxSTAT));
    printf("spiXBUFF = %x\n\r",*(spi1.SPIxBUF));
    pin_set(SSN);

    return result;

}

void send_spi(WORD cmd){
    recieve_or_send_spi(cmd);
}

WORD recieve_spi(){
    WORD result;
    WORD cmd = (WORD) 0xFFFF;
    result = recieve_or_send_spi(cmd);
    return result;

}


int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_ui();
    init_pin();

    MISO = &D[1];
    MOSI = &D[0];
    SCK = &D[2];
    SSN = &D[3];
    SLAVE_INT = &D[4];

    pin_digitalIn(SLAVE_INT);
    pin_digitalOut(SSN);
    spi_open(&spi1, MISO, MOSI, SCK, 2e6 ,0);

    WORD result;


    while (1) {
        if (pin_read(SLAVE_INT)!=0){
            result = recieve_spi();
            //printf("res = %x\n\r",result.w);
            if (result.w == 0x0F0F){
                led_toggle(&led1);
            }
            led_toggle(&led2);
        }
    led_toggle(&led3);
    }
}

