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
#include "rng.h"

volatile uint16_t rng_val;

void init_rng() {
    rng_val = 0x0080;

    timer_every(&timer3, 1e-2, rng_gen);
}

uint16_t rng_gen() {
    // value = (value & 0b1000000000000000) ? ((value ^ 0b0000000000100101) << 1) | 0b0000000000000001 : value << 1;
    rng_val = (rng_val & 0x8000) ? ((rng_val ^ 0x0025) << 1) | 0x0001 : rng_val << 1;
    return rng_val;
}

// from min (inclusive) to max (inclusive)
uint16_t rng_int(uint16_t min, uint16_t max) {
    return (rng_gen() % (max + 1 - min)) + min;
}

// perform a weighted coin flip
// "weight" == 0 => 100% chance of 0
// "weight" == 100 => 100% chance of 1
uint16_t rng_coin_flip(uint16_t weight) {
    return rng_int(0, 100) <= weight;
}

// returns either "replace" or "num" -- "weight" is a percent change (0-100) of returning "replace"
uint16_t rng_coin_replace(uint16_t num, uint16_t weight, uint16_t replace) {
    return rng_coin_flip(weight)? replace : num;
}
