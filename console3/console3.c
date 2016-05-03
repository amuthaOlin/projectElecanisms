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
    //printf("clutch_in:%u\n\r", clutch_in); 
    if (clutch_prev<clutch_in){
        if (clutch_in<9000 && clutch_in>0){
            clutch_out = 1;
        }
        else if(clutch_in<22000 && clutch_in>9000){
            clutch_out = 2;
        }
        else if(clutch_in<25000 && clutch_in>22000){
            clutch_out = 3;
        }
        else if(clutch_in<64000 && clutch_in>25000){
            clutch_out = 4;
        }
    }
    else if (clutch_prev>clutch_in){
        if (clutch_in<9000 && clutch_in>0){
            clutch_out = 1;
        }
        else if(clutch_in<13000 && clutch_in>9000){
            clutch_out = 2;
        }
        else if(clutch_in<25000 && clutch_in>13000){
            clutch_out = 3;
        }
        else if(clutch_in<64000 && clutch_in>25000){
            clutch_out = 4;
        }
    }
    else{
        clutch_out = clutch_prev;
    }
    //printf("clutch_out:%u\n\r", clutch_out); 
    return clutch_out;
}

uint8_t read_arming(){
    uint16_t arming_in = (uint16_t)pin_read(&A[1]);
    uint8_t arming_out;
    //printf("arming_in:%u\n\r", arming_in); 
        if (arming_in<25000){
            arming_out = 1;
        }
        else if(arming_in<52000 && arming_in>20000){
            arming_out = 2;
        }
        else if(arming_in>52000){
            arming_out = 3;
        }
    //printf("arming_out:%u\n\r", arming_out);
    return arming_out;
}

uint8_t read_dial(){

    uint16_t dial_in = (uint16_t)pin_read(&A[3]); 
    uint8_t dial_out;
    //printf("dial_In:%u\n\r",dial_in);
    if (dial_in  < 10000){
        dial_out = 0;
    }
    else if(dial_in < 25000 && dial_in >= 10000){
        dial_out = 1;
    }
    else if(dial_in < 43000 && dial_in >= 25000){
        dial_out = 2;
    }
    else if(dial_in >= 43000){
        dial_out = 3;
    }
    //printf("dial_out:%u\n\r",dial_out);
    return dial_out;
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
    self->state.s3.arming = read_arming();
    self->state.s3.arming_button = (uint8_t)pin_read(&A[2]);
    self->state.s3.dial = read_dial();
    //led_write(&led2, self->state.s0.red_button);
}

void console3_poll(_TIMER *timer) {
    console_poll_changes(&console);
}

void console3_init(){
    pin_digitalIn(&A[2]);
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
    console3_init();

    console_attach_poll(&console, poll_state);

    timer_every(&timer4, 1e-2, console3_poll);

    state = console_s_level;
    while(1) {
        state();
        // printf("Slave sent: 0x%x\r\n", 0x5A);
        // printf("Slave received: 0x%x\r\n", res);
    }
}