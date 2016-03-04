#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "ui.h"
#include "timer.h"



uint16_t val1, val2;
_PIN *SCK, *MISO, *MOSI;
_PIN *NCS;
_PIN *SLAVE_INT;



WORD recieve_or_send_spi(WORD cmd) {
    WORD result;
    printf("cmd = %u\n\r",cmd);


    result.b[1] = spi_transfer(&spi1, cmd.b[1]);
    result.b[0] = spi_transfer(&spi1, cmd.b[0]);

    return result;

}

void send_spi(WORD cmd){
    pin_set(&D[4]);
    uint16_t i;
    for (i = 0;i<1e4;i++)
    {

    }
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
    init_timer();


    MISO = &D[1];
    MOSI = &D[0];
    SCK = &D[2];
    NCS = &D[3];
    SLAVE_INT = &D[4];

    pin_digitalIn(&D[5]);
    pin_digitalOut(SLAVE_INT);

    spi_open_slave(&spi1, MISO, MOSI, SCK, 2e6 ,0);



    WORD result;

    timer_setPeriod(&timer2, .5);
    timer_start(&timer2); 

    WORD command = (WORD) 0x0F0F;

    while (1) {
        led_toggle(&led3);
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);
            send_spi(command);
            led_toggle(&led2);
        }
    }
}

