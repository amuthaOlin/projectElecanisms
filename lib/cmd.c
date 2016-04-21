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
#include "strm.h"
#include "lev.h"

// CMD_COUNT is cumsum of members of state arrays minus cumsum of members of hasrest arrays
#define CMD_COUNT 100 // 97 (100 to be safe)
_CMD cmds[CMD_COUNT];
char numbers[11][7]={"Zero","One","Two","Three","Four","Five","Six","Seven","Eight","Nine","Ten"};

uint16_t __cmd_log2(uint16_t n) {
    return n<2? 1:ceil(log((double)n)/log(2.));
}

void init_cmd(void) {
    uint16_t i, j, k;

    for (k = 0; k < 3; k++) { // k for konsole
        for (i = 0; i < CONS_NUMACTS[k]; i++) {
            if (!CONS_HASREST[k][i])
                cmd_init(i, 0, k);
            for (j = 1; j < CONS_STATES[k][i]; j++) {
                cmd_init(i, j, k);
            }
        }
    }
}

uint16_t cmds_ptr = 0;
void cmd_init(uint16_t actuator, uint16_t action, uint8_t console) {
    WORD32 mask = (WORD32)0;
    WORD32 desired = (WORD32)0;

    uint16_t i;
    uint16_t bitpos = 0;
    uint16_t logres = 0;

    for (i = 0; i < CONS_NUMACTS[console]; i++) {
        logres = __cmd_log2(CONS_STATES[console][i]);
        if (i == actuator) {
            desired.ul |= (uint32_t)action << bitpos;
            mask.ul |= ((uint32_t)pow(2, logres)-1) << bitpos;
        }
        bitpos += logres;
    }

    _CMD cmd_tmp;
    cmd_tmp.index = cmds_ptr;
    cmd_tmp.actuator = actuator;
    cmd_tmp.action = action;
    cmd_tmp.console = console;
    cmd_tmp.desired = desired;
    cmd_tmp.mask = mask;
    cmds[cmds_ptr] = cmd_tmp;

    cmds_ptr++;
}

uint16_t cmd_get(uint8_t console, uint16_t actuator, uint16_t action) {
    uint16_t i;
    _CMD *cmd_search;
    // "O(n) time"
    for (i = 0; i < CMD_COUNT; i++) {
        cmd_search = &cmds[i];
        if (cmd_search->console == console &&
            cmd_search->actuator == actuator &&
            cmd_search->action == action) {
            return i;
        }
    }
    return -1; // invalid command
}

void cmd_print(uint16_t index) {
    _CMD *cmd = &cmds[index];

    printf("=======\r\n");
    printf("Command for console %d\r\n", cmd->console+1);
    printf("-------\r\n");
    printf("%s\r\n", cmd->command);
    printf("-------\r\n");
    printf("Actuator %d, action %d\r\n", cmd->actuator, cmd->action);
    printf("Desired bits: %08lx\r\n", (unsigned long)cmd->desired.ul);
    printf("Mask bits   : %08lx\r\n", (unsigned long)cmd->mask.ul);
}

void cmd_str(uint16_t cmdidx, _LEV *level) { // assume str is 16 char long
    _CMD *cmd = &cmds[cmdidx];

    if (CONS_HASREST[cmd->console][cmd->actuator] && CONS_STATES[cmd->console][cmd->actuator] == 2) {
        strm_genPush(cmd->command,lev_getName(level,cmd->console,cmd->actuator));
    } else {
        strm_genSet(cmd->command,lev_getName(level,cmd->console,cmd->actuator),numbers[cmd->action]);
    }
    // strcpy(cmd->command, "Do something!");
}

WORD32 cmd_packet(uint16_t cmdidx) {
    _CMD *cmd = &cmds[cmdidx];
    WORD32 dir = (WORD32)0;

    dir.d.packet = 0; // type for "command"
    dir.d.actaddr = cmd->actuator;
    dir.d.actact = cmd->action;

    return dir;
}

// returns 1 if command passes
uint8_t cmd_test(uint16_t cmdidx, WORD32 state) {
    _CMD *cmd = &cmds[cmdidx];

    return (state.ul & cmd->mask.ul) == cmd->desired.ul;
}