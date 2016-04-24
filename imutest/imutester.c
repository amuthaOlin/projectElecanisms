#include <p24FJ128GB206.h>
#include <stdio.h>
#include "config.h"
#include "common.h"
#include "pin.h"
#include "uart.h"
#include "spi.h"
#include "leds.h"
#include "ui.h"
#include "cd.h"
#include "cmd.h"
#include "int.h"
#include "timer.h"

// MPU-9250 Register Map for Gyroscope and Accelerometer


int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_timer();
    timer_initDelayMicro(&timer5);

    init_ui();
    init_mpu();
    init_pin();
    init_oc();

    led_on(&led3);

    while (1) {
        printf("====================\r\n");
        printf("Reading: %04x\r\n", accel_bits());
        timer_delayMicro(100e3);
    }
}
