#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include "common.h"
#include "timer.h"
#include "i2c.h"

void init_lcd(void);

typedef struct {
    _I2C *i2c
} _LCD;

void lcd_init(_I2C *i2c, float freq);
void lcd_display(_I2C *i2c);
void lcd_clear(_I2C *i2c);
void lcd_send(_I2C *i2c, uint8_t value, uint8_t command);
void enableToggle();
void enablePulse(_I2C *i2c);
void lcd_write(_I2C *i2c, uint8_t value);
void i2c_write(_I2C *i2c, uint8_t ch);
void init_delay();
void delayMicroseconds(uint16_t uS);
void lcd_goto(_I2C *i2c, uint8_t line, uint8_t col);
void lcd_send8(_I2C *i2c,uint8_t value, uint8_t command);
void lcd_print(_I2C *i2c,char *str);
void lcd_cursor(_I2C *i2c, uint8_t cur);

#endif





