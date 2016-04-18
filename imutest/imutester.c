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
#include "imu.h"

int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_timer();
    init_ui();
    init_pin();
    init_oc();
    init_int();
    init_leds();
    init_cd();
    init_cmd();
    init_rng();
    imu_init();

    led_on(&led3);

    printf("Reading?: %d\r\n", gyro_read(0));
    printf("Reading?: %d\r\n", gyro_read(1));
    printf("Reading?: %d\r\n", gyro_read(2));
    printf("Reading?: %d\r\n", gyro_read(3));
    printf("Reading?: %d\r\n", gyro_read(4));
    printf("Reading?: %d\r\n", gyro_read(5));
    printf("Reading?: %d\r\n", gyro_read(6));
    printf("Reading?: %d\r\n", gyro_read(7));

    while (1) {}
}
