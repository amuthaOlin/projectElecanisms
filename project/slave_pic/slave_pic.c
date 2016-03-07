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
    
    result1 = (uint8_t)(*(&spi1->SPIxBUF));
    printf("result = %x\n\r,result.w");

}

void send_spi(){
    led_toggle(&led1);
    pin_set(SLAVE_INT);
    pin_clear(SLAVE_INT);
    *(&spi1->SPIxBUF) = (uint16_t)cmd.b[1];
    *(&spi1->SPIxBUF) = (uint16_t)cmd.b[0];
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
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            send_spi();

        }
    }
}

