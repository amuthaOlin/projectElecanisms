#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include <stdint.h>
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "spi.h"
#include "timer.h"
#include "uart.h"
#include "spacecomms.h"


void init_console(void);

typedef struct _CONSOLE {
    WORD32 res;
    WORD32 state;
    _SPI *spi;
    _PIN *Sint;
    void (*poll)(struct _CONSOLE *self);
} _CONSOLE;

extern _CONSOLE console;

void console_init(_CONSOLE *self, _PIN *MISO, _PIN *MOSI, _PIN *SCK, _PIN *Sint, _PIN *CSn, _SPI *spi);
void console_tx(_CONSOLE *self, WORD32 cmd);
void console_attach_poll(_CONSOLE *self, void (*poll)(_CONSOLE *self));
void console_poll_changes(_CONSOLE *self);

#endif
