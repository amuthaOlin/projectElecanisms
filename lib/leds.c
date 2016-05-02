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
#include "rng.h"
#include "timer.h"

#define LEDS_HIGH_R 0x000F // high word of LEDS_HIGH*OC1RS
#define LEDS_LOW_R 0x0001 // high word of LEDS_LOW*OC1RS
#define LEDS_NUM 84 // 8*3+60
#define LEDS_FREQ 2e5

_LEDS ledbar1, ledbar2, ledbar3, ledcenter;

uint8_t leds_state[3*LEDS_NUM];

volatile uint16_t bitcount = 0;
void __attribute__((interrupt, auto_psv)) _OC1Interrupt(void) {
    disable_interrupts();
    bitclear(&IFS0, 2);

    OC1R = bitread(&leds_state[(uint16_t)(bitcount/8)], 7-bitcount%8) ? LEDS_HIGH_R : LEDS_LOW_R;

    bitcount++;
    if (bitcount == 24*LEDS_NUM+1) {
        OC1R = 0;
        bitclear(&IEC0, 2); // disable OC1 interrupt
        oc_free(&oc1);
        bitcount = 0;
    }
    enable_interrupts();
}

void __leds_update(_TIMER *timer) {
    oc_pwm(&oc1, &A[5], NULL, LEDS_FREQ, 0x0000);
    bitset(&IEC0, 2); // enable OC1 interrupt
}

void init_leds(void) { // init the objects and set up the unified controller
    uint8_t i;
    for (i = 0; i < LEDS_NUM; i++) {
        leds_state[i] = 0;
    }
    leds_init(&ledbar1, 8, 0);
    leds_init(&ledbar2, 8, 8);
    leds_init(&ledbar3, 8, 16);
    leds_init(&ledcenter, 60, 24);

    IPC0 |= 0x0700; // OC1 interrupt highest priority
    timer_every(&timer4, 0.035, __leds_update);
}

void leds_init(_LEDS *self, uint16_t num, uint16_t stateptr) {
    self->num = num;
    self->stateptr = stateptr;
}

// volatile uint8_t bounce_led = 0;
// volatile int8_t bounce_dir = 1;
// volatile uint8_t bounce_r = 0;
// volatile uint8_t bounce_g = 0;
// volatile uint8_t bounce_b = 0;
// void __leds_bounce_write(_TIMER *timer) {
//     leds_clear(&leds);
//     leds_writeRGB(&leds, bounce_led, bounce_r,bounce_g,bounce_b);
//     bounce_led += bounce_dir;
//     if (bounce_led == LEDS_NUM-1)
//         bounce_dir = -1;
//     if (bounce_led < 1)
//         bounce_dir = 1;
// }

void leds_writeRGBs(_LEDS *self, uint8_t red, uint8_t green, uint8_t blue) {
    uint8_t i;
    for (i = 0; i < self->num; i++)
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
void leds_bar(_LEDS *self, float fill, float bri) {
    if (fill > 1) fill = 1;
    if (fill < 0) return leds_clear(self);

    uint16_t leds_lit = fill*self->num;
    uint8_t i;

    bar_g = fill*255;
    bar_r = (1-fill)*255;

    // clear all non-lit LEDs
    for (i = leds_lit+2; i < self->num; i++)
        leds_clearOne(self, i);

    leds_writeRange(self, 0, leds_lit+1, bar_r*bri,bar_g*bri,bar_b*bri);
    leds_brighten(self, leds_lit, ((fill*self->num)-leds_lit)*bri);
}

// space and time are floats 0-1
void leds_centerDisplay(_LEDS *self, float fire, float space) {
    // write a red bar from the bottom up based on `fire`
    uint16_t leds_fire = fire*self->num;
    leds_writeFire(self, 0, leds_fire+1); // fire color
    leds_brighten(self, leds_fire, (float)(fire*self->num)-leds_fire);
    // write a blue dot based on `space`
    uint16_t space_pos = space*self->num;
    leds_writeRange(self, space_pos, space_pos+6, 0,40,255); // spaceship color
    // clear all the LEDs that aren't supposed to be lit
    leds_writeRange(self, leds_fire+1, space_pos, 0,0,0);
    leds_writeRange(self, space_pos+6, self->num, 0,0,0);
}

volatile float fire_bri = .5;
void leds_writeFire(_LEDS *self, uint16_t start, uint16_t end) {
    float fbri = fire_bri;
    // weight the random walk towards middle brightness
    uint8_t brighten = rng_coin_flip(100 - 100*fbri);
    if (brighten && fbri < .9) {
        fbri += .03;
    } else if (!brighten && fbri > .1) {
        fbri -= .03;
    }

    leds_writeRange(self, start, end, 200*fbri,20*fbri,0);
}

void leds_clear(_LEDS *self) {
    leds_writeRGBs(self, 0,0,0);
}

void leds_clearOne(_LEDS *self, uint16_t led) {
    leds_writeRGB(self, led, 0,0,0);
}

void leds_writeRange(_LEDS *self, uint16_t start, uint16_t end, uint8_t red, uint8_t green, uint8_t blue) {
    uint16_t i;
    for (i = start; i < end; i++)
        leds_writeRGB(self, i, red,green,blue);
}

void leds_writeRGB(_LEDS *self, uint8_t led, uint8_t red, uint8_t green, uint8_t blue) {
    leds_state[3*(self->stateptr + led)] = green;
    leds_state[3*(self->stateptr + led)+1] = red;
    leds_state[3*(self->stateptr + led)+2] = blue;
}

void leds_brighten(_LEDS *self, uint8_t led, float factor) {
    leds_state[3*(self->stateptr + led)] *= factor;
    leds_state[3*(self->stateptr + led)+1] *= factor;
    leds_state[3*(self->stateptr + led)+2] *= factor;
}

void leds_writeWhite(_LEDS *self, uint8_t led, uint8_t brightness) {
    leds_state[3*(self->stateptr + led)] = brightness;
    leds_state[3*(self->stateptr + led)+1] = brightness;
    leds_state[3*(self->stateptr + led)+2] = brightness;
}