#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "i2c.h"
#include "LCD.h"
#include "timer.h"

// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 ?  E  RW RS

void CamelCase(char* str){
    while(*str){
        if(*str >= 'a' && *str <= 'z'){
        *str = ('A' + *str - 'a');
        str++;
        }
        str++;
    }
}

void Caesar(char* str, uint8_t shift){
    while(*str){
        if(*str >= 'a' && *str <= 'z'){
        *str = *str+shift;
        }
        str++;
    }
}

int16_t main(void) {
    led_on(&led1);
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
    init_timer();
    led_off(&led1);
    init_delay();
    // led_on(&led3);

    // line 2 starts at 40.
    char string1[17] ="TEAM SPACE TEAM";
    char string2[17] ="IS THE BEST TEAM";
    char string[17]="spacey team!";
    char* strptr1=string1;
    char* strptr2=string2;
    char* strptr=string;
    init_lcd(&i2c3, 1e3);
    // lcd_write(&i2c3,'E');
    // lcd_cursor(&i2c3,1);
    delayMicroseconds(1000);
    // lcd_goto(&i2c3,1,8);
    delayMicroseconds(100);
    // lcd_write(&i2c3,'E');
    // lcd_goto(&i2c3,2,0);
    // lcd_write(&i2c3,'P');
    // lcd_write(&i2c3,'1');
    // CamelCase(strptr);
    Caesar(strptr,1);
    lcd_print(&i2c3,strptr);
    // lcd_print2(&i2c3,strptr1,strptr2);
    led_on(&led3);
    // i2c_stop(&i2c3);

    while(1) {
        delayMicroseconds(500);
        led_toggle(&led2);


    }
}


