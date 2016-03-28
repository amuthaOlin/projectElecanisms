#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include "timer.h"
#include "i2c.h"

void init_lcd(void);

typedef struct {
    _I2C *i2c;

    float freq;
    uint8_t addr_write;
    uint8_t addr_read;
} _LCD;

extern _LCD lcd1, lcd2, lcd3, lcd4;

void lcd_init(_LCD *self, _I2C *i2c, float freq, uint8_t addr);
void lcd_display(_LCD *self);
void lcd_clear(_LCD *self);
void lcd_send(_LCD *self, uint8_t value, uint8_t command);
void lcd_write(_LCD *self, uint8_t value);
void lcd_goto(_LCD *self, uint8_t line, uint8_t col);
void lcd_send8(_LCD *self, uint8_t value, uint8_t command);
void lcd_print(_LCD *self, char *str);
void lcd_cursor(_LCD *self, uint8_t cur);

void init_delay();
void delayMicroseconds(uint16_t uS);
void enableToggle();
void enablePulse(_LCD *self);

#endif
