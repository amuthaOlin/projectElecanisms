
#include <p24FJ128GB206.h>
#include "LCD.h"
#include "common.h"
#include "timer.h"
#include "ui.h"



// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 ?  E  RW RS

uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _IOWriteVal;


void init_lcd(_I2C *i2c, float freq) {
	    i2c_open(i2c, freq);
    	i2c_start(i2c);
    	init_delay();
    	lcd_init(i2c);

}

void delayMicroseconds(uint16_t uS){
	uint16_t count=0;
	while (count < uS) {
		if (timer_flag(&timer1)) {
	        timer_lower(&timer1);
	        count +=1;
	    }
	}
}


void lcd_init(_I2C *i2c){

	// Some bullshit according to pg 46
	i2c_write(i2c,0x30);//0b00110000
	delayMicroseconds(4500);

	i2c_write(i2c,0x30);//0b0000011
	led_on(&led2);
	delayMicroseconds(4500);



	i2c_write(i2c,0x30);//0b0000011
	delayMicroseconds(150);

	i2c_write(i2c,0x20); // Set to 4bit mode

	lcd_send(i2c, LCD_FUNCTIONSET | DISPLAYMASK, INTERNAL_WRITE); // Set rows and colums

	_displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
	lcd_displayoff(i2c);

	lcd_clear(i2c);
	

	_displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;

	lcd_send(i2c, LCD_ENTRYMODESET | _displaymode, INTERNAL_WRITE);

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
	delayMicroseconds(1);
	enableToggle();
	i2c_write(i2c,_IOWriteVal);
	delayMicroseconds(100);
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

void lcd_write(_I2C *i2c, uint8_t value){
	lcd_display(i2c);
	lcd_send(i2c, value, DR_WRITE);
}



void i2c_write(_I2C *i2c, uint8_t ch){
	i2c_putc(i2c, IOWRITE);
	i2c_putc(i2c,ch); 
	// i2c_idle(i2c);
}

void init_delay(){
	timer_setPeriod(&timer1, 0.000001);
    timer_start(&timer1);
}

