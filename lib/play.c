#include <p24FJ128GB206.h>
#include "play.h"

#include "rng.h"
#include "con.h"
#include "lev.h"
#include "ui.h"
#include "lcd.h"
#include "spacecomms.h"

#define PLAY_NUM_CMDS 90
#define PLAY_TICK 1e-2 // seconds

volatile uint8_t PLAYING = 0;

_PLAY play;

uint16_t __play_rand_cmd_idx() {
    uint8_t act = 0;
    act = rng_int(0, 17);
    uint8_t cons = act/6;

    uint8_t numstates = CONS_STATES[cons][act];
    uint8_t state = rng_int(0, numstates-1);
    return cmd_get(cons, act, state);
}

uint16_t __play_valid_cmd_idx() {
    uint16_t idx;
    do {
        idx = __play_rand_cmd_idx();
    } while(cmd_test(idx));

    return idx;
}

void __play_advance(uint8_t sole, uint8_t success) {
    if (!success) {
        play.cmds_progress++;
        cd_advance(&cdcenter, 2.0);
    } else {
        play.cmds_progress--;
    }
    con_send_cmd(&con[sole], &cmds[__play_valid_cmd_idx()], level.cmd_time, play.clock);
}

void play_state_change(uint8_t sole) {
    con_state_change(&con[sole]);

    if (play.PLAYING) {
        uint8_t i;
        for (i = 0; i < 3; i++) {
            if (cmd_test(con[i].last_cmd->index)) {
                __play_advance(i, 1);
            }
        }
    }
}

void __play_loop();

void __play_update_level_begin_packet() {
    play.level_packet_type_2.d2.packet = 2;
    play.level_packet_type_2.d2.theme = level.lab_theme;
    play.level_packet_type_2.d2.mods = level.mod;
    play.level_packet_type_2.d2.argument1 = level.arg_freq;
    play.level_packet_type_2.d2.argument2 = level.arg_shift;


    play.level_packet1.d1.packet = 1;
    play.level_packet1.d1.index1 = level.label[0][0];
    play.level_packet1.d1.index2 = level.label[0][1];
    play.level_packet1.d1.index3 = level.label[0][2];
    play.level_packet1.d1.index4 = level.label[0][3];
    play.level_packet1.d1.index5 = level.label[0][4];
    play.level_packet1.d1.index6 = level.label[0][5];


    play.level_packet2.d1.packet = 1;
    play.level_packet2.d1.index1 = level.label[1][0];
    play.level_packet2.d1.index2 = level.label[1][1];
    play.level_packet2.d1.index3 = level.label[1][2];
    play.level_packet2.d1.index4 = level.label[1][3];
    play.level_packet2.d1.index5 = level.label[1][4];
    play.level_packet2.d1.index6 = level.label[1][5];

    play.level_packet3.d1.packet = 1;
    play.level_packet3.d1.index1 = level.label[2][0];
    play.level_packet3.d1.index2 = level.label[2][1];
    play.level_packet3.d1.index3 = level.label[2][2];
    play.level_packet3.d1.index4 = level.label[2][3];
    play.level_packet3.d1.index5 = level.label[2][4];
    play.level_packet3.d1.index6 = level.label[2][5];

    led_on(&led1);
}

void __play_send_level_begin_packet() {
    // printf("Begin transfers\r\n");
    // printf("Sending theme: %d\r\n", play.level_packet_type_2.d2.theme);
    // printf("Sending mod: %d\r\n", play.level_packet_type_2.d2.mods);
    // printf("Sending arg1: %d\r\n", play.level_packet_type_2.d2.argument1);
    // printf("Sending arg2: %d\r\n", play.level_packet_type_2.d2.argument2);
    con_transfer(&con[0],play.level_packet1);
    con_transfer(&con[0],play.level_packet_type_2);
    con_transfer(&con[1],play.level_packet2);
    con_transfer(&con[1],play.level_packet_type_2);
    con_transfer(&con[2],play.level_packet3);
    con_transfer(&con[2],play.level_packet_type_2);
    // printf("End transfers\r\n");
}


void play_begin() {
    play.timer = &timer1;
    play.clock = 0;
    play.cmds_to_win = 50;
    play.cmds_progress = play.cmds_to_win/2;
    play.success = 0;

    __play_update_level_begin_packet();
    __play_send_level_begin_packet();

    lcd_broadcast(level.message);
    timer_every(play.timer, PLAY_TICK, __play_loop);
    cd_start(&cdcenter, level.level_time, play.clock);
    play.PLAYING = 1;

    uint8_t i;
    for (i = 0; i < 3; i++)
        con_send_cmd(&con[i], &cmds[__play_rand_cmd_idx()], level.cmd_time, play.clock);
}

void __play_end() {
    PLAYING = 0;
    timer_cancel(play.timer);
}


void __play_loop() {
    play.clock++;

    cd_update_all(play.clock);

    // we lost!
    if (cdcenter.flag) {
        cdcenter.flag = 0;
        leds_clear(&ledcenter);
        leds_writeRGBs(&ledcenter, 255,0,0);

        play.success = 0;
        return __play_end();
    }

    uint8_t i;
    for (i = 0; i < 3; i++) {
        if (con[i].cd->flag) {
            con[i].cd->flag = 0;
            return __play_advance(i, 0);
        }
    }

    // we won!
    if (play.cmds_progress >= play.cmds_to_win) {
        play.success = 1;
        return __play_end();
    }
}