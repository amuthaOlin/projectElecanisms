#include <p24FJ128GB206.h>
#include "common.h"
#include "ui.h"
#include "console.h"
#include "labc.h"

_CONSOLE console;

STATE_HANDLER_T state, last_state;

void console_s_change_lcds(void) {
    if (state != last_state){
        last_state = state;
        console_update_lcds();
        console.LCD_flag = 0;
    }
    state = console_s_level;
    if (state != last_state){

    }
}

void console_s_level(void) {
    if (state != last_state){
        last_state = state;
    }
    if(console.LCD_flag == 2){
        state = console_s_change_lcds;
    }
    if (state != last_state){

    }
}

void init_console(void) {
    console_init(&console, &D[1], &D[0], &D[2], &D[4], &D[3], &spi1);
}

void handle_CSn(_INT *intx) {
    console.res = spi_read_slave(console.spi);
    if (console.res.d1.packet == 1) {
        console.LCD_flag = 1;
        console.lcd_update1 = console.res;
    }
    if (console.res.d2.packet == 2) {
        console.LCD_flag = 2;
        console.lcd_update2 = console.res;
    }
    //printf("res:%x%x\n\r",res.l[1],res.l[0]);
}

void console_update_lcds(void) {
    level.lab_numb[0] = console.lcd_update1.d1.index1;
    level.lab_numb[1] = console.lcd_update1.d1.index2;
    level.lab_numb[2] = console.lcd_update1.d1.index3;
    level.lab_numb[3] = console.lcd_update1.d1.index4;
    level.lab_numb[4] = console.lcd_update1.d1.index5;
    level.lab_numb[5] = console.lcd_update1.d1.index6;
    level.lab_theme = console.lcd_update2.d2.theme;
    level.mod = console.lcd_update2.d2.mods;
    level.arg_freq = console.lcd_update2.d2.argument1;
    level.arg_shift = console.lcd_update2.d2.argument2;

    labc_setup();
}

void console_init(_CONSOLE *self, _PIN *MISO, _PIN *MOSI, _PIN *SCK, _PIN *Sint, _PIN *CSn, _SPI *spi){
    self->Sint = Sint;
    self->spi = spi;
    self->state = (WORD32)0;
    self->last_state = (WORD32)0;

    spi_open_slave(self->spi, MOSI, MISO, SCK, CSn, 1, 1);

    pin_digitalOut(self->Sint);
    pin_clear(self->Sint);
    int_attach(&int1, console.spi->SSn, 1, handle_CSn);
}

void console_attach_poll(_CONSOLE *self, void (*poll)(_CONSOLE *self)) {
    self->poll = poll;
}

void console_tx(_CONSOLE *self, WORD32 cmd) {
	printf("cmd:%lu\n\r",cmd.l );
    spi_queue_slave(self->spi, cmd);
    pin_set(self->Sint);
    pin_clear(self->Sint);
}

void console_poll_changes(_CONSOLE *self) {
    self->last_state = (*self).state;
    self->poll(self);
    if (self->last_state.l != self->state.l) {
        console_tx(self, self->state);
    }
}


