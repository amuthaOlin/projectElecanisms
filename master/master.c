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
volatile WORD32 cmd[3];
volatile WORD32 desired_state[3];

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *SSn0  = &D[3];
_PIN *Sint0 = &D[4];
_PIN *SSn1  = &D[5];
_PIN *Sint1 = &D[6];
_PIN *SSn2  = &D[7];
_PIN *Sint2 = &D[8];

WORD32 master_tx(_PIN *CSn, WORD32 cmd){
    WORD32 tmp;
    pin_clear(CSn);
    tmp = spi_queue(&spi1, cmd);
    pin_set(CSn);
    // printf("====================\n\r");
    // printf("Transaction complete\n\r");
    // printf("Tx: %x%x%x%x\n\r", cmd.b[3], cmd.b[2], cmd.b[1], cmd.b[0]);
    // printf("Rx: %x%x%x%x\n\r", tmp.b[3], tmp.b[2], tmp.b[1], tmp.b[0]);
    return tmp;
}

// if the state is correct, send the command again
void handle_sint0(_INT *intx) {
    res[0] = master_tx(SSn0, (WORD32)0x00000000);
}

void handle_sint1(_INT *intx) {
    res[1] = master_tx(SSn1, (WORD32)0x00000000);
}

void handle_sint2(_INT *intx) {
    res[2] = master_tx(SSn2, (WORD32)0x00000000);
}

void send_command(uint8_t slave, WORD32 cmd) {
    switch (slave) {
        case 0:
            res[0] = master_tx(SSn0, cmd);
            cd_start(&cd1, 1, game_clock);
            break;
        case 1:
            res[1] = master_tx(SSn1, cmd);
            cd_start(&cd2, 4, game_clock);
            break;
        case 2:
            res[2] = master_tx(SSn2, cmd);
            cd_start(&cd3, 9, game_clock);
            break;
    }
}

void init_master_comms() {
    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1, 1);
    pin_digitalIn(Sint0);
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);

    pin_digitalOut(SSn0);
    pin_digitalOut(SSn1);
    pin_digitalOut(SSn2);
    pin_set(SSn0);
    pin_set(SSn1);
    pin_set(SSn2);
    led_off(&led1);
    led_off(&led2);
    led_off(&led3);

    int_attach(&int1, Sint0, 1, handle_sint0);
    int_attach(&int2, Sint1, 1, handle_sint1);
    int_attach(&int3, Sint2, 1, handle_sint2);
}

void game_loop() {
    game_clock++;

    cd_update_all(game_clock);

    if (res[0].l == desired_state[0].l) {
       send_command(0, cmd[0]);
    }
    if (res[1].l == desired_state[1].l) {
       send_command(1, cmd[1]);
    }
    if (res[2].l == desired_state[2].l) {
       send_command(2, cmd[2]);
    }

    if (cd1.flag) {
        // advance game countdown
        led_off(&led1);
    }
    if (cd2.flag) {
        // advance game countdown
        led_off(&led2);
    }
    if (cd3.flag) {
        // advance game countdown
        led_off(&led3);
    }
    if (cdcenter.flag) {
        // game over
    }
}

void game_init() {
    init_master_comms();
    timer_every(&timer1, GAME_TICK, game_loop);

    SPACK_DIR cmdtest;
    cmdtest.packet = 0;
    cmdtest.actaddr = 0;
    cmdtest.actact = 1;
    // put actuator 0 in state 1 ("press a button")
    cmd[0] = (WORD32)cmdtest;
    cmd[1] = (WORD32)cmdtest;
    cmd[2] = (WORD32)cmdtest;

    SLAVE0_STATE s0state;
    s0state.red_button = 1;
    SLAVE1_STATE s1state;
    s1state.slider = 2;
    SLAVE2_STATE s2state;
    s2state.green_button = 1;

    desired_state[0] = (WORD32)s0state;
    desired_state[1] = (WORD32)s1state;
    desired_state[2] = (WORD32)s2state;

    cd_start(&cdcenter, 3, game_clock);
}

volatile uint8_t sw1_last = 0;
volatile uint8_t sw2_last = 0;
volatile uint8_t sw3_last = 0;
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

    while (1) {
        if (!sw_read(&sw1) && sw1_last == 1) {
            send_command(0, cmd[0]);
            led_on(&led1);
        }
        if (!sw_read(&sw2) && sw2_last == 1) {
            send_command(1, cmd[1]);
            led_on(&led2);
        }
        if (!sw_read(&sw3) && sw3_last == 1) {
            send_command(2, cmd[2]);
            led_on(&led3);
        }
        sw1_last = sw_read(&sw1);
        sw2_last = sw_read(&sw2);
        sw3_last = sw_read(&sw3);
    }
}

// SPACK_DIR dirpacket = workspackunion.whateveryoucallthepackdir;

// dirpacket.actaddr
// uint8_t some_val = dirpacket.actact

// union {
//     WORD32
//     SPACK_DIR
// }
