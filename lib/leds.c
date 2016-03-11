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
#define LEDS_HIGH_R 0x0009 // high word of LEDS_HIGH*OC1RS
#define LEDS_LOW 0x3D70 // ~24% duty cycle
#define LEDS_LOW_R 0x0004 // high word of LEDS_LOW*OC1RS
#define LED_NUM 1
#define LEDS_FREQ 8e5
#define LEDS_PERIOD 20 // cycles for LEDS_FREQ (FCY = 16e6)

_LEDS leds;

uint8_t leds_state[3*LED_NUM];

volatile WORD32 oc1tmp;
void __leds_refresh(_TIMER *timer) {
    led_toggle(&led1);
    oc_pwm(leds.oc, leds.pin, NULL, LEDS_FREQ, LEDS_HIGH);
}

volatile uint8_t bitcount = 0;
void __attribute__((interrupt, auto_psv)) _OC1Interrupt(void) {
    bitclear(&IFS0, 2);

    // if (bitcount%2)
        OC1R = LEDS_HIGH_R;
    // else
        // OC1R = LEDS_LOW_R;

    bitcount++;
    if (bitcount == 3) {
        bitcount = 0;
        OC1R = 0x0000;
        // oc_free(leds.oc); // clears leds.pin
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

    // enable OC1 interrupt
    oc_pwm(leds.oc, leds.pin, NULL, LEDS_FREQ, LEDS_HIGH);
    bitset(&IEC0, 2);

    timer_every(&timer5, 60e-6, __leds_refresh);
}