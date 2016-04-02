#include <p24FJ128GB206.h>
#include "common.h"
#include "ui.h"
#include "console.h"

_CONSOLE console;

void init_console(void) {
    console_init(&console, &D[1], &D[0], &D[2], &D[4], &D[3], &spi1);
}

void console_init(_CONSOLE *self, _PIN *MISO, _PIN *MOSI, _PIN *SCK, _PIN *Sint, _PIN *CSn, _SPI *spi){
    self->Sint = Sint;
    self->spi = spi;

    spi_open_slave(self->spi, MOSI, MISO, SCK, CSn, 1, 1);

    pin_digitalOut(self->Sint);
    pin_clear(self->Sint);
}

void console_attach_poll(_CONSOLE *self, void (*poll)(_CONSOLE *self)) {
    self->poll = poll;
}

void console_tx(_CONSOLE *self, WORD32 cmd) {
    spi_queue_slave(self->spi, cmd);
    pin_set(self->Sint);
    pin_clear(self->Sint);
}

void console_poll_changes(_CONSOLE *self) {
    self->last_state = self->state;
    self->poll(self);
    // printf("Last state: %x\r\n",self->last_state.l);
    // printf("Curr state: %x\r\n",self->state.l);
    if (self->last_state.l != self->state.l){
        console_tx(self, self->state);
    }
}
