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
_PIN *Sint1 = &D[10];
_PIN *Sint2 = &D[6];
_PIN *Sint3 = &D[8];

_PIN *SSn[] = { &D[3], &D[5], &D[7] };
_PIN *Coin = &D[12];

volatile uint8_t coin = 0;
volatile uint8_t level_number = 0;
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
    play_state_change(0);
}

void con2_state_change(_INT *intx) {
    play_state_change(1);
}

void con3_state_change(_INT *intx) {
    play_state_change(2);
}

void coin_wait(){
    char coin_str[33] = "Please insert a penny";
    if (state != last_state) {  // if we are entering the state, do initialization stuff
        last_state = state;
        lcd_broadcast(coin_str);
    }
    if (coin == 1){
        state = pre_level;
        level_number = 1;
        coin = 0;
    }

}

volatile uint8_t red_pressed = 0;
void pre_level(){
    char ready_str[33]="Hold the red button if ready";
    char launch_str[33]=" Space Team      Launched";
    if (state != last_state) {
        last_state = state;
        lcd_broadcast(ready_str);
    }
    
    red_pressed = con[0].state.s1.red_button && con[1].state.s2.red_button && con[2].state.s3.red_button;
    if (red_pressed == 1){
        state = level_play;
    }

    if (state != last_state) {
        lcd_broadcast(launch_str);
    }
        
}

volatile uint8_t level_successs = 0;
void level_play(){
    char win_str[33]="You Beat Level ";
    if (state != last_state) {
        last_state = state;
        lev_setup(level_number);
        play_begin();
    }

    if(play.PLAYING == 0){
        level_successs = play.success;
    // level_success = play_level(&lev1); //TODO fix play_level() so it works in this way
        if (level_success == 0){
            game_success = 0;
            state = game_over;
        } else if (level_success == 1){
            if (level_number == 15){
                game_success = 1;
                state = game_over;
            }
            else {
                level_number = level_number +1;
                state = pre_level;
            }
        }
    }

    if(state != last_state){
        if(level_success == 1){
            strcat(win_str,numbers[level_number]);
            lcd_broadcast(win_str); //TODO get level_number and string cat
        }
    }


}

void game_over(){
    char lose_str[33]="You made it to  level ";
    char win_str[33]="Congratulations!You Win!";
    if (state != last_state) {
        last_state = state;
        if (game_success == 0){
            strcat(lose_str,numbers[level_number]);
            strcat(lose_str,"!");
            lcd_broadcast(lose_str);//TODO get level_number and string cat//TODO get level_number and string cat
        } else {
            lcd_broadcast(win_str);//TODO get level num and string cat
        }
    }
    led_on(&led1);
    uint8_t i;
    for (i=0; i<10; i++ ){
    timer_delayMicro(50000);
    }

    state = coin_wait;
    

}

void coin_handler(_INT *intx) {
    coin = 1;
    //uint8_t level_successs = play_level(); // (for now)
}

void init_game(){
    // uint8_t level_successs = level_play(); // (for now)
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
    // cd3.tick_sec = GAME_TICK;

    init_cmd();
    init_i2c();
    init_lcd(1);
    init_rng();
    init_con();

    spi_open(&spi1, &D[0], &D[1], &D[2], 1e6, 1, 1);
    pin_digitalIn(Sint1);
    pin_digitalIn(Sint2);
    pin_digitalIn(Sint3);
    pin_digitalIn(Coin);

    int_attach(&int1, Sint1, 1, con1_state_change);
    int_attach(&int2, Sint2, 1, con2_state_change);
    int_attach(&int3, Sint3, 1, con3_state_change);
    int_attach(&int4, Coin, 0, coin_handler);
    //init_game(&int4);
}

int16_t main(void) {

    init_master();
    level_number=15;
    state = game_over;
    game_success = 1;
    last_state = (STATE_HANDLER_T)NULL;
    while (1) {
        state();
    }
    // init_master();
    // state = coin_wait;
    // last_state = (STATE_HANDLER_T)NULL;
    // while (1) {
    //     state();
    // }
}
