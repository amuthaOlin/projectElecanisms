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

uint8_t level_number = 0;
uint8_t game_success = 0;
typedef void (*STATE_HANDLER_T)(void);
void coin_wait(void);
void pre_level(void);
void level(void);
void game_over(void);

STATE_HANDLER_T state;




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
    int i = 0;
    for(i=0;i<3;i++){
        lcd_print(con[i].lcd,"Please insert a penny");
    }
}

void pre_level(){
    uint8_t red_pressed = 0;
    int i = 0;
    for(i=0;i<3;i++){
        lcd_print(con[i].lcd,"hold down the red button when ready");
    }
    while(red_pressed == 0){
        red_pressed = con[0].state.s1.red_button && con[1].state.s2.red_button && con[2].state.s3.red_button;
    }
    for(i=0;i<3;i++){
        lcd_print(con[i].lcd,"Start in 3");
    }
    for(i=0;i<3;i++){
        lcd_print(con[i].lcd,"Start in 2");
    }
    for(i=0;i<3;i++){
        lcd_print(con[i].lcd,"Start in 1");
    }
    for(i=0;i<3;i++){
        lcd_print(con[i].lcd," Space Team      Launched");
    }
    state = level;
}

void level(){
    uint8_t level_succes = 0;
    lev_setup(&lev1,level_number);
    level_succes = play_level(&lev1); //TODO fix play_level() so it works in this way
    if (level_succes == 0){
        game_success = 0;
        state = game_over;
    }
    else if(level_succes == 1){
        if(level_number == 15){
            game_success = 1;
            state = game_over;
        }
        else{
            level_number = level_number +1;
            state = pre_level;
        }
    }
}

void game_over(){

    if (game_success == 0){
        for(i=0;i<3;i++){
            lcd_print(con[i].lcd,"Game Over. You lost. You reached level ");//TODO get level num and string cat
        }
    }
    else{
        for(i=0;i<3;i++){
            lcd_print(con[i].lcd,"Game Over. You lost. You reached level ");//TODO get level num and string cat
        }
    }
}




void init_game(_INT *intx) {
    level_number = 1;
    state = pre_level;
    //uint8_t level_success = play_level(); // (for now)
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
    init_lev(); // TODO:NEEDS TO BE WRITTEN

    // this represents a problem
    // cd1.tick_sec = GAME_TICK;
    // cd2.tick_sec = GAME_TICK;
    // cd3.tick_sec = GAME_TICK;

    init_cmd();
    init_i2c();
    init_lcd(0);
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
    int_attach(&int4, Coin, 0, init_game);

    //init_game();
}

int16_t main(void) {
    init_master();
    state = coin_wait;
    while (1) {
        state();
    }
}
