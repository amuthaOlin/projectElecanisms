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

    lcd_print(lcds[console], cmd_strs[cmd->index]);

    cd_start(&cd[console], cd_time, game_clock);
}

void cons1_state_change(_INT *intx) {
    res[0] = master_tx(SSn[0], (WORD32)0xFEEDF00D);
    uint8_t success = cmd_test(cmd[0]->index, res[0]);

    if (success)
        led_off(&led1);

    // printf("Console 1: %08lx\r\n", (unsigned long)res[0].ul);
}

void cons2_state_change(_INT *intx) {
    res[1] = master_tx(SSn[1], (WORD32)0xFEEDF00D);
    uint8_t success = cmd_test(cmd[1]->index, res[1]);

    if (success)
        led_off(&led1);

    // printf("Console 2: %08lx\r\n", (unsigned long)res[1].ul);
}

void cons3_state_change(_INT *intx) {
    res[2] = master_tx(SSn[2], (WORD32)0xFEEDF00D);
    uint8_t success = cmd_test(cmd[2]->index, res[2]);

    if (success)
        led_off(&led1);

    // printf("Console 3: %08lx\r\n", (unsigned long)res[2].ul);
}

void game_loop() {
    game_clock++;

    cd_update_all(game_clock);

    if (cd[0].flag) {
        cd[0].flag = 0;
        leds_writeRGBs(&ledbar1, 255,0,0);
        cd_advance(&cdcenter, 2.0);
    }
    if (cd[1].flag) {
        cd[1].flag = 0;
        leds_writeRGBs(&ledbar2, 255,0,0);
        cd_advance(&cdcenter, 2.0);
    }
    if (cd[2].flag) {
        cd[2].flag = 0;
        leds_writeRGBs(&ledbar3, 255,0,0);
        cd_advance(&cdcenter, 2.0);
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

    cd_start(&cdcenter, 10, game_clock);
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

volatile uint8_t sw1_last = 0;
volatile uint8_t sw2_last = 0;
volatile uint8_t sw3_last = 0;
int16_t main(void) {
    init_master();

    while (1) {
        if (!sw_read(&sw1) && sw1_last == 1) {
            send_command(0, cmd[0], 2);
            led_on(&led1);
        }
        if (!sw_read(&sw2) && sw2_last == 1) {
            send_command(1, cmd[1], 2);
            led_on(&led2);
        }
        if (!sw_read(&sw3) && sw3_last == 1) {
            send_command(2, cmd[2], 2);
            led_on(&led3);
        }
        sw1_last = sw_read(&sw1);
        sw2_last = sw_read(&sw2);
        sw3_last = sw_read(&sw3);
    }
}
