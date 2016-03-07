#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "int.h"
#include "i2c.h"
#include "uart.h"
#include "timer.h"
#include "LCD.h"

uint8_t last_pos = 0;
uint8_t pos = 0;

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_uart();
    init_i2c();
    init_timer();
    init_delay();
    init_lcd(&i2c3, 1e3);

    uint16_t pot_val;

    while(1) {
        pot_val = pin_read(&A[0]) >> 6;

        printf("Potentiometer: %x\r\n", pot_val);

        if (pot_val > 0x02ff) {
            pos = 4;
            if (pos != last_pos) {
                lcd_clear(&i2c3);
                lcd_write(&i2c3, 'P');
                lcd_write(&i2c3, 'O');
                lcd_write(&i2c3, 'S');
                lcd_write(&i2c3, 'I');
                lcd_write(&i2c3, 'T');
                lcd_write(&i2c3, 'I');
                lcd_write(&i2c3, 'O');
                lcd_write(&i2c3, 'N');
                lcd_write(&i2c3, ' ');
                lcd_write(&i2c3, '4');
            }
        } else if (pot_val > 0x01ff) {
            pos = 3;
            if (pos != last_pos) {
                lcd_clear(&i2c3);
                lcd_write(&i2c3, 'P');
                lcd_write(&i2c3, 'O');
                lcd_write(&i2c3, 'S');
                lcd_write(&i2c3, 'I');
                lcd_write(&i2c3, 'T');
                lcd_write(&i2c3, 'I');
                lcd_write(&i2c3, 'O');
                lcd_write(&i2c3, 'N');
                lcd_write(&i2c3, ' ');
                lcd_write(&i2c3, '3');
            }
        } else if (pot_val > 0x0100) {
            pos = 2;
            if (pos != last_pos) {
                lcd_clear(&i2c3);
                lcd_write(&i2c3, 'P');
                lcd_write(&i2c3, 'O');
                lcd_write(&i2c3, 'S');
                lcd_write(&i2c3, 'I');
                lcd_write(&i2c3, 'T');
                lcd_write(&i2c3, 'I');
                lcd_write(&i2c3, 'O');
                lcd_write(&i2c3, 'N');
                lcd_write(&i2c3, ' ');
                lcd_write(&i2c3, '2');
            }
        } else {
            pos = 1;
            if (pos != last_pos) {
                lcd_clear(&i2c3);
                lcd_write(&i2c3, 'P');
                lcd_write(&i2c3, 'O');
                lcd_write(&i2c3, 'S');
                lcd_write(&i2c3, 'I');
                lcd_write(&i2c3, 'T');
                lcd_write(&i2c3, 'I');
                lcd_write(&i2c3, 'O');
                lcd_write(&i2c3, 'N');
                lcd_write(&i2c3, ' ');
                lcd_write(&i2c3, '1');
            }
        }
        last_pos = pos;
    }
}
