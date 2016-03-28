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
	WORD32 res, cmd;
	WORD32 state;


	
	_PIN *MISO  = &D[1];
	_PIN *MOSI  = &D[0];
	_PIN *SCK   = &D[2];
	_PIN *CSn   = &D[3];
	_PIN *Sint  = &D[4];
} _CONSOLE;

extern _CONSOLE console1, console2, console3;


void slave_tx();
void handle_CSn(_INT *intx);
void init_slave_comms(void);

#endif
