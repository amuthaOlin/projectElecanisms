#ifndef _LABC_H_
#define _LABC_H_

typedef struct _LABC {
    // constant
    uint8_t lab_numb[6];
    char lab_str[6][17];
    uint8_t lab_theme;
    uint8_t mod;
    uint8_t arg_freq;
    uint8_t arg_shift;
} _LABC;

extern _LABC level;

void labc_setup(void);
void labc_print(void);

#endif

