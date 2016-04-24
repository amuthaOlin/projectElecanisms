#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "spi.h"
#include "timer.h"
#include "uart.h"
#include "spacecomms.h"
#include "console.h"

typedef void (*STATE_HANDLER_T)(void);
STATE_HANDLER_T state, last_state;

void change_lcds(void);
void level(void);

void change_lcds(void){
    if (state != last_state){
        last_state = state;
        update_lcds(console.res);
    }
    state = level;
    if (state != last_state){
        
    }
}

void level(void){
    if (state != last_state){
        last_state = state;
    }
    if(console.LCD_flag == 1){
        state = change_lcds;
    }
    if (state != last_state){
        
    }
}

uint8_t read_wordwheel_inside(){
    uint16_t wordwheel_in = (uint16_t)pin_read(&A[1]);
    //printf("wordwheel_inside:%u\n\r",wordwheel_in);
    uint8_t wordwheel_out;
    if(wordwheel_in<51000){
        wordwheel_out = 0;
    }
    else if (wordwheel_in<52000 && wordwheel_in>51000){
        wordwheel_out =1;    
    }
    else if (wordwheel_in<54000 && wordwheel_in>52000){
        wordwheel_out =2;    
    }
    else if (wordwheel_in<56000 && wordwheel_in>54000){
        wordwheel_out =3;    
    }
    else if (wordwheel_in<59000 && wordwheel_in>56000){
        wordwheel_out =4;    
    }
    else if (wordwheel_in>59000){
        wordwheel_out =5;    
    }
    // printf("outside:%u\n\r",wordwheel_out);
    return wordwheel_out;
}

uint8_t read_wordwheel_outside(){
    uint16_t wordwheel_in = (uint16_t)pin_read(&A[2]);
    uint8_t wordwheel_out;
    if(wordwheel_in<22000){
        wordwheel_out = 0;
    }
    else if (wordwheel_in<29000 && wordwheel_in>22000){
        wordwheel_out =1;    
    }
    else if (wordwheel_in<35000 && wordwheel_in>29000){
        wordwheel_out =2;    
    }
    else if (wordwheel_in<40000 && wordwheel_in>35000){
        wordwheel_out =3;    
    }
    else if (wordwheel_in<44000 && wordwheel_in>40000){
        wordwheel_out =4;    
    }
    else if (wordwheel_in>44000){
        wordwheel_out =5;    
    }
    return wordwheel_out;
}

uint8_t read_joystick(){
    uint8_t j1 = pin_read(&D[8]);
    uint8_t j3 = pin_read(&D[9]);
    uint8_t j5 = pin_read(&D[10]);
    uint8_t j7 = pin_read(&A[5]); // pin 11 isn't working

    return j1 + (j3<<1) + (j5<<2) + (j7<<3);
}


void poll_state(_CONSOLE *self) {
    //led_toggle(&led3);
    self->state.s1.red_button = (uint8_t)!pin_read(&D[5]);
    if (self->state.s1.red_button)
        led_on(&led3);
    else
        led_off(&led3);
    self->state.s1.toggle1 = (uint8_t)pin_read(&D[6]);
    self->state.s1.toggle2 = (uint8_t)pin_read(&D[7]);
    self->state.s1.green_button = (uint8_t)pin_read(&A[4]);
    self->state.s1.joystick = read_joystick();
    self->state.s1.wormhole1 = (uint8_t)pin_read(&D[12]);
    self->state.s1.wormhole2 = (uint8_t)pin_read(&D[13]);   
    self->state.s1.hotsystem = (uint8_t)pin_read(&A[0]);
    self->state.s1.wordwheel_inside = read_wordwheel_inside();
    self->state.s1.wordwheel_outside = read_wordwheel_outside();

    printf("State: %08lx\r\n", (unsigned long)self->state.ul);
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
    timer_initDelay(&timer5);

    init_uart();
    init_console();

    init_i2c();
    init_lcd(1);

    console1_init();
    console_attach_poll(&console, poll_state);


    timer_every(&timer4, 1e-2, console1_poll);

    state = level;
    while(1) {
        state();
        // printf("Slave sent: 0x%x\r\n", 0x5A);
        // printf("Slave received: 0x%x\r\n", res);
    }
}
