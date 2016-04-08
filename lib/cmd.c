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
#include "math.h"
#include "cmd.h"
#include "spacecomms.h"
#include "ui.h"

_CMD cmds[65]; // cumsum of members of state arrays

char *cmd_strs = {
    "Cat",
    "Dog"
};

uint16_t __log2(uint16_t n) {
    return n<2? 1:ceil(log((double)n)/log(2.));
}

void init_cmd(void) {
    uint16_t i;
    uint8_t j;

    for (i = 0; i < CONS1_NUMACTS; i++) {
        if (!CONS1_HASREST[i])
            cmd_init(i, 0, 1);
        for (j = 1; j < CONS1_STATES[i]; j++) {
            cmd_init(i, j, 1);
        }
    }

    for (i = 0; i < CONS2_NUMACTS; i++) {
        if (!CONS2_HASREST[i])
            cmd_init(i, 0, 1);
        for (j = 1; j < CONS2_STATES[i]; j++) {
            cmd_init(CONS1_NUMACTS+i, j, 2);
        }
    }

    for (i = 0; i < CONS3_NUMACTS; i++) {
        if (!CONS3_HASREST[i])
            cmd_init(i, 0, 1);
        for (j = 1; j < CONS3_STATES[i]; j++) {
            cmd_init(CONS1_NUMACTS+CONS2_NUMACTS+i, j, 3);
        }
    }
}

uint16_t cmds_ptr = 0;
void cmd_init(uint16_t actuator, uint16_t action, uint8_t console) {
    _CMD cmd_tmp;
    cmd_tmp.actuator = actuator;
    cmd_tmp.action = action;

    WORD32 mask = (WORD32)0;
    WORD32 desired = (WORD32)0;

    uint16_t i;
    uint16_t bitpos = 0;
    uint16_t logres = 0;
    switch (console) {
        case 1:
            for (i = 0; i < CONS1_NUMACTS; i++) {
                logres = __log2(CONS1_STATES[i]);
                if (i == actuator) {
                    desired.ul |= (uint32_t)action << bitpos;
                    mask.ul |= ((uint32_t)pow(2, logres)-1) << bitpos;
                }
                bitpos += logres;
            }
            break;
        case 2:
            for (i = 0; i < CONS2_NUMACTS; i++) {
                logres = __log2(CONS2_STATES[i]);
                if (i == actuator-CONS1_NUMACTS) {
                    desired.ul |= (uint32_t)action << bitpos;
                    mask.ul |= ((uint32_t)pow(2, logres)-1) << bitpos;
                }
                bitpos += logres;
            }
            break;
        case 3:
            for (i = 0; i < CONS3_NUMACTS; i++) {
                logres = __log2(CONS3_STATES[i]);
                if (i == actuator-CONS1_NUMACTS-CONS2_NUMACTS) {
                    desired.ul |= (uint32_t)action << bitpos;
                    mask.ul |= ((uint32_t)pow(2, logres)-1) << bitpos;
                }
                bitpos += logres;
            }
            break;
    }
    cmd_tmp.desired = desired;
    cmd_tmp.mask = mask;
    cmds[cmds_ptr] = cmd_tmp;

    cmds_ptr++;
}

void cmd_print(uint16_t index) {
    printf("=======\r\n");
    printf("Command\r\n");
    printf("-------\r\n");
    printf("Actuator: %d\r\n", cmds[index].actuator);
    printf("Action: %d\r\n", cmds[index].action);
    printf("Desired bits: %04lx\r\n", (unsigned long)cmds[index].desired.ul);
    printf("Mask bits   : %04lx\r\n", (unsigned long)cmds[index].mask.ul);
}

void cmd_send(uint16_t cmd, float cd_time, _CD *cd);