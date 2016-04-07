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
volatile int32_t game_clock = 0; // time unit of "ticks"

volatile WORD32 res[3];
volatile WORD32 cmd[3];
WORD32 desired_state[3];

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *SSn1  = &D[3];
_PIN *Sint1 = &D[4];
_PIN *SSn2  = &D[5];
_PIN *Sint2 = &D[6];
_PIN *SSn3  = &D[7];
_PIN *Sint3 = &D[8];

WORD32 master_tx(_PIN *SSn, WORD32 cmd){
    WORD32 tmp;
    pin_clear(SSn);
    tmp = spi_queue(&spi1, cmd);
    pin_set(SSn);
    return tmp;
}

void send_command(uint8_t slave, WORD32 cmd) {
    switch (slave) {
        case 0:
            res[0] = master_tx(SSn1, cmd);
            cd_start(&cd1, 1, game_clock);
            break;
        case 1:
            res[1] = master_tx(SSn2, cmd);
            cd_start(&cd2, 4, game_clock);
            break;
        case 2:
            res[2] = master_tx(SSn3, cmd);
            cd_start(&cd3, 9, game_clock);
            break;
    }
}

void handle_sint1(_INT *intx) {
    res[0] = master_tx(SSn1, (WORD32)0xFEEDF00D);
    if (res[0].l == desired_state[0].l) {
        send_command(0, cmd[0]);
        led_off(&led1);
    }
    printf("console1:%lu\n\r",res[0]);
}

void handle_sint2(_INT *intx) {
    res[1] = master_tx(SSn2, (WORD32)0xFEEDF00D);
    if (res[1].l == desired_state[1].l) {
        send_command(1, cmd[1]);
        led_off(&led2);
    }
    printf("console2:%lu\n\r",res[1]);
}

void handle_sint3(_INT *intx) {
    res[2] = master_tx(SSn3, (WORD32)0xFEEDF00D);
    if (res[2].l == desired_state[2].l) {
        send_command(2, cmd[2]);
        led_off(&led3);
    }
    printf("console3:%u\n\r",res[2].s3.clutch);
}

void init_master_comms() {
    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1, 1);
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);
    pin_digitalIn(Sint3);

    pin_digitalOut(SSn1);
    pin_digitalOut(SSn2);
    pin_digitalOut(SSn3);
    pin_set(SSn1);
    pin_set(SSn2);
    pin_set(SSn3);
    led_off(&led1);
    led_off(&led2);
    led_off(&led3);

    int_attach(&int1, Sint1, 1, handle_sint1);
    int_attach(&int2, Sint2, 1, handle_sint2);
    int_attach(&int3, Sint3, 1, handle_sint3);
}

void game_loop() {
    game_clock++;

    cd_update_all(game_clock);

    if (cd1.flag) {
        leds_writeRGBs(&ledbar1, 255,0,0);
        cd_advance(&cdcenter, 2.0);
    }
    if (cd2.flag) {
        leds_writeRGBs(&ledbar2, 255,0,0);
        cd_advance(&cdcenter, 2.0);
    }
    if (cd3.flag) {
        leds_writeRGBs(&ledbar3, 255,0,0);
        cd_advance(&cdcenter, 2.0);
    }
    if (cdcenter.flag) {
        leds_writeRGB(&ledcenter, 0, 255,0,0);
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

    WORD32 s1state = (WORD32)0;
    s1state.s1.red_button = 1;
    WORD32 s2state = (WORD32)0;
    s2state.s2.slider = 2;
    WORD32 s3state = (WORD32)0;
    s3state.s3.triangle1 = 1;

    desired_state[0] = s1state;
    desired_state[1] = s2state;
    desired_state[2] = s3state;

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

    cd1.tick_sec = GAME_TICK;
    cd2.tick_sec = GAME_TICK;
    cd3.tick_sec = GAME_TICK;
    
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
