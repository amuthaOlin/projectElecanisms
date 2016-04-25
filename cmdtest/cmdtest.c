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
#include "play.h"
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
    init_lcd(0);
    init_rng();
    init_con();

    lcd_clear(&lcd[0]);
    lcd_clear(&lcd[1]);
    lcd_clear(&lcd[2]);


    // lcd_print1(&lcdcmd[0],".");
    // lcd_print1(&lcdcmd[1],".");
    // lcd_print1(&lcdcmd[2],".");

    // lev_genCmd();
    // cmd_print(32);
    // cmd_print(33);
    // cmd_print(34);


    // char command[33] = "                                ";
    // strm_genSet(command,"Thingymagig","One");
    // lcd_print(&lcdcmd[0],command);



    lcd_stop(&lcd[0]);
    led_on(&led3);


    _PIN *MISO  = &D[1];
    _PIN *MOSI  = &D[0];
    _PIN *SCK   = &D[2];
    _PIN *Sint1 = &D[10];
    _PIN *Sint2 = &D[6];
    _PIN *Sint3 = &D[8];

    _PIN *SSn[] = { &D[3], &D[5], &D[7] };

    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1, 1);
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);
    pin_digitalIn(Sint3);

    // int_attach(&int1, Sint1, 1, con1_state_change);
    // int_attach(&int2, Sint2, 1, con2_state_change);
    // int_attach(&int3, Sint3, 1, con3_state_change);


    uint8_t i, j;
    for (i = 1; i < 15; i++) {
        for (j = 0; j < 10; j++) {
            timer_delayMicro(0xFFFF);
        }
        lev_init();
        lev_setup(i);
        play_begin();
        printf("LEVEL %d===========================\r\n", i);
        printf("Label theme: %d\r\n", level.lab_theme);
        printf("Label mod: %d\r\n", level.mod);
        printf("Label arg1: %d\r\n", level.arg_freq);
        printf("Label arg2: %d\r\n", level.arg_shift);
    }

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
