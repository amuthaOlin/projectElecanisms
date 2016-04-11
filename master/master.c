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
volatile _CMD *cmd[3];

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *Sint1 = &D[4];
_PIN *Sint2 = &D[6];
_PIN *Sint3 = &D[8];

_PIN *SSn[] = { &D[3], &D[5], &D[7] };
_LCD *lcds[] = { &lcdcmd1, &lcdcmd2, &lcdcmd3 };

WORD32 master_tx(_PIN *SSn, WORD32 cmd){
    WORD32 tmp;
    pin_clear(SSn);
    tmp = spi_queue(&spi1, cmd);
    pin_set(SSn);
    return tmp;
}

void send_command(uint8_t console, _CMD *cmd, float cd_time) {
    res[console] = master_tx(SSn[console], cmd_packet(cmd->index));

    lcd_clear(lcds[console]);
    lcd_print1(lcds[console], cmd_strs[cmd->index]);

    cd_start(&cd[console], cd_time, game_clock);
}

void cons_state_change(uint8_t console) {
    res[console] = master_tx(SSn[console], (WORD32)0xFEEDF00D);
    uint8_t success = cmd_test(cmd[console]->index, res[console]);

    if (cd[console].active && success)
        game_advance(console, 1);

    // printf("Console %d: %08lx\r\n", console+1, (unsigned long)res[0].ul);
}

void cons1_state_change(_INT *intx) {
    cons_state_change(0);
}

void cons2_state_change(_INT *intx) {
    cons_state_change(1);
}

void cons3_state_change(_INT *intx) {
    cons_state_change(2);
}

uint16_t game_next_cmd_idx() {
    return 5;
}

void game_advance(uint8_t console, uint8_t success) {
    if (!success)
        cd_advance(&cdcenter, 2.0);

    send_command(console, cmd[game_next_cmd_idx()], 7);
}

void game_loop() {
    game_clock++;

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
        leds_writeRGB(&ledcenter, 0, 255,0,0);
        // game over
    }
}

void init_game() {
    timer_every(&timer1, GAME_TICK, game_loop);

    cmd[0] = &cmds[cmd_get(0, 0, 1)];
    cmd[1] = &cmds[cmd_get(1, 0, 1)];
    cmd[2] = &cmds[cmd_get(2, 0, 1)];

    cd_start(&cdcenter, 30, game_clock);
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

    pin_digitalOut(SSn[0]);
    pin_digitalOut(SSn[1]);
    pin_digitalOut(SSn[2]);
    pin_set(SSn[0]);
    pin_set(SSn[1]);
    pin_set(SSn[2]);
    led_off(&led1);
    led_off(&led2);
    led_off(&led3);

    int_attach(&int1, Sint1, 1, cons1_state_change);
    int_attach(&int2, Sint2, 1, cons2_state_change);
    int_attach(&int3, Sint3, 1, cons3_state_change);

    init_game();
}

int16_t main(void) {
    init_master();

    while (1) {}
}
