#ifndef _LABC_H_
#define _LABC_H_

typedef struct _LABC {
    // constant
    uint8_t lab_num[6];
    char lab_str[6][17];
    uint8_t lab_theme;
    uint8_t mod;
    uint8_t arg_freq;
    uint8_t arg_shift;
} _LABC;

void labc_setup(_LABC *level);
void labc_print(_LABC *level);
void labc_recievel(_LABC *level, uint8_t console);

#endif

