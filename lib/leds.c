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

_LEDS leds;

void __leds_bit_high(_LEDS *self) {
    pin_write(self->pin, LEDS_HIGH);
}

void __leds_bit_low(_LEDS *self) {
    pin_write(self->pin, LEDS_LOW);
}

void __leds_reset(_LEDS *self) {
    pin_write(self->pin, 0x0000);
    timer_delayMicro(7);
}

volatile uint16_t bitcount = 0;
void __attribute__((interrupt, auto_psv)) _OC1Interrupt(void) {
    bitclear(&IFS0, 2);

    // actually write the RGB data
    __leds_bit_high(&leds);

    bitcount++;
    if (bitcount == 24*LED_NUM) {
        bitcount = 0;
        __leds_reset(&leds);
    }
}

void init_leds(void) {
    // must pass OC1 for now
    leds_init(&leds, &D[7], &oc1);
}

void leds_init(_LEDS *self, _PIN *pin, _OC *oc) {
    self->pin = pin;
    self->oc = oc;

    oc_pwm(self->oc, self->pin, NULL, 8e5, 0x0000);
    // OC1 interrupt
    bitset(&IEC0, 2);
}