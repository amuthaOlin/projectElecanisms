#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "spi.h"
#include "labc.h"
#include "strm.h"
#include "lcd.h"
#include "timer.h"
#include "uart.h"
#include "spacecomms.h"
#include "console.h"

uint8_t read_clutch(uint16_t clutch_prev){
    uint16_t clutch_in = (uint16_t)pin_read(&A[0]);
    uint8_t clutch_out;
    printf("clutch_in:%u\n\r", clutch_in); 
    if (clutch_prev<clutch_in){
        if (clutch_in<33500 && clutch_in>0){
            clutch_out = 0;
        }
        else if(clutch_in<38000 && clutch_in>33500){
            clutch_out = 1;
        }
        else if(clutch_in<41600 && clutch_in>38000){
            clutch_out = 2;
        }
        else if(clutch_in<64000 && clutch_in>41600){
            clutch_out = 3;
        }
    }
    else if (clutch_prev>=clutch_in){
        if (clutch_in<39400 && clutch_in>0){
            clutch_out = 0;
        }
        else if(clutch_in<43000 && clutch_in>39400){
            clutch_out = 1;
        }
        else if(clutch_in<46000 && clutch_in>43000){
            clutch_out = 2;
        }
        else if(clutch_in<64000 && clutch_in>46000){
            clutch_out = 3;
        }
    }
    return clutch_out;
}

void poll_state(_CONSOLE *self) {
    //led_toggle(&led3);
    self->state.s3.red_button = (uint8_t)!pin_read(&D[5]);
    self->state.s3.triangle1 = (uint8_t)!pin_read(&D[6]);
    self->state.s3.triangle2 = (uint8_t)!pin_read(&D[7]);
    self->state.s3.triangle3 = (uint8_t)!pin_read(&D[8]);
    self->state.s3.toggle1 = (uint8_t)pin_read(&D[9]);
    self->state.s3.toggle2 = (uint8_t)pin_read(&D[10]);
    self->state.s3.wormhole1 = (uint8_t)pin_read(&D[12]);
    self->state.s3.wormhole2 = (uint8_t)pin_read(&D[13]);
    self->state.s3.clutch = read_clutch(self->state.s3.clutch);
    //led_write(&led2, self->state.s0.red_button);
}

void console3_poll(_TIMER *timer) {
    console_poll_changes(&console);
}

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    timer_initDelayMicro(&timer5);

    init_uart();

    init_i2c();
    init_lcd(3);
    lcd_clear(&lcd[0]);
    lcd_clear(&lcd[1]);
    lcd_clear(&lcd[2]);

    init_console();

    console_attach_poll(&console, poll_state);

    timer_every(&timer4, 1e-2, console3_poll);

    state = console_s_level;
    while(1) {
        state();
        // printf("Slave sent: 0x%x\r\n", 0x5A);
        // printf("Slave received: 0x%x\r\n", res);
    }
}