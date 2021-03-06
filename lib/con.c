#include <p24FJ128GB206.h>
#include "con.h"

#include "spi.h"

_CON con[3];

WORD32 con_transfer(_CON *self, WORD64 cmd) {
    WORD32 tmp;
    pin_clear(self->SSn);
    tmp = spi_queue(&spi1, cmd);
    pin_set(self->SSn);
    return tmp;
}

void init_con(void) {
    _PIN *SSn[] = { &D[3], &D[5], &D[7] };

    con_init(&con[0], &cd1, &lcd[0], SSn[0], 0);
    con_init(&con[1], &cd2, &lcd[1], SSn[1], 1);
    con_init(&con[2], &cd3, &lcd[2], SSn[2], 2);
}

void con_init(_CON *self, _CD *cd, _LCD *lcd, _PIN *SSn, uint8_t num) {
    self->cd = cd;
    self->lcd = lcd;
    self->SSn = SSn;
    self->num = num;

    self->state = (WORD32)0;

    pin_digitalOut(self->SSn);
    pin_set(self->SSn);
}

void con_send_cmd(_CON *self, _CMD *cmd, float cd_time, int32_t game_clock) {
    self->last_cmd = cmd;

    lcd_print(self->lcd, cmd->command);

    cd_start(self->cd, cd_time, game_clock);
}

void con_state_change(_CON *self) {
    WORD64 empty = (WORD64){0,0,0,0,0,0,0,0};
    self->state = con_transfer(self, empty);
}