#include <p24FJ128GB206.h>
#include "con.h"

_CON con[3];

void init_con(void) {
    _PIN *SSn[] = { &D[3], &D[5], &D[7] };

    uint8_t i;
    for (i = 0; i < 3; i++) {
        con_init(&con[i], &cd[i], &lcdcmd[i], SSn[i]);
    }
}

void con_init(_CON *self, _CD *cd, _LCD *lcd, _PIN *SSn) {
    self->cd = cd;
    self->lcd = lcd;
    self->SSn = SSn;

    self->state = (WORD32)0;
    self->trans_res = (WORD32)0;

    pin_digitalOut(self->SSn);
    pin_set(self->SSn);
}

void con_send_cmd(_CON *self, _CMD *cmd, float cd_time) {
    self->last_cmd = cmd;
    self->trans_res = con_transfer(self, cmd_packet(cmd->index));

    lcd_clear(self->lcd);
    lcd_print1(self->lcd, cmd_strs[cmd->index]);

    cd_start(self->cd, cd_time, game_clock);
}

// returns command success/failure
uint8_t con_state_change(_CON *self) {
    self->trans_res = con_transfer(self, (WORD32)0xFEEDF00D);

    self->state = self->trans_res;
    uint8_t success = cmd_test(self->last_cmd->index, self->state);

    return self->cd->active && success;
}

WORD32 con_transfer(_CON *self, WORD32 cmd) {
    WORD32 tmp;
    pin_clear(self->SSn);
    tmp = spi_queue(&spi1, cmd);
    pin_set(self->SSn);
    return tmp;
}