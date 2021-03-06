#include <p24FJ128GB206.h>
#include "play.h"

#include "rng.h"
#include "con.h"
#include "lev.h"
#include "ui.h"
#include "lcd.h"
#include "spacecomms.h"

#define PLAY_NUM_CMDS 89
#define PLAY_TICK 1e-2 // seconds

_PLAY play;

uint16_t __play_rand_cmd_idx() {
    uint8_t act = rng_int(0, 17);
    uint8_t cons = act/6;
    uint8_t group = act%6;

    if (group == 5 && cons == 0) { // word wheel
        uint8_t groupidx = cmd_groupidx(0, 10);
        return rng_int(groupidx, groupidx+35);
    } else {
        uint8_t groupidx = cmd_groupidx(cons, group);
        uint8_t groupcmds = cmd_groupcount(cons, group);
        return rng_int(groupidx, groupidx+groupcmds - 1);
    }
}

volatile uint16_t cmd_i = 0;
uint16_t __play_valid_cmd_idx() {
    uint16_t idx = __play_rand_cmd_idx();
    while (cmd_test(idx)) {
        idx = __play_rand_cmd_idx();
    }

    return idx;

    //CODE TO TEST EACH ACTUATOR
    // uint16_t temp = cmd_i;
    // cmd_i++;
    // cmd_i = cmd_i%PLAY_NUM_CMDS;
    // return temp;
}

void __play_advance(uint8_t sole, uint8_t success) {
    if (!success) {
        if (play.WORMHOLE == sole+1) {
            play.cmds_progress -= 3;
            play.WORMHOLE = 0;
        } else {
            play.cmds_progress+=5; /// CHANGED
        }
    } else {
        play.cmds_progress++;
    }
    if (play.cmds_progress == 22) {
        play.WORMHOLE = sole+1;
        con_send_cmd(&con[sole], &cmd_wormhole, level.cmd_time, play.clock);
    } else {
        con_send_cmd(&con[sole], &cmds[__play_valid_cmd_idx()], level.cmd_time, play.clock);
    }
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
        uint8_t wormhole_evaded = con[0].state.s1.wormhole1 && con[0].state.s1.wormhole2 && con[1].state.s2.wormhole1 && con[1].state.s2.wormhole2 && con[2].state.s3.wormhole1 && con[2].state.s3.wormhole2;
        if (play.WORMHOLE && wormhole_evaded) {
            play.WORMHOLE = 0;
            __play_advance(play.WORMHOLE-1, 1);
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
    // timer_delayMicro(50000);
    con_transfer(&con[0],play.level_packet1);
    con_transfer(&con[0],play.level_packet_type_2);
    // timer_delayMicro(50000);
    con_transfer(&con[1],play.level_packet2);
    con_transfer(&con[1],play.level_packet_type_2);
    // timer_delayMicro(50000);
    con_transfer(&con[2],play.level_packet3);
    con_transfer(&con[2],play.level_packet_type_2);
    // printf("End transfers\r\n");
}


void play_begin() {
    play.timer = &timer1;
    play.clock = 0;
    play.cmds_to_win = 28;
    play.cmds_progress = play.cmds_to_win/2;
    play.success = 0;

    __play_update_level_begin_packet();
    uint8_t i;
    for (i = 0; i < 3; i++) {
        __play_send_level_begin_packet();
    }

    if (level.message) {
        lcd_broadcast(level.message);
        for (i = 0; i < 20; i++) {
            timer_delayMicro(50000);
        }
    }

    timer_every(play.timer, PLAY_TICK, __play_loop);
    cd_start(&cdcenter, level.level_time, play.clock);
    play.PLAYING = 1;
    play.WORMHOLE = 0;

    for (i = 0; i < 3; i++)
        con_send_cmd(&con[i], &cmds[__play_rand_cmd_idx()], level.cmd_time, play.clock);
}

void __play_end() {
    play.PLAYING = 0;
    timer_cancel(play.timer);
    leds_clear(&ledbar1);
    leds_clear(&ledbar2);
    leds_clear(&ledbar3);
}

void __play_loop() {
    play.clock++;

    cd_update_all(play.clock);
    leds_centerDisplay(&ledcenter, cdcenter.percent_done, play.cmds_progress/(float)play.cmds_to_win);

    // printf("Percent done: %f\r\n", cdcenter.percent_done);
    if (cdcenter.percent_done > play.cmds_progress/(float)play.cmds_to_win) {
        // we lost!
        cdcenter.flag = 0;
        leds_clear(&ledcenter);

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