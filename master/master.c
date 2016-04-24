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
#include "con.h"
#include "rng.h"
#include "i2c.h"
#include "lcd.h"
#include "cmd.h"
#include "int.h"
#include "play.h"
#include "timer.h"
#include "spacecomms.h"
#include "oc.h"

volatile WORD32 res[3];
volatile _CMD *commands[3];

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *Sint1 = &D[10];
_PIN *Sint2 = &D[6];
_PIN *Sint3 = &D[8];

_PIN *SSn[] = { &D[3], &D[5], &D[7] };
_PIN *Coin = &D[12];

void con1_state_change(_INT *intx) {
    play_state_change(0);
}

void con2_state_change(_INT *intx) {
    play_state_change(1);
}

void con3_state_change(_INT *intx) {
    play_state_change(2);
}

void init_game(_INT *intx) {
    uint8_t level_success = play_level(); // (for now)
}

void init_master() {
    init_clock();
    init_uart();
    init_spi();
    init_timer();
    timer_initDelayMicro(&timer5);
    init_ui();
    init_pin();
    init_oc();
    init_int();
    init_leds();
    init_cd();

    // this represents a problem
    // cd1.tick_sec = GAME_TICK;
    // cd2.tick_sec = GAME_TICK;
    // cd3.tick_sec = GAME_TICK;

    init_cmd();
    init_i2c();
    init_lcd();
    init_rng();
    init_con();

    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1, 1);
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);
    pin_digitalIn(Sint3);
    pin_digitalIn(Coin);

    int_attach(&int1, Sint1, 1, con1_state_change);
    int_attach(&int2, Sint2, 1, con2_state_change);
    int_attach(&int3, Sint3, 1, con3_state_change);
    int_attach(&int4, Coin, 0, init_game);

    //init_game();
}

int16_t main(void) {
    init_master();
    while (1) {}
}
