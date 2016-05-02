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
#include "rng.h"
#include "cmd.h"
#include "int.h"
#include "timer.h"
#include "spacecomms.h"

uint16_t __play_rand_cmd_idx() {
    uint8_t act = 0;
    act = rng_int(0, 17);
    uint8_t cons = act/6;
    // printf("Console: %d\n", cons);
    uint8_t group = act%6;
    printf("Group: %d\r\n", group);

    uint8_t groupcmds = cmd_groupcount(cons, group);

    uint8_t groupidx = cmd_groupidx(cons, group);
    return rng_int(groupidx, groupidx+groupcmds - 1);
}

// uint16_t __play_valid_cmd_idx() {
//     uint16_t idx;
//     do {
//         idx = __play_rand_cmd_idx();
//     } while(cmd_test(idx));

//     return idx;
// }

int16_t main(void) {
    init_clock();
    init_uart();
    init_spi();
    init_timer();

    timer_initDelayMicro(&timer5);

    init_ui();
    init_pin();
    init_oc();
    init_int();
    init_leds();
    init_cd();
    init_cmd();
    init_rng();

    led_on(&led3);

    // printf("Command index: %u\r\n", __play_rand_cmd_idx());
    // printf("Command index: %u\r\n", __play_rand_cmd_idx());
    // printf("Command index: %u\r\n", __play_rand_cmd_idx());
    // printf("Command index: %u\r\n", __play_rand_cmd_idx());
    // printf("Command index: %u\r\n", __play_rand_cmd_idx());
    // printf("Command index: %u\r\n", __play_rand_cmd_idx());
    // printf("Command index: %u\r\n", __play_rand_cmd_idx());
    // printf("Command index: %u\r\n", __play_rand_cmd_idx());

    __play_rand_cmd_idx();
    __play_rand_cmd_idx();
    __play_rand_cmd_idx();
    __play_rand_cmd_idx();
    __play_rand_cmd_idx();
    __play_rand_cmd_idx();
    __play_rand_cmd_idx();
    __play_rand_cmd_idx();

    while (1) {}
}
