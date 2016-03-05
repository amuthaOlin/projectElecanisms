
#include <p24FJ128GB206.h>
#include "LCD.h"
#include "common.h"
#include "timer.h"
#include "ui.h"



// I2C Reg (MSB) P7 P6 P5 P4 P3 P2 P1 P0
// Driver pin    D7 D6 D5 D4 BL  E  RW RS

uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _IOWriteVal;


void init_lcd(_I2C *i2c, float freq) {
	    i2c_open(i2c, freq);
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

	lcd_send(i2c, 0xC, INTERNAL_WRITE); // Display on, cursor off

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


void lcd_goto(_I2C *i2c, uint8_t x, uint8_t y)   //x=col, y=row
{
uint8_t address;

   switch(y)
     {
      case 1:
        address = 0x00;
        break;

      case 2:
        address = 0x40;
        break;

      default:
        address = 0x00;
        break;
     }

   address += x-1;
   lcd_send(0, 0x80 | address, INTERNAL_WRITE);
}
