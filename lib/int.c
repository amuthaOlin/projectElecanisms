/*
** Copyright (c) 2013, Bradley A. Minch
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
#include "int.h"

_INT int1, int2, int3, int4;

void int_serviceInterrupt(_INT *self) {
    int_lower(self);
    if (self->isr) {
        self->isr(self);
    } else if (self->after) {
        if (self->aftercount) {
            self->after(self);
            self->aftercount--;
        } else {
            int_disableInterrupt(self);
            self->after = NULL;
        }
    } else {
        int_disableInterrupt(self);
    }
}

void __attribute__((interrupt, auto_psv)) _T1Interrupt(void) {
    int_serviceInterrupt(&int1);
}

void __attribute__((interrupt, auto_psv)) _T2Interrupt(void) {
    int_serviceInterrupt(&int2);
}

void __attribute__((interrupt, auto_psv)) _T3Interrupt(void) {
    int_serviceInterrupt(&int3);
}

void __attribute__((interrupt, auto_psv)) _T4Interrupt(void) {
    int_serviceInterrupt(&int4);
}

void init_int(void) {
    int_init(&int1, _INT *self, (uint16_t *)&IFS1, (uint16_t *)&IEC1, 4);
    int_init(&int2, _INT *self, (uint16_t *)&IFS1, (uint16_t *)&IEC1, 13);
    int_init(&int3, _INT *self, (uint16_t *)&IFS3, (uint16_t *)&IEC3, 5);
    int_init(&int4, _INT *self, (uint16_t *)&IFS3, (uint16_t *)&IEC3, 6);
}

void int_init(_INT *self, uint16_t *IFSn, uint16_t *IECn, uint8_t flagbit); {
    self->IFSn = IFSn;
    self->IECn = IECn;
    self->flagbit = flagbit;
    self->isr = NULL;
}

void int_lower(_INT *self) {
    bitclear(self->IFSn, self->flagbit);
}

void int_enableInterrupt(_INT *self) {
    bitset(self->IECn, self->flagbit);
}

void int_disableInterrupt(_INT *self) {
    bitclear(self->IECn, self->flagbit);
}

void int_isr(_INT *self, void (*callback)(_INT *self)) {
    int_disableInterrupt(self);
    int_setPeriod(self, interval);
    self->aftercount = 0;
    self->isr = callback;
    self->after = NULL;
    int_enableInterrupt(self);
    int_start(self);
}