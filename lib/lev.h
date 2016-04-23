#ifndef _LEV_H_
#define _LEV_H_

typedef struct _LEV {
    uint8_t asteroids[8];  
    uint8_t wormholes[8];
    uint8_t label[3][6];
    char label_name1[6][17];
    char label_name2[6][17];
    char label_name3[6][17];
    uint8_t lab_theme;
    uint8_t mod;
    uint8_t arg_freq;
    uint8_t arg_shift;
    float level_time;
    float cmd_time;
    uint8_t actuators;
    char message[33];

} _LEV;

void lev_init(_LEV *level);
void __lev_pickLabels(_LEV *level, uint8_t theme);
void lev_setup(_LEV *level, uint8_t lev_num);
void lev_send(_LEV *level, uint8_t console);
void lev_genCmd(_LEV *level);
char* lev_getName(_LEV *level, uint8_t console, uint8_t actuator);
void lev_printCmd(uint8_t cons, uint8_t cmd_num);

#endif