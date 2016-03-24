#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "spi.h"
#include "timer.h"
#include "uart.h"


_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *CSn   = &D[3];
_PIN *Sint  = &D[4];


volatile WORD32 res, cmd;


WORD32 recieve_and_send_spi(cmd){
    res.b[3] = spi_transfer_slave(&spi1, cmd.b[3], Sint);
    res.b[2] = spi_transfer_slave(&spi1, cmd.b[2], Sint);
    res.b[1] = spi_transfer_slave(&spi1, cmd.b[1], Sint);
    res.b[0] = spi_transfer_slave(&spi1, cmd.b[0], Sint);
    return res;
}

void handle_CSn(_INT *intx) {
    res = recieve_and_send_spi(0x00);
}


void init_slave_comms(void){
    spi_open_slave(&spi1, MOSI, MISO, SCK, 1);
    pin_digitalOut(Sint);
    pin_clear(Sint);
    pin_digitalIn(CSn);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    init_uart();

    init_slave_comms();


    timer_setPeriod(&timer1, 0.5);
    timer_start(&timer1);

    int_attach(&int1, CSn, 1, handle_CSn);

    while(1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);

            //res = spi_transfer_slave(&spi1, 0x5A, Sint);

            led_toggle(&led3);
            printf("Slave sent: 0x%x\r\n", 0x5A);
            printf("Slave received: 0x%x\r\n", res);
        }
    }
}

