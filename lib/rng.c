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

#define MPU_SELF_TEST_X_GYRO 0x00
#define MPU_SELF_TEST_Y_GYRO 0x01
#define MPU_SELF_TEST_Z_GYRO 0x02
#define MPU_SELF_TEST_X_ACCEL 0x0D
#define MPU_SELF_TEST_Y_ACCEL 0x0E
#define MPU_SELF_TEST_Z_ACCEL 0x0F
#define MPU_XG_OFFSET_H 0x13
#define MPU_XG_OFFSET_L 0x14
#define MPU_YG_OFFSET_H 0x15
#define MPU_YG_OFFSET_L 0x16
#define MPU_ZG_OFFSET_H 0x17
#define MPU_ZG_OFFSET_L 0x18
#define MPU_SMPLRT_DIV 0x19
#define MPU_CONFIG 0x1A
#define MPU_GYRO_CONFIG 0x1B
#define MPU_ACCEL_CONFIG 0x1C
#define MPU_ACCEL_CONFIG2 0x1D
#define MPU_LP_ACCEL_ODR 0x1E
#define MPU_WOM_THR 0x1F
#define MPU_FIFO_EN 0x23
#define MPU_I2C_MST_CTRL 0x24
#define MPU_I2C_SLV0_ADDR 0x25
#define MPU_I2C_SLV0_REG 0x26
#define MPU_I2C_SLV0_CTRL 0x27
#define MPU_I2C_SLV1_ADDR 0x28
#define MPU_I2C_SLV1_REG 0x29
#define MPU_I2C_SLV1_CTRL 0x2A
#define MPU_I2C_SLV2_ADDR 0x2B
#define MPU_I2C_SLV2_REG 0x2C
#define MPU_I2C_SLV2_CTRL 0x2D
#define MPU_I2C_SLV3_ADDR 0x2E
#define MPU_I2C_SLV3_REG 0x2F
#define MPU_I2C_SLV3_CTRL 0x30
#define MPU_I2C_SLV4_ADDR 0x31
#define MPU_I2C_SLV4_REG 0x32
#define MPU_I2C_SLV4_DO 0x33
#define MPU_I2C_SLV4_CTRL 0x34
#define MPU_I2C_SLV4_DI 0x35
#define MPU_I2C_MST_STATUS 0x36
#define MPU_INT_PIN_CFG 0x37
#define MPU_INT_ENABLE 0x38
#define MPU_INT_STATUS 0x3A
#define MPU_ACCEL_XOUT_H 0x3B
#define MPU_ACCEL_XOUT_L 0x3C
#define MPU_ACCEL_YOUT_H 0x3D
#define MPU_ACCEL_YOUT_L 0x3E
#define MPU_ACCEL_ZOUT_H 0x3F
#define MPU_ACCEL_ZOUT_L 0x40
#define MPU_TEMP_OUT_H 0x41
#define MPU_TEMP_OUT_L 0x42
#define MPU_GYRO_XOUT_H 0x43
#define MPU_GYRO_XOUT_L 0x44
#define MPU_GYRO_YOUT_H 0x45
#define MPU_GYRO_YOUT_L 0x46
#define MPU_GYRO_ZOUT_H 0x47
#define MPU_GYRO_ZOUT_L 0x48
#define MPU_EXT_SENS_DATA_00 0x49
#define MPU_EXT_SENS_DATA_01 0x4A
#define MPU_EXT_SENS_DATA_02 0x4B
#define MPU_EXT_SENS_DATA_03 0x4C
#define MPU_EXT_SENS_DATA_04 0x4D
#define MPU_EXT_SENS_DATA_05 0x4E
#define MPU_EXT_SENS_DATA_06 0x4F
#define MPU_EXT_SENS_DATA_07 0x50
#define MPU_EXT_SENS_DATA_08 0x51
#define MPU_EXT_SENS_DATA_09 0x52
#define MPU_EXT_SENS_DATA_10 0x53
#define MPU_EXT_SENS_DATA_11 0x54
#define MPU_EXT_SENS_DATA_12 0x55
#define MPU_EXT_SENS_DATA_13 0x56
#define MPU_EXT_SENS_DATA_14 0x57
#define MPU_EXT_SENS_DATA_15 0x58
#define MPU_EXT_SENS_DATA_16 0x59
#define MPU_EXT_SENS_DATA_17 0x5A
#define MPU_EXT_SENS_DATA_18 0x5B
#define MPU_EXT_SENS_DATA_19 0x5C
#define MPU_EXT_SENS_DATA_20 0x5D
#define MPU_EXT_SENS_DATA_21 0x5E
#define MPU_EXT_SENS_DATA_22 0x5F
#define MPU_EXT_SENS_DATA_23 0x60
#define MPU_I2C_SLV0_DO 0x63
#define MPU_I2C_SLV1_DO 0x64
#define MPU_I2C_SLV2_DO 0x65
#define MPU_I2C_SLV3_DO 0x66
#define MPU_I2C_MST_DELAY_CTRL 0x67
#define MPU_SIGNAL_PATH_RESET 0x68
#define MPU_MOT_DETECT_CTRL 0x69
#define MPU_USER_CTRL 0x6A
#define MPU_PWR_MGMT_1 0x6B
#define MPU_PWR_MGMT_2 0x6C
#define MPU_FIFO_COUNTH 0x72
#define MPU_FIFO_COUNTL 0x73
#define MPU_FIFO_R_W 0x74
#define MPU_WHO_AM_I 0x75
#define MPU_XA_OFFSET_H 0x77
#define MPU_XA_OFFSET_L 0x78
#define MPU_YA_OFFSET_H 0x7A
#define MPU_YA_OFFSET_L 0x7B
#define MPU_ZA_OFFSET_H 0x7D
#define MPU_ZA_OFFSET_L 0x7E

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
        timer_delayMicro(50);
    }
    return rn;
}


void init_rng() {
    __rng_init_mpu();
    rng_val = __rng_seed();

    timer_every(&timer3, 1e-2, (void*)rng_gen);
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
