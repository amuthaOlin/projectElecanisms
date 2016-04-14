#ifndef _CON_H_
#define _CON_H_

#include <stdint.h>
#include "common.h"
#include "spacecomms.h"
#include "pin.h"
#include "lcd.h"
#include "cmd.h"
#include "cd.h"

void init_con(void);

typedef struct _CON {
    WORD32 state;
    WORD32 trans_res;

    _CMD *last_cmd;

    _CD *cd;
    _LCD *lcd;
    _PIN *SSn;
} _CON;

extern _CON con[3];

void con_init(_CON *self, _CD *cd, _LCD *lcd, _PIN *SSn);
void con_send_cmd(_CON *self, _CMD *cmd, float cd_time);
WORD32 con_transfer(_CON *self, WORD32 cmd);

uint8_t con_state_change(_CON *self);

#endif
