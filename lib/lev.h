#ifndef _LEV_H_
#define _LEV_H_

typedef struct _LEV {
    uint8_t[8] asteroids;  
    uint8_t[8] wormholes;
    uint8_t[6] label1;
    uint8_t[6] label2;
    uint8_t[6] label3;
    uint8_t lab_theme;
    uint8_t mod;
    uint8_t arg_freq;
    uint8_t arg_shift;
    float level_time;
    float cmd_time;
    uint8_t[6] actuators;
    char[33] message;

} _LEV;


#endif