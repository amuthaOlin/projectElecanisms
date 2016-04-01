#include <p24FJ128GB206.h>
#include "common.h"
#include "console.h"

_CONSOLE console;

void init_console(void) {
	console_init(&console, &D[1], &D[0], &D[2], &D[4], &D[3], &spi1, poll_state);  
}

void console_init(_CONSOLE *self, _PIN *MISO, _PIN *MOSI, _PIN *SCK, _PIN *Sint, _PIN *CSn, _SPI *spi, void (*poll)(_CONSOLE *self)){
	self->Sint = Sint;
	self->CSn = CSn;
	self->poll = poll;
	self->spi = spi;
	spi_open_slave(self->spi, MOSI, MISO, SCK, CSn, 1, 1);
    pin_digitalOut(self->Sint);
    pin_clear(self->Sint);
    pin_digitalIn(self->CSn);
    int_attach(&int1, self->CSn, 1, handle_CSn);
    self->poll();

}

void console_tx(_CONSOLE *self) {
    spi_queue_slave(self->spi, self->cmd);
    pin_set(self->Sint);
    pin_clear(self->Sint);
}



WORD32 last_state;
void console_poll_changes(_CONSOLE *self){
    last_state = self->state;
    self->poll();
    //printf("last:%x\n\r",last_state.l);
    //printf("state:%x\n\r",self->state.l);
    if (last_state.l != self->state.l){
        //led_toggle(&led1);
        self->cmd.l = self->state.l; 
        console_tx(&console);
    }
    
}

