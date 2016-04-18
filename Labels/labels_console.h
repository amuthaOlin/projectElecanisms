#ifndef _LABCON_H_
#define _LABCON_H_

typedef struct _LEVEL {
    // constant
    uint8_t lab_num[6];
    char lab_str[6][17];
    uint8_t lab_theme;
    uint8_t mod;
    uint8_t arg_freq;
    uint8_t arg_shift;
} _LEVEL;

void label_setup(_LEVEL *level);
void label_print(_LEVEL *level);
void recieve_level(_LEVEL *level, uint8_t console);

#endif

