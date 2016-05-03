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

uint8_t read_tri_state(){
    uint16_t tri_state_in = (uint16_t)pin_read(&A[0]);
    uint8_t tri_state_out;
    // printf("%u\n\r",tri_state_in);
    if (tri_state_in<1000){
        tri_state_out = 1;
    }
    else if(tri_state_in>60000){
        tri_state_out = 2;
    }
    else if (tri_state_in>30000 && tri_state_in<35000){
        tri_state_out = 0;
    }
    return tri_state_out;
}

uint8_t read_dial(){

    uint16_t dial_in = (uint16_t)pin_read(&A[5]); 
    uint16_t dial_out;
    //printf("dial_In:%u\n\r",dial_in);
    if (dial_in > 40000){
        dial_out = 0;
    }
    else if(dial_in <= 40000 && dial_in > 20000){
        dial_out = 1;
    }
    else if(dial_in <= 20000 && dial_in >= 0){
        dial_out = 2;
    }
    // printf("dial_out:%u\n\r",dial_out);
    return dial_out;
}

uint8_t read_slider(){

    uint16_t slider_in = (uint16_t)pin_read(&A[1]);
    uint8_t slider_out = 0;
    //printf("Slider_In:%u\n\r",slider_in);

    if (slider_in > 55000){
        slider_out = 0;
    }
    else if(slider_in < 55000 && slider_in > 49000){
        slider_out = 1;
    }
    else if(slider_in < 49000 && slider_in > 30000){
        slider_out = 2;
    }
    else if(slider_in < 30000){
        slider_out = 3;
    }
    // printf("Slider_out:%u\n\r",slider_out);
    return slider_out;

}


void poll_state(_CONSOLE *self) {
    //led_toggle(&led3);
    self->state.s2.red_button = (uint8_t)!pin_read(&D[5]);
    self->state.s2.green_button1 = (uint8_t)pin_read(&D[7]);
    self->state.s2.green_button2 = (uint8_t)pin_read(&D[8]);
    self->state.s2.green_button3 = (uint8_t)pin_read(&D[9]);
    self->state.s2.green_button4 = (uint8_t)pin_read(&D[10]);
    self->state.s2.wormhole1 = (uint8_t)pin_read(&D[12]);
    self->state.s2.wormhole2 = (uint8_t)pin_read(&D[13]);
    self->state.s2.tri_state = read_tri_state();
    self->state.s2.slider = read_slider();
    self->state.s2.hotsystem1 = (uint8_t)pin_read(&A[2]);
    self->state.s2.hotsystem2 = (uint8_t)pin_read(&A[3]);
    self->state.s2.hotsystem3 = (uint8_t)pin_read(&A[4]);
    self->state.s2.hotsystem4 = (uint8_t)pin_read(&D[11]); 
    self->state.s2.dial = read_dial();
    //led_write(&led2, self->state.s0.red_button);

    // printf("State: %08lx\r\n", (unsigned long)self->state.ul);
}

void console2_poll(_TIMER *timer) {
    console_poll_changes(&console);
}

void console2_init(){
    pin_digitalIn(&A[2]);
    pin_digitalIn(&A[3]);
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
    init_lcd(2);
    lcd_clear(&lcd[0]);
    lcd_clear(&lcd[1]);
    lcd_clear(&lcd[2]);

    init_console();
    
    console2_init();
    console_attach_poll(&console, poll_state);

    timer_every(&timer4, 1e-2, console2_poll);

    state = console_s_level;
    led_toggle(&led1);
    while(1) {
        state();
        // printf("Slave sent: 0x%x\r\n", 0x5A);
        // printf("Slave received: 0x%x\r\n", res);
    }
}
