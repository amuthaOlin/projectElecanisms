/*
** Copyright (c) 2016, Evan Dorsky
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
#include "cmd.h"
#include "spacecomms.h"
#include "ui.h"

_CMD cmds[32];
WORD32 desired_states [32];

char *cmd_strs = {
    "Cat",
    "Dog"
}


uint8_t __log2(uint16_t n) {
    uint8_t r = 0;

    while (n >>= 1)
        r++;

    return r;
}

void init_cmd(void) {
    uint32_t i;
    uint8_t j;

    for (i = 0; i < CONS1_NUMACTS; i++) {
        for (j = 0; j < CONS1_STATES[i]; j++) {
            cmd_init(i, CONS1_STATES[i] == 1?1:j, 1);
        }
    }

    for (i = 0; i < CONS2_NUMACTS; i++) {
        for (j = 0; j < CONS2_STATES[i]; j++) {
            cmd_init(CONS1_NUMACTS+i, CONS2_STATES[i] == 1?1:j, 2);
        }
    }

    for (i = 0; i < CONS3_NUMACTS; i++) {
        for (j = 0; j < CONS3_STATES[i]; j++) {
            cmd_init(CONS1_NUMACTS+CONS2_NUMACTS+i, CONS3_STATES[i] == 1?1:j, 3);
        }
    }
}

uint32_t cmds_ptr = 0;
void cmd_init(uint16_t actuator, uint16_t action, uint8_t console) {
    _CMD cmd_tmp;
    cmd_tmp.actuator = actuator;
    cmd_tmp.action = action;

    cmds[cmds_ptr] = cmd_tmp;

    WORD32 desired = (WORD32)0;

    uint8_t i;
    uint8_t bitpos = 0;
    switch (console) {
        case 1:
            for (i = 0; i < CONS1_NUMACTS) {
                if (i == actuator)
                    desired.l |= action << bitpos;
                bitpos += __log2(CONS1_STATES[i]);
            }
            break;
        case 2:
            for (i = 0; i < CONS2_NUMACTS) {
                if (i == actuator)
                    desired.l |= action << bitpos;
                bitpos += __log2(CONS2_STATES[i]);
            }
            break;
        case 3:
            for (i = 0; i < CONS3_NUMACTS) {
                if (i == actuator)
                    desired.l |= action << bitpos;
                bitpos += __log2(CONS3_STATES[i]);
            }
            break;
    }

    desired_states[cmds_ptr] = desired;

    cmds_ptr++;
};

void cmd_send(uint32_t cmd, float cd_time, _CD *cd);