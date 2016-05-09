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
#include "con.h"
#include "rng.h"
#include "i2c.h"
#include "lcd.h"
#include "cmd.h"
#include "int.h"
#include "play.h"
#include "timer.h"
#include "spacecomms.h"
#include "oc.h"

volatile WORD32 res[3];
volatile _CMD *commands[3];

_PIN *MISO  = &D[1];
_PIN *MOSI  = &D[0];
_PIN *SCK   = &D[2];
_PIN *Sint1 = &D[4];
_PIN *Sint2 = &D[6];
_PIN *Sint3 = &D[8];

_PIN *SSn[] = { &D[3], &D[5], &D[7] };
_PIN *Coin_pin = &D[12];

volatile uint8_t coin = 0; 
volatile uint8_t power_on = 0;
volatile uint8_t level_number = 5;
volatile uint8_t game_success = 0;
volatile uint8_t level_success = 0;
volatile uint8_t in_level= 0;


char numbers[16][3]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15"};

typedef void (*STATE_HANDLER_T)(void);
void coin_wait(void);
void pre_level(void);
void level_play(void);
void game_over(void);

STATE_HANDLER_T state, last_state;

void con1_state_change(_INT *intx) {
    led_on(&led1);
    play_state_change(0);
}

void con2_state_change(_INT *intx) {
    led_on(&led2);
    play_state_change(1);
}

void con3_state_change(_INT *intx) {
    led_on(&led3);
    play_state_change(2);
}

void coin_wait(){
    char coin_str[33] = "Please insert a penny.";
    if (state != last_state) {  // if we are entering the state, do initialization stuff
        last_state = state;
        lcd_broadcast(coin_str);
    }
    if (coin==1) {
        state = pre_level;
        level_number = 13; //change to 1 for real game  //CHANGED
        coin = 0;
    }
}

volatile uint8_t red_pressed = 0;
char ready_str[33]="Hold the red button when ready.";
char launch_str[33]="Spaceteam       launched!";
void pre_level() {
    if (state != last_state) {
        last_state = state;
        lcd_broadcast(ready_str);
    }
    
    // red_pressed = con[0].state.s1.red_button && con[1].state.s2.red_button && con[2].state.s3.red_button;
    red_pressed = 1;
    if (red_pressed) {
        lcd_broadcast(launch_str);
        uint8_t i;
        for (i = 0; i < 20; i++)
            timer_delayMicro(50000);
        state = level_play;
    }
}

char level_win_str[33] = "You beat level  ";
                        //0123456789012345<---==
void level_play() {
    if (state != last_state) {
        last_state = state;
        pin_clear(&D[11]);
        uint8_t i;
        for (i = 0; i < 2; i++)
            timer_delayMicro(50000);
        pin_set(&D[11]);
        for (i = 0; i < 3; i++)
            timer_delayMicro(50000);
        lev_setup(level_number);
        lev_genCmd();
        play_begin();
    }

    if (play.PLAYING == 0) {
        level_success = play.success;
        if (level_success == 0){
            game_success = 0;  
            state = game_over;
        } else if (level_success == 1){
            if (level_number == 15){
                game_success = 1;
                state = game_over;
            } else {
                level_number++;
                state = pre_level;
            }
        }
    }

    if (state != last_state){
        if (level_success == 1) {
            level_success = 0;
            level_win_str[15] = 0;
            strcat(level_win_str,numbers[level_number-1]);
            lcd_broadcast(level_win_str);
        }
    }
}

void game_over() {
    char game_over[33] = "Game over!";
    char lose_str[33] = "You made it to level ";
                       //0123456789012345678901<---==
    char win_str[33] = "Congratulations!You win!";
    uint8_t i;
    if (state != last_state) {
        last_state = state;
        if (game_success == 0) {
            lcd_broadcast(game_over);
            for (i=0; i<20; i++){
                timer_delayMicro(50000);
            }

            lose_str[21] = 0;
            strcat(lose_str,numbers[level_number]);
            strcat(lose_str,"!");
            lcd_broadcast(lose_str);
        } else {
            lcd_broadcast(win_str);
            game_success = 0;
        }
    }
    for (i=0; i<20; i++){
        timer_delayMicro(50000);
    }

    state = coin_wait;
}

void coin_handler(_INT *intx) {
    if(power_on == 1){
        coin = 1;
        // __asm__("goto 0x0DEA");
    }
    else{
        power_on = 1;
    }
}

void init_master() {
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

    // this represents a problem
    // cd1.tick_sec = GAME_TICK;
    // cd2.tick_sec = GAME_TICK;
    // cd3.tick_sec = GAME_TICK

    init_cmd();
    init_i2c();
    init_lcd(0);
    init_rng();
    init_con();

    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1, 1);
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);
    pin_digitalIn(Sint3);
    pin_digitalIn(Coin_pin);

    int_attach(&int4, Coin_pin, 1, coin_handler);
    int_attach(&int1, Sint1, 1, con1_state_change);
    int_attach(&int2, Sint2, 1, con2_state_change);
    int_attach(&int3, Sint3, 1, con3_state_change);
}

int16_t main(void) {
    init_master();
    pin_digitalOut(&D[11]);
    pin_set(&D[11]);
    state = coin_wait;
    last_state = (STATE_HANDLER_T)NULL;
    while (1) {
        state();
    }
}
