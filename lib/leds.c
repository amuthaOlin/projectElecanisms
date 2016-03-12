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

#define LEDS_HIGH_R 0x000F // high word of LEDS_HIGH*OC1RS
#define LEDS_LOW_R 0x0001 // high word of LEDS_LOW*OC1RS
#define LEDS_NUM 8
#define LEDS_FREQ 2e5
#define LEDS_PERIOD FCY/(LEDS_FREQ*1.) // cycles for LEDS_FREQ (FCY = 16e6)
#define LEDS_RS_PERIOD 960 // cycles for 60us reset

_LEDS leds;

uint8_t leds_state[3*LEDS_NUM];

volatile uint8_t bitcount = 0;
void __attribute__((interrupt, auto_psv)) _OC1Interrupt(void) {
    bitclear(&IFS0, 2);
    if (!bitcount)
        OC1RS = LEDS_PERIOD;

    OC1R = bitread(&leds_state[(uint16_t)(bitcount/8)], 7-bitcount%8) ? LEDS_HIGH_R : LEDS_LOW_R;

    bitcount++;
    if (bitcount == 24*LEDS_NUM+1) {
        OC1RS = LEDS_RS_PERIOD;
        OC1R = 0x0000;
        bitcount = 0;
    }
}

void init_leds(void) {
    leds_init(&leds, &A[5], &oc1, &timer5);
}

volatile uint8_t bounce_led = 0;
volatile int8_t bounce_dir = 1;
volatile uint8_t bounce_r = 0;
volatile uint8_t bounce_g = 0;
volatile uint8_t bounce_b = 0;
void __leds_bounce_write(_TIMER *timer) {
    leds_clear(&leds);
    leds_writeRGB(&leds, bounce_led, bounce_r,bounce_g,bounce_b);
    bounce_led += bounce_dir;
    if (bounce_led == LEDS_NUM-1)
        bounce_dir = -1;
    if (bounce_led < 1)
        bounce_dir = 1;
}

void leds_bounce(_LEDS *self, float period, uint8_t red, uint8_t green, uint8_t blue) {
    bounce_r = red;
    bounce_g = green;
    bounce_b = blue;
    timer_every(self->timer, period, __leds_bounce_write);
}

void leds_writeRGBs(_LEDS *self, uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t i;
    for (i = 0; i < LEDS_NUM; i++)
        leds_writeRGB(self, i, red,green,blue);
}

volatile uint8_t cycle_w = 0;
void __leds_cycle(_LEDS *self) {
    leds_writeRGB(self, 4, cycle_w,cycle_w,cycle_w);
    cycle_w++;
}

volatile uint8_t bar_r = 0;
volatile uint8_t bar_g = 0;
volatile uint8_t bar_b = 0;
void leds_bar(_LEDS *self, float fill, uint8_t brightness) {
    uint8_t leds_lit = fill*LEDS_NUM;
    uint8_t i;

    bar_g = fill*255;
    bar_r = (1-fill)*255;

    leds_clear(&leds);
    for (i = 0; i <= leds_lit; i++) {
        leds_writeRGB(self, i, bar_r,bar_g,bar_b);
    }
    leds_brighten(self, i-1, fill*LEDS_NUM-leds_lit);
}

void leds_clear(_LEDS *self) {
    leds_writeRGBs(self, 0,0,0);
}

void leds_writeRGB(_LEDS *self, uint8_t led, uint8_t red, uint8_t green, uint8_t blue) {
    leds_state[3*led] = green;
    leds_state[3*led+1] = red;
    leds_state[3*led+2] = blue;
}

void leds_brighten(_LEDS *self, uint8_t led, float factor) {
    leds_state[3*led] *= factor;
    leds_state[3*led+1] *= factor;
    leds_state[3*led+2] *= factor;
}

void leds_writeWhite(_LEDS *self, uint8_t led, uint8_t brightness) {
    leds_state[3*led] = brightness;
    leds_state[3*led+1] = brightness;
    leds_state[3*led+2] = brightness;
}

void leds_init(_LEDS *self, _PIN *pin, _OC *oc, _TIMER *timer) {
    self->pin = pin;
    self->oc = oc;
    self->timer = timer;

    oc_pwm(&oc1, self->pin, NULL, LEDS_FREQ, 0x0000);
    bitset(&IEC0, 2);
}