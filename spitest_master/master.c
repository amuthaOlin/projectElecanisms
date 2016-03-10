#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "ui.h"
#include "int.h"




volatile uint8_t res, cmd;
_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *CSn1  = &D[3];
_PIN *Sint1 = &D[4];
_PIN *CSn2  = &D[5];
_PIN *Sint2 = &D[6];
_PIN *CSn3  = &D[7];
_PIN *Sint3 = &D[8];



void init_master_pins(){
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);
    pin_digitalIn(Sint3);

    pin_digitalOut(CSn1);
    pin_digitalOut(CSn2);
    pin_digitalOut(CSn3);
    pin_set(CSn1);
    pin_set(CSn2);
    pin_set(CSn3);
}

void recieve_and_send_spi(_PIN *CSn){
    res = spi_transfer(&spi1, cmd, CSn);
}

//recieve SPI interrupt handler
void handle_sint1(_INT *intx) {
    recieve_and_send_spi(CSn1);
}

void handle_sint2(_INT *intx) {
    recieve_and_send_spi(CSn2);
}

void handle_sint3(_INT *intx) {
    recieve_and_send_spi(CSn3);
}


int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_ui();
    init_pin();
    init_int();

    init_master_pins();


    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1);

    int_attach(&int1, Sint1, 0, handle_sint1);
    int_attach(&int2, Sint1, 0, handle_sint2);
    int_attach(&int3, Sint1, 0, handle_sint3);

    while (1) {
    }
}

