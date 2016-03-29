#ifndef _CMDM_H_
#define _CMDM_H_

#include <stdint.h>
#include "timer.h"
#include "i2c.h"

void CamelCase(char* str);
void Caesar(char* str, uint8_t shift, uint8_t freq);
#endif