#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "ui.h"
#include "int.h"
#include "timer.h"



volatile WORD32 res1, res2, res3, cmd;

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *CSn1  = &D[3];
_PIN *Sint1 = &D[4];
_PIN *CSn2  = &D[5];
_PIN *Sint2 = &D[6];
_PIN *CSn3  = &D[7];
_PIN *Sint3 = &D[8];





WORD32 recieve_and_send_spi(_PIN *CSn){
    WORD32 res;
    res.b[3] = spi_transfer(&spi1, cmd.b[3], CSn);
    res.b[2] = spi_transfer(&spi1, cmd.b[2], CSn);
    res.b[1] = spi_transfer(&spi1, cmd.b[1], CSn);
    res.b[0] = spi_transfer(&spi1, cmd.b[0], CSn);
    printf("BUFFER%x\n\r",*(spi1.SPIxBUF));
    printf("res:%x%x\n\r",res.w[1],res.w[0]);
    printf("cmd:%x%x\n\r",cmd.w[1],cmd.w[0]);
    return res;
}

//recieve SPI interrupt handler
void handle_sint1(_INT *intx) {
    res1 = recieve_and_send_spi(CSn1);
}

void handle_sint2(_INT *intx) {
    res2 = recieve_and_send_spi(CSn2);
}

void handle_sint3(_INT *intx) {
    res3 = recieve_and_send_spi(CSn3);
}

void send_all() {
    send_slave(1);
    send_slave(2);
    send_slave(3);
}


void send_slave(slave) {
    if (slave == 1){
        res1 = recieve_and_send_spi(CSn1);
    }
    if (slave == 2){
        res2 = recieve_and_send_spi(CSn2);
    }
    if (slave == 3){
        res3 = recieve_and_send_spi(CSn3);
    }
}



void init_master_comms(){
    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1);
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);
    pin_digitalIn(Sint3);

    pin_digitalOut(CSn1);
    pin_digitalOut(CSn2);
    pin_digitalOut(CSn3);
    pin_set(CSn1);
    pin_set(CSn2);
    pin_set(CSn3);
    led_off(&led1);
    led_off(&led2);
    led_off(&led3);

    int_attach(&int1, Sint1, 0, handle_sint1);
    int_attach(&int2, Sint1, 0, handle_sint2);
    int_attach(&int3, Sint1, 0, handle_sint3);

}


void game_init(){
    init_master_comms();
    cmd.ul = 0xA1B2C3D4;
    send_all();

}

void game_state(){
    
}



int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_timer();
    init_ui();
    init_pin();
    init_int();
    

    game_init();
    cmd.ul = 0xFACEBEEF;



    while (1) {
        if (res1.ul == 0x567890EF){
            led_toggle(&led1);
        }
        if (res2.ul == 0x567890EF){
            led_toggle(&led2);
        }
        if (res3.ul == 0x567890EF){
            led_toggle(&led3);
        }
    }
}

// SPACK_DIR dirpacket = workspackunion.whateveryoucallthepackdir;

// dirpacket.actaddr
// uint8_t some_val = dirpacket.actact

// union {
//     WORD32
//     SPACK_DIR
// }