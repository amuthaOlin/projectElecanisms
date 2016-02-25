#include <p24FJ128GB206.h>
#include <stdint.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "usb.h"
#include "pin.h"
#include "i2c.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_pin();
    init_i2c();
}
