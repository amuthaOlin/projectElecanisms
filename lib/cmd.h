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
#ifndef _CMD_H_
#define _CMD_H_

#include <stdint.h>
#include "cd.h"
#include "lev.h"
#include "strm.h"

void init_cmd(void);

typedef struct _CMD {
    // constant
    uint16_t index;
    uint8_t console;
    uint16_t actuator;
    uint16_t action;
    uint8_t group;
    uint8_t states;
    uint8_t hasrest;

    WORD32 mask;
    WORD32 desired;

    // dynamic
    float cd_time;
    char name[17];
    char command[33];
    _CD *cd;
} _CMD;

extern _CMD cmds[];
extern char cmd_strs[][33];
extern _CMD cmd_wormhole;

void cmd_init(uint16_t actuator, uint16_t action, uint8_t console);
void cmd_str(uint16_t cmdidx);
void cmd_print(uint16_t index);

uint16_t cmd_get(uint8_t console, uint16_t actuator, uint16_t action);
uint8_t cmd_test(uint16_t cmdidx);

#endif
