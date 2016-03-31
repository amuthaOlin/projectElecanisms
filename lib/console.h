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

typedef struct {
	WORD32 res
	WORD32 cmd;
	WORD32 state;

	_PIN *MISO;
	_PIN *MOSI;
	_PIN *SCK;
	_PIN *CSn;
	_PIN *Sint;
	void (*poll_state)(struct _CONSOLE *self);

} _CONSOLE;

extern _CONSOLE console1, console2, console3;


void slave_tx(void);
void handle_CSn(_INT *intx);
void init_slave_comms(void);
void poll_changes(void);
void init_slave(void);

#endif
