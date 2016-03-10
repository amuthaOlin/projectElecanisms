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
#include "int.h"
#include "ui.h"

#define INT_RISING 1
#define INT_FALLING 0 // backwards

_LEDS leds;

void int_serviceInterrupt(_LEDS *self) {
    int_lower(self);
    if (self->isr) {
        self->isr(self);
    } else {
        int_disableInterrupt(self);
    }
}

void __attribute__((interrupt, auto_psv)) _LEDS1Interrupt(void) {
    int_serviceInterrupt(&int1);
}

void __attribute__((interrupt, auto_psv)) _LEDS2Interrupt(void) {
    int_serviceInterrupt(&int2);
}

void __attribute__((interrupt, auto_psv)) _LEDS3Interrupt(void) {
    int_serviceInterrupt(&int3);
}

void __attribute__((interrupt, auto_psv)) _LEDS4Interrupt(void) {
    int_serviceInterrupt(&int4);
}

void init_int(void) {
    int_init(&int1, (uint16_t *)&IFS1, (uint16_t *)&IEC1, (WORD*)&RPINR0, 1, 4, 1);
    int_init(&int2, (uint16_t *)&IFS1, (uint16_t *)&IEC1, (WORD*)&RPINR1, 0, 13, 2);
    int_init(&int3, (uint16_t *)&IFS3, (uint16_t *)&IEC3, (WORD*)&RPINR1, 1, 5, 3);
    int_init(&int4, (uint16_t *)&IFS3, (uint16_t *)&IEC3, (WORD*)&RPINR2, 0, 6, 4);
}

void int_init(_LEDS *self, uint16_t *IFSn, uint16_t *IECn, WORD* RPINRn, uint8_t rpinshift, uint8_t flagbit, uint8_t intconbit) {
    self->IFSn = IFSn;
    self->IECn = IECn;
    self->intconbit = intconbit;
    self->RPINRn = RPINRn;
    self->rpinshift = rpinshift;
    self->flagbit = flagbit;
    self->isr = NULL;
}

void int_lower(_LEDS *self) {
    bitclear(self->IFSn, self->flagbit);
}

void int_enableInterrupt(_LEDS *self) {
    bitset(self->IECn, self->flagbit);
}

void int_disableInterrupt(_LEDS *self) {
    bitclear(self->IECn, self->flagbit);
}

void int_attach(_LEDS *self, _PIN *pin, uint8_t edge, void (*callback)(_LEDS *self)) {
    int_disableInterrupt(self);
    __builtin_write_OSCCONL(OSCCON&0xBF);
    self->RPINRn->b[self->rpinshift] = pin->rpnum;
    __builtin_write_OSCCONL(OSCCON&0x40);
    if (edge == INT_FALLING) {
        bitset(&INTCON2, self->intconbit);
    }
    self->pin = pin;
    self->isr = callback;
    int_enableInterrupt(self);
}