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
#include "timer.h"
#include "spacecomms.h"
#include "oc.h"

#define GAME_TICK 1e-2 // seconds
volatile int32_t game_clock = 0; // time unit of "ticks"

volatile WORD32 res[3];
volatile _CMD *commands[3];

#define GAME_NUM_CMDS 97

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *Sint1 = &D[10];
_PIN *Sint2 = &D[6];
_PIN *Sint3 = &D[8];

_PIN *SSn[] = { &D[3], &D[5], &D[7] };

uint16_t game_rand_cmd_idx() {
    return rng_int(0, GAME_NUM_CMDS);
}

void game_advance(uint8_t sole, uint8_t success) {
    if (!success)
        cd_advance(&cdcenter, 2.0);

    con_send_cmd(&con[sole], &cmds[game_rand_cmd_idx()], 6, game_clock);
}

void game_state_change(uint8_t sole) {
    uint8_t success = con_state_change(&con[sole]);
    if (success) {
        game_advance(sole, 1);
    }
}

void con1_state_change(_INT *intx) {
    game_state_change(0);
    led_toggle(&led1);
}

void con2_state_change(_INT *intx) {
    game_state_change(1);
    led_toggle(&led2);
}

void con3_state_change(_INT *intx) {
    game_state_change(2);
    led_toggle(&led3);
}

void game_loop() {
    game_clock++;

    cd_update_all(game_clock);

    if (cd1.flag) {
        printf("FLAG BAG 1\r\n");
        cd1.flag = 0;
        game_advance(0, 0);
    }
    if (cd2.flag) {
        printf("FLAG BAG 2\r\n");
        cd2.flag = 0;
        game_advance(1, 0);
    }
    if (cd3.flag) {
        printf("FLAG BAG 3\r\n");
        cd3.flag = 0;
        game_advance(2, 0);
    }
    if (cdcenter.flag) {
        cdcenter.flag = 0;
        leds_clear(&ledcenter);
        leds_writeRGBs(&ledcenter, 255,0,0);

        // level over
        while(1) {}
    }
}

void init_game() {
    timer_every(&timer1, GAME_TICK, game_loop);

    cd_start(&cdcenter, 240, game_clock);

    con_send_cmd(&con[0], &cmds[cmd_get(0, 0, 1)], 6, game_clock);
    con_send_cmd(&con[1], &cmds[cmd_get(1, 0, 1)], 6, game_clock);
    con_send_cmd(&con[2], &cmds[cmd_get(2, 0, 1)], 6, game_clock);
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
    cd1.tick_sec = GAME_TICK;
    cd2.tick_sec = GAME_TICK;
    cd3.tick_sec = GAME_TICK;

    init_cmd();
    init_i2c();
    init_lcd();
    init_rng();
    init_con();

    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1, 1);
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);
    pin_digitalIn(Sint3);

    int_attach(&int1, Sint1, 1, con1_state_change);
    int_attach(&int2, Sint2, 1, con2_state_change);
    int_attach(&int3, Sint3, 1, con3_state_change);

    init_game();
}

int16_t main(void) {
    init_master();

    while (1) {}
}
