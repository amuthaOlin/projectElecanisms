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

void recieve_or_send_spi() {
    led_toggle(&led1);
    result.b[1] = spi_transfer(&spi1, cmd.b[1]);
    result.b[0] = spi_transfer(&spi1, cmd.b[0]);
    // printf("SLAVE_INT = %x\n\r",pin_read(SLAVE_INT));
    // printf("SSN = %x\n\r",pin_read(SSN));
    led_on(&led2);

}

void send_spi(){

    pin_set(SLAVE_INT);
    pin_clear(SLAVE_INT);

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
    spi_open_slave(&spi1, MISO, MOSI, SCK, SSN, 2e6 ,0);



    timer_setPeriod(&timer2, .5);
    timer_start(&timer2); 

    int_attach(&int1,SSN,0,recieve_or_send_spi);




    while (1) {
        led_toggle(&led3);
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            send_spi();

        }
    }
}

