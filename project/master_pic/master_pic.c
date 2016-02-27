#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "ui.h"




uint16_t val1, val2;
_PIN *ENC_SCK, *ENC_MISO, *ENC_MOSI;
_PIN *ENC_NCS;



WORD recieve_or_send_spi(WORD cmd) {
    WORD result;
   

    pin_clear(ENC_NCS);
    spi_transfer(&spi1, cmd.b[1]);
    spi_transfer(&spi1, cmd.b[0]);
    pin_set(ENC_NCS);

    pin_clear(ENC_NCS);
    result.b[1] = spi_transfer(&spi1, 0);
    result.b[0] = spi_transfer(&spi1, 0);
    pin_set(ENC_NCS);

    return result;

}

void send_spi(WORD cmd){
    pin_set(&D[4]);
    recieve_or_send_spi(cmd);
    pin_clear(&D[4]);
}

WORD recieve_spi(){
    WORD result;
    WORD cmd;
    result = recieve_or_send_spi(cmd);
    return result;

}


int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_ui();
    init_pin();

    ENC_MISO = &D[1];
    ENC_MOSI = &D[0];
    ENC_SCK = &D[2];
    ENC_NCS = &D[3];


    spi_open(&spi1, ENC_MISO, ENC_MOSI, ENC_SCK, 2e6 ,1);



    WORD result;


    while (1) {
        if (pin_read(&D[4])){
            result = recieve_spi();
            if (result.b[0] == 0xFF){
                led_toggle(&led1);
    
            }
        led_toggle(&led2);
        }
    led_toggle(&led3);
    }
}

