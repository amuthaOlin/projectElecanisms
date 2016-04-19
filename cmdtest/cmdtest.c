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
#include "lev.h"
#include "strm.h"

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

    _LEV level;
    lev_setup(&level,2);
    lev_genCmd(&level);


    led_on(&led3);

    uint8_t success;
    printf("=================\r\n");
    printf("PRINTING COMMANDS\r\n");
    cmd_print(0);
    cmd_print(27);
    cmd_print(28);
    cmd_print(29);
    cmd_print(30);
    success = cmd_test(27, (WORD32)(uint32_t)0x00000200); // should pass
    printf("Command test: %s\r\n", success? "Success" : "Failure");
    success = cmd_test(28, (WORD32)(uint32_t)0x00000300); // should pass
    printf("Command test: %s\r\n", success? "Success" : "Failure");
    success = cmd_test(29, (WORD32)(uint32_t)0x0000ff00); // should fail
    printf("Command test: %s\r\n", success? "Success" : "Failure");
    success = cmd_test(30, (WORD32)(uint32_t)0x00000400); // should pass
    printf("Command test: %s\r\n", success? "Success" : "Failure");

    while (1) {}
}
