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
#include "lcd.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
    init_uart();
    init_spi();
    init_timer();
    init_oc();
    init_int();
    init_leds();
    init_cd();
    init_cmd();
    timer_initDelayMicro(&timer5);
    init_lcd();
    init_rng();

    lcd_clear(&lcdcmd[0]);
    lcd_clear(&lcdcmd[1]);
    lcd_clear(&lcdcmd[2]);


    // lcd_print1(&lcdcmd[0],".");
    // lcd_print1(&lcdcmd[1],".");
    // lcd_print1(&lcdcmd[2],".");

    _LEV level;
    lev_setup(&level,3);
    lev_genCmd(&level);
    lev_printCmd(0,29);
    lev_printCmd(1,30);
    lev_printCmd(2,31);

    // char command[33] = "                                ";
    // strm_genSet(command,"Thingymagig","One");
    // lcd_print(&lcdcmd[0],command);



    lcd_stop(&lcdcmd[0]);
    led_on(&led3);

    // uint8_t success;
    // printf("=================\r\n");
    // printf("PRINTING COMMANDS\r\n");
    // cmd_print(0);
    // cmd_print(27);
    // cmd_print(28);
    // cmd_print(29);
    // cmd_print(30);
    // success = cmd_test(27, (WORD32)(uint32_t)0x00000200); // should pass
    // printf("Command test: %s\r\n", success? "Success" : "Failure");
    // success = cmd_test(28, (WORD32)(uint32_t)0x00000300); // should pass
    // printf("Command test: %s\r\n", success? "Success" : "Failure");
    // success = cmd_test(29, (WORD32)(uint32_t)0x0000ff00); // should fail
    // printf("Command test: %s\r\n", success? "Success" : "Failure");
    // success = cmd_test(30, (WORD32)(uint32_t)0x00000400); // should pass
    // printf("Command test: %s\r\n", success? "Success" : "Failure");

    while (1) {}
}
