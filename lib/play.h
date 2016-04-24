#ifndef _PLAY_H_
#define _PLAY_H_

#include <stdint.h>
#include "common.h"

#include "timer.h"

void init_play(void);

typedef struct _PLAY {
    _TIMER *timer;
    volatile int32_t clock;

    uint16_t cmds_to_win;
    uint16_t cmds_progress;
} _PLAY;

extern _PLAY play;
volatile extern int32_t play_clock; // time unit seconds

void play_state_change(uint8_t sole);

void play_begin();
void play_end(uint8_t success);

#endif
