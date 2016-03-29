#include <p24FJ128GB206.h>

#include "common.h"
#include "lcd.h"
#include "timer.h"
#include "ui.h"
#include "strmanip.h"


void CamelCase(char* str){
    while(*str){
        if(*str >= 'a' && *str <= 'z'){
        *str = ('A' + *str - 'a');
        str++;
        }
        str++;
    }
}

void Caesar(char* str, uint8_t shift, uint8_t freq){
    while(*str){
        if(*str >= 'A' && *str <= 'z'){
        *str = *str+shift;
        str=str+freq; // Only shift every freq letters
        }
        str++;
    }
}


