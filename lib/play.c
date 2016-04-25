#include <p24FJ128GB206.h>
#include "play.h"

#include "rng.h"
#include "con.h"
#include "lev.h"
#include "lcd.h"

#define PLAY_NUM_CMDS 90
#define PLAY_TICK 1e-2 // seconds

volatile uint8_t PLAYING = 0;

_PLAY play;

uint16_t __play_rand_cmd_idx() {
    return rng_int(0, PLAY_NUM_CMDS);
}


void __play_advance(uint8_t sole, uint8_t success) {
    if (!success) {
        play.cmds_progress++;
        cd_advance(&cdcenter, 2.0);
    } else {
        play.cmds_progress--;
        con_send_cmd(&con[sole], &cmds[__play_rand_cmd_idx()], 6, play.clock);
    }
}

void play_state_change(uint8_t sole) {
    uint8_t success = con_state_change(&con[sole]);
    if (success) {
        __play_advance(sole, 1);
    }
}


void __play_loop();

void __play_update_level_begin_packet(){
    play.level_packet_type_2.d2.packet = 2;
    play.level_packet_type_2.d2.theme = level.lab_theme;
    play.level_packet_type_2.d2.mods = level.mod;
    play.level_packet_type_2.d2.argument1 = level.arg_freq;
    play.level_packet_type_2.d2.argument2 = level.arg_shift;


    play.level_packet1.d1.packet = 1;
    play.level_packet1.d1.index1 = level.label_name1[0];
    play.level_packet1.d1.index2 = level.label_name1[1];
    play.level_packet1.d1.index3 = level.label_name1[2];
    play.level_packet1.d1.index4 = level.label_name1[3];
    play.level_packet1.d1.index5 = level.label_name1[4];
    play.level_packet1.d1.index6 = level.label_name1[5];


    play.level_packet2.d1.packet = 1;
    play.level_packet2.d1.index1 = level.label_name2[0];
    play.level_packet2.d1.index2 = level.label_name2[1];
    play.level_packet2.d1.index3 = level.label_name2[2];
    play.level_packet2.d1.index4 = level.label_name2[3];
    play.level_packet2.d1.index5 = level.label_name2[4];
    play.level_packet2.d1.index6 = level.label_name2[5];

    play.level_packet3.d1.packet = 1;
    play.level_packet3.d1.index1 = level.label_name3[0];
    play.level_packet3.d1.index2 = level.label_name3[1];
    play.level_packet3.d1.index3 = level.label_name3[2];
    play.level_packet3.d1.index4 = level.label_name3[3];
    play.level_packet3.d1.index5 = level.label_name3[4];
    play.level_packet3.d1.index6 = level.label_name3[5];

}

void __play_send_level_begin_packet(){
    con_transfer(&con[0],play.level_packet1);
    con_transfer(&con[0],play.level_packet_type_2);
    con_transfer(&con[1],play.level_packet2);
    con_transfer(&con[1],play.level_packet_type_2);
    con_transfer(&con[2],play.level_packet3);
    con_transfer(&con[2],play.level_packet_type_2);
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
    cd_start(&cdcenter, level.cmd_time, play.clock);

    uint8_t i;
    for (i = 0; i < 3; i++)
        con_send_cmd(&con[i], &cmds[__play_rand_cmd_idx()], 6, play.clock);

    printf("Begin level play!\r\n");
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