#include <p24FJ128GB206.h>
#include "play.h"

#include "rng.h"

_PLAY play;

void play_state_change(uint8_t sole) {
    uint8_t success = con_state_change(&con[sole]);
    if (success) {
        __play_advance(sole, 1);
    }
}

uint16_t __play_rand_cmd_idx() {
    return rng_int(0, GAME_NUM_CMDS);
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

void play_loop() {
    play.clock++;

    cd_update_all(play.clock);

    // we lost!
    if (cdcenter.flag) {
        cdcenter.flag = 0;
        leds_clear(&ledcenter);
        leds_writeRGBs(&ledcenter, 255,0,0);

        return play_end(0);
    }

    uint8_t i;
    for (i = 0; i < 3; i++) {
        if (con[i].flag) {
            con[i].flag = 0;
            return __play_advance(i, 0);
        }
    }

    // we won!
    if (play.cmds_progress >= play.cmds_to_win) {
        return play_end(1);
    }
}

void play_begin() {
    play.timer = timer1;
    play.clock = 0;
    play.cmds_to_win = 50;
    play.cmds_progress = play.cmds_to_win/2;

    timer_every(&play.timer, GAME_TICK, play_loop);
    cd_start(&cdcenter, 240, play.clock);

    uint8_t i;
    for (i = 0; i < 3; i++)
        con_send_cmd(&con[i], &cmds[__play_rand_cmd_idx()], 6, play.clock);

    printf("Begin level play!\r\n");
}

void play_end(uint8_t success) {
    timer_cancel(&play.timer);

    if (success) {
    } else {
    }
}