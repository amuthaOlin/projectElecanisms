#ifndef _SPACECOMMS_H
#define _SPACECOMMS_H

#include <stdint.h>

typedef struct SPACK_DIR {
  unsigned packet:2;
  unsigned actaddr:5;
  unsigned actact:6;
  unsigned long :19;
} SPACK_DIR;

typedef struct CONS1_STATE {
  unsigned red_button:1;
  unsigned toggle1:1;
  unsigned toggle2:1;
  unsigned joystick:4;
  unsigned wormhole1:1;
  unsigned wormhole2:1;
  unsigned hotsystem:1; 
  unsigned long :22;
} CONS1_STATE;

uint8_t CONS1_NUMACTS = 9;
uint8_t CONS1_STATES[] = {
    1,
    2,
    2,
    2,
    2,
    2,
    2,
    1,
    1
}

typedef struct CONS2_STATE {
  unsigned red_button:1;
  unsigned green_button1:1;
  unsigned green_button2:1;
  unsigned green_button3:1;
  unsigned green_button4:1;
  unsigned wormhole1:1;
  unsigned wormhole2:1;
  unsigned tri_state:2;
  unsigned slider:2;
  unsigned hotsystem1:1;
  unsigned hotsystem2:1;
  unsigned hotsystem3:1;
  unsigned hotsystem4:1;
  unsigned long :17;
} CONS2_STATE;

uint8_t CONS2_NUMACTS = 11;
uint8_t CONS2_STATES[] = {
    1,
    2,
    2,
    2,
    2,
    2,
    0,
    1,
    1,
    3,
    4
}

typedef struct CONS3_STATE {
  unsigned red_button:1;
  unsigned triangle1:1;
  unsigned triangle2:1;
  unsigned triangle3:1;
  unsigned toggle1:1;
  unsigned toggle2:1;
  unsigned wormhole1:1;
  unsigned wormhole2:1;
  unsigned long :24;
} CONS3_STATE;

uint8_t CONS3_NUMACTS = 9;
uint8_t CONS3_STATES[] = {
    1,
    2,
    2,
    1,
    1,
    1,
    0,
    1,
    1
}

#endif