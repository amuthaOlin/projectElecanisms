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

uint8_t read_wordwheel_outside(){
    uint16_t wordwheel_in = (uint16_t)pin_read(&A[1]);
    // printf("wordwheel_outside:%u\n\r",wordwheel_in);
    uint8_t wordwheel_out;
    if (wordwheel_in<33000 && wordwheel_in>23000){
        wordwheel_out = 0;
    }
    else if (wordwheel_in<43000 && wordwheel_in>33000){
        wordwheel_out =5;    
    }
    else if (wordwheel_in<48000 && wordwheel_in>43000){
        wordwheel_out =4;    
    }
    else if (wordwheel_in<58000 && wordwheel_in>48000){
        wordwheel_out =3;    
    }
    else if (wordwheel_in<5000 || wordwheel_in>58000){
        wordwheel_out =2;    
    }
    else if (wordwheel_in>5000 && wordwheel_in<16000){
        wordwheel_out =1;    
    }
    // printf("outside:%u\n\r",wordwheel_out);
    return wordwheel_out;
}

uint8_t read_wordwheel_inside(uint8_t wordwheel_prev){
    uint16_t wordwheel_in = (uint16_t)pin_read(&A[2]);
    // printf("wordwheel_inside:%u\r\n",wordwheel_in);
    uint8_t wordwheel_out;
    if(wordwheel_in<16000){
        wordwheel_out = 5;
    }
    else if (wordwheel_in<22000 && wordwheel_in>16000){
        wordwheel_out =4;    
    }
    else if (wordwheel_in<29500 && wordwheel_in>22000){
        wordwheel_out =3;    
    }
    else if (wordwheel_in<36000 && wordwheel_in>29500){
        wordwheel_out =2;    
    }
    else if (wordwheel_in<41000 && wordwheel_in>36000){
        wordwheel_out =1;    
    }
    else if (wordwheel_in>41000){
        wordwheel_out =0;    
    }
    // printf("outside:%u\r\n",wordwheel_out);
    return wordwheel_out;
}

// 16 elements
uint8_t joystick_key[] = {0, 5, 3, 4, 1, 0, 2, 0, 7, 6, 0, 0, 8, 0, 0, 0};

uint8_t read_joystick(){
    uint8_t j1 = pin_read(&D[8]);
    uint8_t j3 = pin_read(&D[9]);
    uint8_t j5 = pin_read(&D[10]);
    uint8_t j7 = pin_read(&A[5]); // pin 11 isn't working

    uint8_t num = j1 + (j3<<1) + (j5<<2) + (j7<<3);
    // printf("Position: %d\r\n", joystick_key[num]);
    return joystick_key[num];
}


void poll_state(_CONSOLE *self) {
    //led_toggle(&led3);
    self->state.s1.red_button = (uint8_t)!pin_read(&D[5]);
    self->state.s1.toggle1 = (uint8_t)pin_read(&D[7]);
    self->state.s1.toggle2 = (uint8_t)pin_read(&D[6]);
    self->state.s1.green_button = (uint8_t)pin_read(&A[4]);
    self->state.s1.joystick = read_joystick();
    self->state.s1.wormhole1 = (uint8_t)pin_read(&D[12]);
    self->state.s1.wormhole2 = (uint8_t)pin_read(&D[13]);   
    self->state.s1.hotsystem = (uint8_t)pin_read(&A[0]);
    self->state.s1.wordwheel_inside = read_wordwheel_inside(self->state.s1.wordwheel_inside);
    self->state.s1.wordwheel_outside = read_wordwheel_outside();
    printf("Wordwheel state: %d|%d\r\n", self->state.s1.wordwheel_outside, self->state.s1.wordwheel_inside);

    // printf("Hotsystem: %d, T1: %d, T2: %d\r\n", self->state.s1.hotsystem, self->state.s1.toggle1, self->state.s1.toggle2);
    // printf("State: %08lx\r\n", (unsigned long)self->state.ul);
}


void console1_poll(_TIMER *timer) {
    console_poll_changes(&console);
}

void console1_init(){
    pin_digitalIn(&A[0]);
    pin_digitalIn(&A[5]);
    pin_digitalIn(&A[4]);
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
    init_lcd(1);
    lcd_clear(&lcd[0]);
    lcd_clear(&lcd[1]);
    lcd_clear(&lcd[2]);

    init_console();

    console1_init();
    console_attach_poll(&console, poll_state);

    timer_every(&timer4, 1e-2, console1_poll);

    state = console_s_level;
    while(1) {
        state();
        // printf("Slave sent: 0x%x\r\n", 0x5A);
        // printf("Slave received: 0x%x\r\n", res);
    }
}
