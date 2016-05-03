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
#include "spi.h"
#include "ui.h"
#include "timer.h"

#define MPU_ACCEL_CONFIG 0x1C
#define MPU_ACCEL_CONFIG2 0x1D
#define MPU_ACCEL_XOUT_L 0x3C
#define MPU_PWR_MGMT_1 0x6B

#define DELAY_INIT 1e3 // us

volatile uint16_t rng_val;

_PIN MPU9250_CSN, MPU9250_INT;

void __rng_mpu_writeReg(uint8_t address, uint8_t value) {
    if (address<=0x7E) {
        pin_clear(&MPU9250_CSN);
        spi_transfer(&spi2, address);
        spi_transfer(&spi2, value);
        pin_set(&MPU9250_CSN);
    }
}

uint8_t __rng_mpu_readReg(uint8_t address) {
    uint8_t value;

    if (address<=0x7E) {
        pin_clear(&MPU9250_CSN);
        spi_transfer(&spi2, 0x80|address);
        value = spi_transfer(&spi2, 0);
        pin_set(&MPU9250_CSN);
        return value;
    } else
        return 0xFF;
}

void __rng_init_mpu(void) {
    _PIN MPU_SCK, MPU_MISO, MPU_MOSI;
    pin_init(&MPU_SCK, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)&ANSB, 9, 9, 8, 9, (uint16_t *)&RPOR4);
    pin_init(&MPU_MISO, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)&ANSB, 14, 14, 0, 14, (uint16_t *)&RPOR7);
    pin_init(&MPU_MOSI, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)&ANSB, 8, 8, 0, 8, (uint16_t *)&RPOR4);

    pin_init(&MPU9250_CSN, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)NULL, 13, -1, 0, -1, (uint16_t *)NULL);
    pin_init(&MPU9250_INT, (uint16_t *)&PORTB, (uint16_t *)&TRISB, 
             (uint16_t *)NULL, 11, -1, 0, -1, (uint16_t *)NULL);

    pin_digitalOut(&MPU9250_CSN);
    pin_set(&MPU9250_CSN);

    spi_open(&spi2, &MPU_MISO, &MPU_MOSI, &MPU_SCK, 1e6, 0, 0);

    __rng_mpu_writeReg(MPU_PWR_MGMT_1, 0x80);
    __rng_mpu_writeReg(MPU_ACCEL_CONFIG, 0x08);
    __rng_mpu_writeReg(MPU_ACCEL_CONFIG2, 0x09);

    timer_delayMicro(DELAY_INIT);
}

uint16_t __rng_seed() {
    uint16_t rn = 0;
    uint16_t i;
    for (i = 0; i < 16; i++) {
        rn |= (0x0004 & (uint16_t)__rng_mpu_readReg(MPU_ACCEL_XOUT_L)) << i;
        timer_delayMicro(500);
    }
    return rn;
}


void init_rng() {
    __rng_init_mpu();
    srand(__rng_seed());
}

// uint16_t rng_gen() {
//     // value = (value & 0b1000000000000000) ? ((value ^ 0b0000000000100101) << 1) | 0b0000000000000001 : value << 1;
//     rng_val = (rng_val & 0x8000) ? ((rng_val ^ 0x0025) << 1) | 0x0001 : rng_val << 1;
//     return rng_val;
// }


//http://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c
uint16_t randint(uint16_t n) {
  if ((n - 1) == 32767) {
    return rand();
  } else {
    // Chop off all of the values that would cause skew...
    long end = 32767 / n; // truncate skew
    // assert (end > 0L);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}


// from min (inclusive) to max (inclusive)
uint16_t rng_int(uint16_t min, uint16_t max) {
    uint16_t n = max - min;
    return min + randint(n+1);
    
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
