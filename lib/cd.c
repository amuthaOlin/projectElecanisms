/*
** Copyright (c) 2013, Evan Dorsky
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met: 
** 
**     1. Redistributions of source code must retain the above copyright 
**        notice, this list of conditions and the following disclaimer. 
**     2. Redistributions in binary form must reproduce the above copyright 
**        notice, this list of conditions and the following disclaimer in the 
**        documentation and/or other materials provided with the distribution. 
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
** POSSIBILITY OF SUCH DAMAGE.
*/
#include <p24FJ128GB206.h>
#include "common.h"
#include "cd.h"

_CD cd1, cd2, cd3;

void init_cd(void) {
    cd_init(&cd1, 1e-3, &ledbar1);
    cd_init(&cd2, 1e-3, &ledbar2);
    cd_init(&cd3, 1e-3, &ledbar3);
}

void cd_init(_CD *self, float step_sec, _LEDS *ledbar) {
    self->step_sec = step_sec;
    self->flag = 0;
    self->ledbar = ledbar;
    self->active = 0;
}

void cd_start(_CD *self, float dur_sec, uint16_t ticks_start) {
    self->flag = 0;
    self->dur_sec = dur_sec;

    self->ticks_dur = (uint16_t)(dur_sec/self->step_sec);
    self->ticks_start = ticks_start;

    self->active = 1;
}

void cd_update(_CD *self, uint16_t ticks_cur) {
    if (!self->active) return;
    
    uint16_t ticks_consumed = ticks_cur - self->ticks_start;
    if (ticks_consumed > self->ticks_dur) {
        self->flag = 1;
    }

    leds_bar(&ledbar1, (float)ticks_consumed/self->ticks_dur, 1);
}

void cd_update_all(uint32_t ticks_cur) {
    cd_update(&cd1, ticks_cur);
    cd_update(&cd2, ticks_cur);
    cd_update(&cd3, ticks_cur);
}