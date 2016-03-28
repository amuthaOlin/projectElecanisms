#include <p24FJ128GB206.h>
#include "common.h"
#include "console.h"

_CONSOLE console1, console2, console3;

void slave_tx() {
    spi_queue_slave(&spi1, cmd);
    pin_set(Sint);
    pin_clear(Sint);
}

void handle_CSn(_INT *intx) {
    res = spi_read_slave(&spi1);
    //printf("res:%x%x\n\r",res.l[1],res.l[0]);
    if (res.l == 0xA1B2) {
        led_toggle(&led2);
    }
}

void init_slave_comms(void) {
    spi_open_slave(&spi1, MOSI, MISO, SCK, CSn, 1, 1);
    pin_digitalOut(Sint);
    pin_clear(Sint);
    pin_digitalIn(CSn);
    int_attach(&int1, CSn, 1, handle_CSn);
}

