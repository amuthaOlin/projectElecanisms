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
#ifndef _LEDS_H_
#define _LEDS_H_

#include <stdint.h>
#include "pin.h"
#include "timer.h"
#include "oc.h"

void init_leds(void);

typedef struct _LEDS {
    uint16_t num;
    uint16_t stateptr;
} _LEDS;

extern _LEDS ledbar1, ledbar2, ledbar3, ledcenter;

void leds_init(_LEDS *self, uint16_t num, uint16_t stateptr);
void leds_writeRGB(_LEDS *self, uint8_t led, uint8_t red, uint8_t green, uint8_t blue);
void leds_writeWhite(_LEDS *self, uint8_t led, uint8_t brightness);
void leds_brighten(_LEDS *self, uint8_t led, float factor);

void leds_writeRGBs(_LEDS *self, uint8_t red, uint8_t green, uint8_t blue);
void leds_writeRange(_LEDS *self, uint16_t start, uint16_t end, uint8_t red, uint8_t green, uint8_t blue);
void leds_clear(_LEDS *self);
void leds_clearOne(_LEDS *self, uint16_t led);

// void leds_bounce(_LEDS *self, float period, uint8_t red, uint8_t green, uint8_t blue);
void leds_centerDisplay(_LEDS *self, float fire, float space);
void leds_writeFire(_LEDS *self, uint16_t start, uint16_t end);
void leds_bar(_LEDS *self, float fill, float bri);

#endif
