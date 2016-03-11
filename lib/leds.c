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
#include "leds.h"
#include "ui.h"
#include "oc.h"
#include "timer.h"

#define LEDS_HIGH 0x7AE1 // ~48% duty cycle
#define LEDS_LOW 0x3D70 // ~24% duty cycle
#define LED_NUM 8
#define LEDS_FREQ 8e5

_LEDS leds;

uint8_t leds_state[3*LED_NUM];

volatile int16_t bitcount = 0;
void __attribute__((interrupt, auto_psv)) _OC1Interrupt(void) {
    bitclear(&IFS0, 2);
    if (!bitcount)
        oc_freq(leds.oc, LEDS_FREQ);

    pin_write(leds.pin, bitread(&leds_state[(uint16_t)(bitcount/8)], 5)? LEDS_LOW : LEDS_HIGH);

    bitcount++;
    if (bitcount == 24*LED_NUM-1) {
        bitcount = 0;
        oc_freq(leds.oc, 16666);
        pin_write(leds.pin, 0);
    }
}

void init_leds(void) {
    // must pass OC1 for now
    leds_init(&leds, &A[5], &oc1);
}

void leds_writeOne(_LEDS *self, uint8_t led, uint8_t red, uint8_t green, uint8_t blue) {
    leds_state[3*led] = green;
    leds_state[3*led+1] = red;
    leds_state[3*led+2] = blue;
}

void leds_init(_LEDS *self, _PIN *pin, _OC *oc) {
    self->pin = pin;
    self->oc = oc;

    oc_pwm(self->oc, self->pin, NULL, LEDS_FREQ, 0x0000);
    // enable OC1 interrupt
    bitset(&IEC0, 2);

    uint8_t i;
    // for (i = 0; i < LED_NUM; i++) {
    // leds_writeOne(self, 2, 255,255,255);
    // }
}