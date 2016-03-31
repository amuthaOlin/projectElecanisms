#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "pin.h"
#include "int.h"
#include "spi.h"
#include "timer.h"
#include "uart.h"
#include "spacecomms.h"
#include "console.h"




void poll_state(){
    state.s1.red_button = pin_read(&D[5]);
}



int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_int();
    init_spi();
    init_timer();
    init_uart();

    cmd.l = 0xBEEFFACE;
    init_slave();

    timer_setPeriod(&timer4, 0.01);
    timer_start(&timer4);

   

    while(1) {
        if (timer_flag(&timer4)) {
            timer_lower(&timer4);
            poll_changes();
        }
        //     printf("Slave sent: 0x%x\r\n", 0x5A);
        //     printf("Slave received: 0x%x\r\n", res);
        
    }
}
