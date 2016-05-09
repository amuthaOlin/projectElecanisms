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
#include "con.h"
#include "spacecomms.h"
#include "ui.h"
#include "strm.h"
#include "lev.h"

char words_outer[6][10] = {"Ignite", "Punch", "Charge", "Poke", "Munch", "Soak"};
char words_inner[6][10] = {" Ratchet", " Bobbin", " Quark", " Sucker", " Rotor", " Uffler"};

// CMD_COUNT is cumsum of members of state arrays minus cumsum of members of hasrest arrays
#define CMD_COUNT 120 // 97-9 (120 to be safe)
_CMD cmds[CMD_COUNT];
_CMD cmds_special[9];
_CMD cmd_wormhole;

uint16_t log_reses[] = {
    1, // 0
    1, // 1
    1, // 2
    2, // 3
    2, // 4
    3, // 5
    3, // 6
    3, // 7
    4, // 8
    4, // 9
    4, // 10
    4, // 11
    4, // 12
    4, // 13
    4, // 14
    4, // 15
};
uint16_t __cmd_log2(uint16_t n) {
    return log_reses[n];
}

char worm_str[33] = "WORMHOLE        INCOMING!";
uint16_t cmds_ptr = 0;
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

    uint8_t word_outer = cmd_get(0, 7, 0);
    uint8_t word_inner = cmd_get(0, 6, 0);

    // word wheel commands -- go at the end of the command list
    k = 0;
    for (i = word_outer; i < word_outer + 6; i++) {
        for (j = word_inner; j < word_inner + 6; j++) {
            _CMD wordcmd_tmp;
            wordcmd_tmp.console = 0;
            wordcmd_tmp.index = cmds_ptr;
            wordcmd_tmp.actuator = 0;
            wordcmd_tmp.group = 10; // special word wheel group
            wordcmd_tmp.states = 36;
            wordcmd_tmp.hasrest = 0;
            wordcmd_tmp.action = k;
            wordcmd_tmp.desired = (WORD32)(cmds[i].desired.ul | cmds[j].desired.ul);
            wordcmd_tmp.mask = (WORD32)(cmds[i].mask.ul | cmds[j].mask.ul);

            strcpy(wordcmd_tmp.command, words_outer[i-word_outer]);
            strcat(wordcmd_tmp.command, words_inner[j-word_inner]);
            k++;

            cmds[cmds_ptr] = wordcmd_tmp;
            cmds_ptr++;
        }
    }

    cmd_wormhole.console = 6;
    cmd_wormhole.index = 0;
    cmd_wormhole.actuator = 0;
    cmd_wormhole.group = 7;
    cmd_wormhole.states = 0;
    cmd_wormhole.hasrest = 0;
    cmd_wormhole.action = 0;
    cmd_wormhole.desired = (WORD32)0xFFFFFFFF;
    cmd_wormhole.mask = (WORD32)0xFFFFFFFF;
    strcpy(cmd_wormhole.command, worm_str);
}

uint16_t cmds_special_ptr = 0;
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
    cmd_tmp.actuator = actuator;
    cmd_tmp.action = action;
    cmd_tmp.console = console;
    cmd_tmp.desired = desired;
    cmd_tmp.mask = mask;
    cmd_tmp.group = CONS_GROUP[console][actuator];
    cmd_tmp.states = CONS_STATES[console][actuator];
    cmd_tmp.hasrest = CONS_HASREST[console][actuator];

    if (CONS_GROUP[console][actuator] > 5) {// special
        cmd_tmp.index = cmds_special_ptr;
        cmds_special[cmds_special_ptr] = cmd_tmp;
        cmds_special_ptr++;
    } else {
        cmd_tmp.index = cmds_ptr;
        cmds[cmds_ptr] = cmd_tmp;
        cmds_ptr++;
    }
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

// index of the first command in a given console group
uint16_t cmd_groupidx(uint8_t console, uint8_t group) {
    uint16_t i;
    _CMD *cmd_search;
    // "O(n) time"
    for (i = 0; i < CMD_COUNT; i++) {
        cmd_search = &cmds[i];
        if (cmd_search->console == console &&
            cmd_search->group == group) {
            return i;
        }
    }
    return -1; // invalid group (?)
}

uint16_t cmd_groupcount(uint8_t console, uint8_t group) {
    uint16_t i;
    _CMD *cmd_search;
    // "O(n) time"
    uint8_t ingroup = 0;
    for (i = 0; i < CMD_COUNT; i++) {
        cmd_search = &cmds[i];
        if (cmd_search->console == console &&
            cmd_search->group == group) {
            ingroup++;
        }
    }
    return ingroup;
}

void cmd_print(uint16_t index) {
    _CMD *cmd = &cmds[index];

    printf("=======\r\n");
    printf("Command for console %d\r\n", cmd->console+1);
    printf("-------\r\n");
    printf("%s\r\n", cmd->command);
    printf("-------\r\n");
    printf("Actuator %d, action %d\r\n", cmd->actuator, cmd->action);
    printf("Group %d\r\n", cmd->group);
    printf("Desired bits: %08lx\r\n", (unsigned long)cmd->desired.ul);
    printf("Mask bits   : %08lx\r\n", (unsigned long)cmd->mask.ul);
}

void cmd_str(uint16_t cmdidx) { // assume str is 16 char long
    _CMD *cmd = &cmds[cmdidx];

    if (CONS_HASREST[cmd->console][cmd->actuator] && CONS_STATES[cmd->console][cmd->actuator] == 2) {
        strm_genPush(cmd->command,cmd->name,CONS_GROUPNUM[cmd->console][cmd->actuator]);
    } else if (CONS_STATES[cmd->console][cmd->actuator] == 2) {
        strm_genAct(cmd->command,cmd->name,CONS_GROUPNUM[cmd->console][cmd->actuator],cmd->action);
    } else if (!CONS_HASREST[cmd->console][cmd->actuator] && CONS_STATES[cmd->console][cmd->actuator] > 2) {
        strm_genSet(cmd->command,cmd->name,cmd->action+1);
    } else {
        strm_genSet(cmd->command,cmd->name,cmd->action);
    }
}

// returns 1 if command passes
uint8_t __cmd_compare(uint16_t cmdidx, WORD32 state) {
    _CMD *cmd = &cmds[cmdidx];

    return (state.ul & cmd->mask.ul) == cmd->desired.ul;
}

uint8_t cmd_test(uint16_t cmdidx) {
    // check this command against the state of the console it's for
    // cmd_print(cmdidx);
    return __cmd_compare(cmdidx, con[cmds[cmdidx].console].state);
}