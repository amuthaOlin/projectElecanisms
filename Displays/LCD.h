#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include "pin.h"
#include "timer.h"


// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 ?  E  RW RS


// IO commands
#define IOWRITE 0x4E
#define IOREAD	0x4F

//
#define LCD_FUNCTIONSET 0x20
#define DISPLAYMASK 0x08
#define ENABLE_TOGGLE 0x04

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

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
#define INTERNAL_WRITE 0x00
#define BUSY_READ 0x01
#define DR_WRITE 0x02
#define DR_READ 0x03


uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _IOWriteVal;

void init_lcd(_I2C *i2c, float freq);
void lcd_init(_I2C *i2c);
void lcd_display(_I2C *i2c);
void lcd_clear(_I2C *i2c);
void lcd_send(_I2C *i2c,uint8_t value, uint8_t command);
void lcd_write(_I2C *i2c, uint8_t value);
void enableToggle(void);
void enablePluse(_I2C *i2c);
void i2c_write(_I2C *i2c, uint8_t ch);


#endif

