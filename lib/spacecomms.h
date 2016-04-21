#ifndef _SPACECOMMS_H
#define _SPACECOMMS_H

#include <stdint.h>

extern uint8_t CONS_NUMACTS[];
extern uint8_t CONS_STATES[][15];
extern uint8_t CONS_HASREST[][15];
extern uint8_t CONS_GROUP[][15];

typedef struct SPACK_DIR {
  unsigned packet:2;
  unsigned actaddr:5;
  unsigned actact:6;
  unsigned long :19;
} SPACK_DIR;

typedef struct CONS1_STATE {
  unsigned red_button:1;        // 0
  unsigned toggle1:1;           // 1
  unsigned toggle2:1;           // 2
  unsigned green_button:1;      // 3
  unsigned joystick:4;          // 4
  unsigned hotsystem:1;         // 7
  unsigned wordwheel_inside:4;  // 8
  unsigned wordwheel_outside:4; // 9
  unsigned asteroid:1;          // 10
  unsigned wormhole1:1;         // 5
  unsigned wormhole2:1;         // 6
  unsigned long :13;
} CONS1_STATE;

typedef struct CONS2_STATE {
  unsigned red_button:1;        // 0
  unsigned green_button1:1;     // 1
  unsigned green_button2:1;     // 2
  unsigned green_button3:1;     // 3
  unsigned green_button4:1;     // 4
  unsigned tri_state:2;         // 7
  unsigned slider:2;            // 8
  unsigned hotsystem1:1;        // 9
  unsigned hotsystem2:1;        // 10
  unsigned hotsystem3:1;        // 11
  unsigned hotsystem4:1;        // 12
  unsigned dial:3;              // 14
  unsigned asteroid:1;          // 13
  unsigned wormhole1:1;         // 5
  unsigned wormhole2:1;         // 6
  unsigned long :13;
} CONS2_STATE;

typedef struct CONS3_STATE {
  unsigned red_button:1;        // 0
  unsigned triangle1:1;         // 1
  unsigned triangle2:1;         // 2
  unsigned triangle3:1;         // 3
  unsigned toggle1:1;           // 4
  unsigned toggle2:1;           // 5
  unsigned clutch:2;            // 8
  unsigned arming:2;            // 10
  unsigned arming_button:1;     // 11
  unsigned dial:3;              // 12
  unsigned asteroid:1;          // 9
  unsigned wormhole1:1;         // 6
  unsigned wormhole2:1;         // 7
  unsigned long :15;
} CONS3_STATE;



#endif