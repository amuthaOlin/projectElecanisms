#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "leds.h"
#include "ui.h"
#include "cd.h"
#include "int.h"
#include "timer.h"
#include "spacecomms.h"
#include "oc.h"

#define GAME_TICK 1e-2 // seconds
volatile uint32_t game_clock = 0; // time unit of "ticks"

volatile WORD32 res[3];

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *CSn1  = &D[3];
_PIN *Sint1 = &D[4];
_PIN *CSn2  = &D[5];
_PIN *Sint2 = &D[6];
_PIN *CSn3  = &D[7];
_PIN *Sint3 = &D[8];

WORD32 master_tx(_PIN *CSn, WORD32 cmd){
    WORD32 res;
    pin_clear(CSn);
    res = spi_queue(&spi1, cmd);
    pin_set(CSn);
    // printf("====================\n\r");
    // printf("Transaction complete\n\r");
    // printf("Tx: %x%x%x%x\n\r", cmd.b[3], cmd.b[2], cmd.b[1], cmd.b[0]);
    // printf("Rx: %x%x%x%x\n\r", res.b[3], res.b[2], res.b[1], res.b[0]);
    return res;
}

//recieve SPI interrupt handler
void handle_sint1(_INT *intx) {
    res[0] = master_tx(CSn1, (WORD32)0x00000000);
}

void handle_sint2(_INT *intx) {
    res[1] = master_tx(CSn2, (WORD32)0x00000000);
}

void handle_sint3(_INT *intx) {
    res[2] = master_tx(CSn3, (WORD32)0x00000000);
}

void send_command(uint8_t slave, SPACK_DIR cmd) {
    switch (slave) {
        case 1:
            res[0] = master_tx(CSn1, (WORD32)cmd);
            cd_start(&cd1, 1, game_clock);
            break;
        case 2:
            res[1] = master_tx(CSn2, (WORD32)cmd);
            cd_start(&cd2, 4, game_clock);
            break;
        case 3:
            res[2] = master_tx(CSn3, (WORD32)cmd);
            cd_start(&cd3, 9, game_clock);
            break;
    }
}

void init_master_comms() {
    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1, 1);
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

    int_attach(&int1, Sint1, 1, handle_sint1);
    int_attach(&int2, Sint2, 1, handle_sint2);
    int_attach(&int3, Sint3, 1, handle_sint3);
}

void game_loop() {
    game_clock++;
    led_toggle(&led3);

    cd_update_all(game_clock);

    if (cd1.flag) { // if the flag comes up, the command failed
        // penalty for console n
    }
    if (cd2.flag) {

    }
    if (cd3.flag) {

    }
}

void game_init() {
    init_master_comms();
    timer_every(&timer1, GAME_TICK, game_loop);

    cd_start(&cd1, 1, game_clock);
    cd_start(&cd2, 2, game_clock);
    cd_start(&cd3, 3, game_clock);
}

int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_timer();
    init_ui();
    init_pin();
    init_oc();
    init_int();
    init_leds();
    init_cd();

    cd1.step_sec = GAME_TICK;
    cd2.step_sec = GAME_TICK;
    cd3.step_sec = GAME_TICK;
    
    game_init();

    while (1) {}
}

// SPACK_DIR dirpacket = workspackunion.whateveryoucallthepackdir;

// dirpacket.actaddr
// uint8_t some_val = dirpacket.actact

// union {
//     WORD32
//     SPACK_DIR
// }
