#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "i2c.h"
#include "timer.h"
#include "lcd.h"
#include "strm.h"
#include "uart.h"

// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 ?  E  RW RS

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_uart();
    init_i2c();
    init_timer();
    timer_initDelayMicro(&timer5);

    init_lcd(1);

    lcd_clear(&lcd[0]);
    lcd_clear(&lcd[1]);
    lcd_clear(&lcd[2]);


    printf("____________\r\n");
    char string1[33];
    char* strptr1=string1;
    char string2[33]="Cake Muncher";
    char* strptr2=string2;
    char string3[33]="SP AC E   ";
    char* strptr3=string3;

    char numbers[33][3]={"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32"};
    // lcd_broadcast(strptr1);
    // lcd_print1(&lcd[0],strptr2);
    // lcd_print1(&lcd[0],strptr3);
    // lcd_clear(&lcd[0]);
    // lcd_print1(&lcd[0],strptr3);
    // printf("%s\r\n",string1);
    // lcd_broadcast(string2);
    // strm_genAct(string1,"Toggle",2,1);
    // // printf("%s\r\n",string1);
    // lcd_print(&lcd[0], string1);//TODO get level_number and string cat
    // strm_genPush(string2,"Triangle",3);
    // // printf("%s\r\n",string2);
    // lcd_print(&lcd[1], string2);//TODO get level_number and string cat
    // lcd_print(&lcd[2], string2);//TODO get level_number and string cat
    // lcd_broadcast_after(numbers[9]);
    // lcd_print1(&lcd[0],strptr1);
    // // Reverse(strptr2);
    // lcd_print1(&lcd[1],strptr2);
    // lcd_print1(&lcd[2],strptr3);
    // lcd_print2(&lcd1, strptr1,strptr2);
    // strcpy(strptr1,strptr2);
    // lcd_print2(&lcd3, strptr1, strptr2);
    // lcd_stop(&lcd[1]);

    uint8_t space_loc[10];
    uint8_t* space_loc_ptr=space_loc;
    uint8_t spaces;
    uint8_t length;
    lcd_print(&lcd[0],string2);
    strm_Pig(string2,0,0);
    lcd_print(&lcd[1],string2);
    // lcd_print2(&lcd[2],numbers[space_loc[0]],numbers[space_loc[1]]);

    while(1) {
        //
    }
}
