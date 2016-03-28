#include <p24FJ128GB206.h>
#include "lcd.h"
#include "ui.h"

// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 ?  E  RW RS

// IO commands
#define IOWRITE 0x4E
#define IOREAD  0x4F

//
#define LCD_FUNCTIONSET 0x20
#define DISPLAYMASK 0x08
#define ENABLE_TOGGLE 0x04
#define LCD_BACKLIGHT 0x08

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// Modes
#define INTERNAL_WRITE 0x08
#define BUSY_READ 0x0A
#define DR_WRITE 0x09
#define DR_READ 0x0B

uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _IOWriteVal;

void init_lcd(_I2C *i2c) {
        i2c_open(i2c, 1e3);
        init_delay();
        lcd_init(i2c, 1e3);
}

void delayMicroseconds(uint16_t uS) {
    uint16_t count=0;
    while (count < uS) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            count +=1;
        }
    }
}

void lcd_init(_I2C *i2c, float freq) {
    i2c_write(i2c,0x00);

    delayMicroseconds(15000);

    // Some bullshit according to pg 46
    lcd_send8(i2c,0x03,INTERNAL_WRITE);
    delayMicroseconds(5000);

    lcd_send8(i2c,0x03,INTERNAL_WRITE);//0b00110000
    delayMicroseconds(5000);

    lcd_send8(i2c,0x03,INTERNAL_WRITE);//0b00110000
    delayMicroseconds(5000);

    // Put it in 4 bit mode
    lcd_send8(i2c,0x02,INTERNAL_WRITE);//0b00110000
    delayMicroseconds(5000);

    lcd_send(i2c, 0x28, INTERNAL_WRITE); // Set rows and direction
    delayMicroseconds(50);

    lcd_send(i2c, 0x80, INTERNAL_WRITE); // Display off, cursor off
    delayMicroseconds(50);
    
    lcd_send(i2c, 0x01, INTERNAL_WRITE); // Go to home position
    delayMicroseconds(2000);

    lcd_send(i2c, 0x06, INTERNAL_WRITE); // Set curson direction
    delayMicroseconds(5000);

    lcd_send(i2c, 0x0C, INTERNAL_WRITE); // Display on, cursor off
}

void lcd_display(_I2C *i2c){
    _displaycontrol |= LCD_DISPLAYON;
    lcd_send (i2c,_displaycontrol | LCD_DISPLAYCONTROL, INTERNAL_WRITE);
}

void lcd_displayoff(_I2C *i2c){
    _displaycontrol &= ~LCD_DISPLAYON;
    lcd_send (i2c,_displaycontrol | LCD_DISPLAYCONTROL, INTERNAL_WRITE);
}

void lcd_clear(_I2C *i2c){
    lcd_send(i2c,LCD_CLEARDISPLAY, INTERNAL_WRITE);
    delayMicroseconds(2000);
}

void enableToggle(){
    _IOWriteVal ^= ENABLE_TOGGLE;
}

void enablePulse(_I2C *i2c){
    enableToggle();
    i2c_write(i2c,_IOWriteVal);
    delayMicroseconds(100);
    enableToggle();
    i2c_write(i2c,_IOWriteVal);
    delayMicroseconds(1000);
}

void lcd_send(_I2C *i2c,uint8_t value, uint8_t command){
    uint8_t MS= value & 0x78;
    uint8_t LS= value << 4;
    _IOWriteVal = command | MS;
    i2c_write(i2c,_IOWriteVal);
    enablePulse(i2c);
    _IOWriteVal= command | LS;
    i2c_write(i2c,_IOWriteVal);
    enablePulse(i2c);
}

void lcd_send8(_I2C *i2c,uint8_t value, uint8_t command){
    value = value << 4;
    _IOWriteVal = command | value;
    i2c_write(i2c,_IOWriteVal);
    enablePulse(i2c);
}

void lcd_write(_I2C *i2c, uint8_t value){
    lcd_send(i2c, value, DR_WRITE);
    delayMicroseconds(1000);
}

void lcd_print(_I2C *i2c,char *str){
   while (*str){
       lcd_write(i2c,*str);
    str++;
   }
}

void i2c_write(_I2C *i2c, uint8_t ch){
    i2c_start(i2c);
    i2c_putc(i2c,IOWRITE);
    i2c_idle(i2c);
    i2c_putc(i2c,ch); 
    i2c_idle(i2c);
    i2c_stop(i2c);
}

void init_delay(){
    timer_setPeriod(&timer1, 0.000001);
    timer_start(&timer1);
}

void lcd_goto(_I2C *i2c, uint8_t line, uint8_t col) { //x=col, y=row
    uint8_t address;
    switch(line) {
        case 1:
            address = 0x00;
            led_on(&led1);
            break;
        case 2:
            // led_on(&led1);
            address = 0x40;
            break;
        default:
            address = 0x00;
            break;
    }

    address = address+col;
    lcd_send(i2c, LCD_SETDDRAMADDR | address, INTERNAL_WRITE);
}

void lcd_cursor(_I2C *i2c, uint8_t cur){
    switch(cur){
        case 0:
            lcd_send(i2c,0x0C, INTERNAL_WRITE);
            break;

        case 1:
            lcd_send(i2c,0x0E, INTERNAL_WRITE);
            break;

        default:
            break;
    }
}
