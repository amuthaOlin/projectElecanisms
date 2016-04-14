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

volatile uint16_t game_rand_val;
void game_rand_inc() {
    // value = (value & 0b1000000000000000) ? ((value ^ 0b0000000000100101) << 1) | 0b0000000000000001 : value << 1;
    game_rand_val = (game_rand_val & 0x8000) ? ((game_rand_val ^ 0x0025) << 1) | 0x0001 : game_rand_val << 1;
}

uint16_t game_rand_cmd_idx() {
    // should break sometimes because 98 is a possible (but unlikely) value
    return (uint16_t)((float)(game_rand_val)/0xFFFF*(GAME_NUM_CMDS+1));
}

void game_rand_init() {
    game_rand_val = 0x0080;
}

void game_advance(uint8_t sole, uint8_t success) {
    if (!success)
        cd_advance(&cdcenter, 2.0);

    con_send_cmd(&con[sole], &cmds[game_rand_cmd_idx()], 6);
}

void game_state_change(uint8_t sole) {
    uint8_t success = con_state_change(&con[sole]);
    if (success) {
        led_toggle(&led3);
        game_advance(sole, 1);
    }
}

void con1_state_change(_INT *intx) {
    game_state_change(0);
}

void con2_state_change(_INT *intx) {
    game_state_change(1);
}

void con3_state_change(_INT *intx) {
    game_state_change(2);
}

void game_loop() {
    game_rand_inc();
    game_clock++;
    printf("%u\r\n", pin_read(&A[0]));

    cd_update_all(game_clock);

    if (cd[0].flag) {
        cd[0].flag = 0;
        game_advance(0, 0);
    }
    if (cd[1].flag) {
        cd[1].flag = 0;
        game_advance(1, 0);
    }
    if (cd[2].flag) {
        cd[2].flag = 0;
        game_advance(2, 0);
    }
    if (cdcenter.flag) {
        cdcenter.flag = 0;
        leds_clear(&ledcenter);
        leds_writeRGBs(&ledcenter, 255,0,0);

        // game over
        while(1) {}
    }
}

void init_game() {
    game_rand_init();
    timer_every(&timer1, GAME_TICK, game_loop);

    cd_start(&cdcenter, 240, game_clock);

    con_send_cmd(&con[0], &cmds[cmd_get(0, 0, 1)], 6);
    con_send_cmd(&con[1], &cmds[cmd_get(1, 0, 1)], 6);
    con_send_cmd(&con[2], &cmds[cmd_get(2, 0, 1)], 6);
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
    cd[0].tick_sec = GAME_TICK;
    cd[1].tick_sec = GAME_TICK;
    cd[2].tick_sec = GAME_TICK;

    init_cmd();
    init_i2c();
    init_lcd();

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
